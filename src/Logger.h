#pragma once
#include <thread>
#include <mutex>
#include <string.h>
#include <sstream>
#include <memory>
#include <vector>
#include <map>
#include <stdio.h>
#include <filesystem>



#if defined(linux) || defined(__linux) || defined(__linux__)
#define __LOGFILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#else
#define __LOGFILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif



namespace cr
{
namespace utils
{

/// Print color indexes.
enum class PrintColor
{
    NORMAL,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE
};



/// Print options.
enum class PrintFlag
{
    CONSOLE = 1,
    FILE = 2,
    CONSOLE_AND_FILE = CONSOLE | FILE,
    DISABLE
};



/// Logger settings.
class LoggerSettings
{
public:
    /// start filename (e.g. /LOG***.txt).
    std::string prefix{""};
    /// log folder.
    std::string folder{""};
    /// max file size (Mb).
    int maxFileSizeMb{0};
    /// max folder size (Mb).
    int maxFolderSizeMb{0};
};



/// Colour pring class.
class ColourPrint
{
public:
    /**
     * @brief Class constructor.
     *
     * @param color Print color
     * @param settings Logger settings for print
     * @param flags Additional printing options
     * @param filename Print file name
     */
    ColourPrint(PrintColor color,
                LoggerSettings settings,
                uint8_t flags,
                std::string filename);

    /**
     * @brief Default destructor
     *
     * The destructor prints all messages, that were added by << operator,
     * before the ColorPrint are destroyed
     */
    ~ColourPrint();

    /**
     * @brief Add message to the print
     * @tparam T type of the data to print
     * @param msg The message to print
     * @return
     */
    template <typename T>
    std::ostringstream& operator << (T msg) {
        if (m_flags != (uint8_t)PrintFlag::DISABLE)
            *m_stream << msg;
        return *m_stream;
    }

private:

    /// Print mutex.
    static std::mutex m_printMutex;
    /// Pring stream.
    std::ostringstream* m_stream;
    /// Print colour.
    PrintColor m_color;
    /// Print parameters.
    LoggerSettings m_settings;
    /// Print flags.
    uint8_t m_flags;
    /// File name.
    std::string m_fileName;
};



/**
 * @brief Logger main class.
 */
class Logger
{
public:

    /**
     * @brief Method to get string of current version of library.
     * @return String of current library version.
     */
    static std::string getVersion();

    /**
     * @brief Class constructor.
     */
    Logger();

    /**
     * @brief Class destructor.
     */
    ~Logger();

    /**
     * @brief Method for setting params for loggers
     * @param folder Log folder
     * @param filePrefix Start log file filename
     * @param maxFolderSizeMb Max file size (Mb)
     * @param maxFileSizeMb Max folder size (Mb)
     * @return true if params are set.
     */
    static bool setSaveLogParams(std::string folder,
                                 std::string filePrefix,
                                 int maxFolderSizeMb,
                                 int maxFileSizeMb);

    /**
     * @brief Methods to prints message through the operator "<<".
     * @param color Print color
     * @param flags Options for print
     * @return object that will output a message to the stream when destroyed.
     */
    ColourPrint print(PrintColor color, PrintFlag flags = PrintFlag::CONSOLE);

private:

    /// Logger parameters.
    static LoggerSettings m_settings;
    /// File name.
    static std::string m_fileName;
    /// Global mutex.
    static std::mutex m_fileSysMutex;
    /// Settings initiated flag.
    static bool m_isSettingsSetup;

    /// File info structure.
    typedef struct {
        /// Full file name.
        std::filesystem::path name;
        /// File size.
        uint32_t size;
        /// File time.
        std::filesystem::file_time_type time;
    } FileInfo;

    /**
     * @brief Get files in folder.
     * @param folder Folder path.
     * @return List of files info.
     */
    static std::vector<FileInfo> _getFilesInFolder(std::string folder);

    /**
     * @brief Remove unnecessary files.
     * @param files List of files info.
     * @param maxFolderSizeMb Max folder size MB.
     */
    static void _removeUnnecessaryFiles(
            std::vector<FileInfo> files, int maxFolderSizeMb);

    /**
     * @brief Reinit Logging Process.
     */
    static bool _reinitLoggingProcess();
};
}
}

