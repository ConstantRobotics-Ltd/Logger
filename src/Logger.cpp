#include "Logger.h"
#include "LoggerVersion.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <map>

#include <fstream>
#include <string>
#include <iostream>

using namespace cr::utils;
using namespace std::filesystem;

std::mutex ColorPrint::m_printMutex;
LoggerSettings Logger::m_settings{0};
std::mutex Logger::m_fileSysMutex;
std::string Logger::m_fileName = "";
bool Logger::m_isSettingsSetup = false;

namespace cr::utils {
std::string getDayTimeString(std::string format)
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), format.c_str());
    std::string str = ss.str();

    return str;
}
}


ColorPrint::ColorPrint(PrintColor color, LoggerSettings settings,
                       uint8_t flags, std::string fileName)
    : m_stream(new std::ostringstream())
    , m_color(color)
    , m_settings(settings)
    , m_flags(flags)
    , m_fileName(fileName)
{
}

ColorPrint::~ColorPrint()
{
    static std::map<PrintColor, std::string> colors {
        { PrintColor::NORMAL,   "\x1B[0m"   },
        { PrintColor::RED,      "\x1B[31m"  },
        { PrintColor::GREEN,    "\x1B[32m"  },
        { PrintColor::YELLOW,   "\x1B[33m"  },
        { PrintColor::BLUE,     "\x1B[34m"  },
        { PrintColor::MAGENTA,  "\x1B[35m"  },
        { PrintColor::CYAN,     "\x1B[36m"  },
        { PrintColor::WHITE,    "\x1B[37m"  },
    };

    if (m_flags & (uint8_t)PrintFlag::CONSOLE)
    {
        ColorPrint::m_printMutex.lock();
        fprintf (stdout, "%s", colors[m_color].c_str());
        fprintf (stdout, "%s", (*m_stream).str().c_str());
        fprintf (stdout, "%s", colors[PrintColor::NORMAL].c_str());
        ColorPrint::m_printMutex.unlock();

        std::cout << std::flush;
    }

    if (m_flags & (uint8_t)PrintFlag::FILE)
    {
        ColorPrint::m_printMutex.lock();
        std::ofstream out(m_fileName, std::ios::app);
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        auto transformed = std::chrono::system_clock::now().time_since_epoch().count() / 1000;
        auto mks = transformed % 1000000;
        out << getDayTimeString("%Y-%m-%d %H:%M:%S.") << mks << " "
            << (*m_stream).str(); out.close();
        ColorPrint::m_printMutex.unlock();
    }
    delete m_stream;
}

std::string Logger::getVersion()
{
    return LOGGER_VERSION;
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

bool Logger::setSaveLogParams(std::string folder, std::string filePrefix,
                              int maxFolderSizeMb, int maxFileSizeMb)
{
    if (folder == "")
        return false;

    m_fileSysMutex.lock();
    m_settings.folder = folder;
    m_settings.prefix = filePrefix;
    m_settings.maxFolderSizeMb = maxFolderSizeMb;
    m_settings.maxFileSizeMb = maxFileSizeMb;

    m_isSettingsSetup = true;
    _reinitLoggingProcess();

    m_fileSysMutex.unlock();


    return true;
}



std::vector<Logger::FileInfo> Logger::_getFilesInFolder(std::string folder)
{
    // List of files info.
    std::vector<Logger::FileInfo> files;

    // Iterate files in folder.
    for (const auto & entry : directory_iterator(folder))
    {
        Logger::FileInfo info;
        info.name = entry.path();
        info.size = (uint32_t)entry.file_size();
        info.time = entry.last_write_time();
        files.push_back(info);
    }

    // Return result.
    return files;
}

void Logger::_removeUnnecessaryFiles(std::vector<FileInfo> files, int maxFolderSizeMb)
{
    // Calculate folder size.
    uint32_t size = 0;
    for (int i = 0; i < files.size(); ++i)
        size += files[i].size;

    // Check folder size.
    if ((size / 1048576) <= (uint32_t)maxFolderSizeMb)
        return;

    // Sort by date.
    std::sort(files.begin(), files.end(),
    [](Logger::FileInfo &a, Logger::FileInfo &b)
    { return a.time < b.time; });

    // Delete old files antil the folder size will be less maxFolderSizeMb.
    for (int i = 0; i < files.size(); ++i)
    {
        remove(files[i].name);
        size -= files[i].size;
        // Check size.
        if ((size / 1048576) <= (uint32_t)maxFolderSizeMb)
            break;
    }
}

bool Logger::_reinitLoggingProcess()
{
    if (!exists(m_settings.folder))
    {
        const std::filesystem::path folder{ m_settings.folder };
        std::filesystem::create_directory(folder);
    }

    // List files in directory.
    std::vector<Logger::FileInfo> files = _getFilesInFolder(m_settings.folder);

    // Remove unnecessary files.
    _removeUnnecessaryFiles(files, m_settings.maxFolderSizeMb);

    // Init videof writer.
#if defined(linux) || defined(__linux) || defined(__linux__)
    m_fileName = m_settings.folder + "/" + m_settings.prefix + "_" +
            getDayTimeString("%Y.%m.%d_%H.%M.%S") + ".txt";
#else
    m_fileName = m_settings.folder + "\\" + m_settings.prefix + "_" +
            getDayTimeString("%Y.%m.%d_%H.%M.%S") + ".txt";
#endif

    return true;
}


ColorPrint Logger::print(PrintColor msgColor,  PrintFlag flags)
{
    // Check file size.
    m_fileSysMutex.lock();
    if (m_isSettingsSetup && std::filesystem::exists(m_fileName))
    {
        path filePath = m_fileName;
        uint32_t fileSize = (uint32_t)file_size(filePath);
        if ((fileSize / 1048576) >= (uint32_t)m_settings.maxFileSizeMb)
        {
            _reinitLoggingProcess();
        }
    }
    m_fileSysMutex.unlock();

    return ColorPrint(msgColor, m_settings, (uint8_t)flags, m_fileName);
}


