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

namespace cr {
namespace utils {

/// Colors for print
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

/// Options for print
enum class PrintFlag
{
    CONSOLE = 1,
    FILE = 2,
    CONSOLE_AND_FILE = CONSOLE | FILE
};

class LoggerSettings
{
public:
    std::string prefix{""};  /// start filename (e.g. /LOG***.txt)
    std::string folder{""};  /// log folder
    int maxFileSizeMb{0};    /// max file size (Mb)
    int maxFolderSizeMb{0};  /// max folder size (Mb)
};

class ColorPrint
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
    ColorPrint(PrintColor color, LoggerSettings settings,
               uint8_t flags, std::string filename);

    /**
     * @brief Default destructor
     *
     * The destructor prints all messages, that were added by << operator,
     * before the ColorPrint are destroyed
     */
    ~ColorPrint();

    /**
     * @brief Add message to the print
     * @tparam T type of the data to print
     * @param msg The message to print
     * @return
     */
    template <typename T>
    std::ostringstream& operator << (T msg) {
        *m_stream << msg;
        return *m_stream;
    }

private:
    static std::mutex m_printMutex;
    std::ostringstream* m_stream;
    PrintColor m_color;
    LoggerSettings m_settings;
    uint8_t m_flags;
    std::string m_fileName;
};

/**
 * @class
 *
 * @brief Implement logger
 */
class Logger
{
public:

    /**
     * @brief Method to get string of current version of library.
     *
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
     *
     * @param folder Log folder
     * @param filePrefix Start log file filename
     * @param maxFolderSizeMb Max file size (Mb)
     * @param maxFileSizeMb Max folder size (Mb)
     *
     * @return true if params are set.
     */
    static bool setSaveLogParams(
            std::string folder, std::string filePrefix,
            int maxFolderSizeMb, int maxFileSizeMb);

    /**
     * @brief Methods to prints message through the operator "<<"
     *
     * @param color Print color
     * @param flags Options for print
     *
     * @return object that will output a message to the stream when destroyed.
     */
    ColorPrint print(PrintColor color, PrintFlag flags = PrintFlag::CONSOLE);

private:
    static LoggerSettings m_settings;
    static std::string m_fileName;
    static std::mutex m_fileSysMutex;
    static bool m_isSettingsSetup;

    /// File info structure.
    typedef struct {
        std::filesystem::path name;
        uint32_t size;
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

