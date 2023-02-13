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

enum class PrintFlag
{
    CONSOLE = 1,
    FILE = 2,
    CONSOLE_AND_FILE = CONSOLE | FILE
};

class LoggerSettings
{
public:
    int count{0};
    std::string prefix{""};
    std::string folder{""};
    int maxFileSizeMb{1};
    int maxFolderSizeMb{100};
};

class ColorPrint
{
public:
    /**
     * @brief Class constructor.
     *
     * @param color Logger color
     * @param file File to add
     */
    ColorPrint(PrintColor color, LoggerSettings settings,
               uint8_t flags, std::string fileName);

    /**
     * @brief Default destructor
     *
     * The destructor prints all messages, that were added by << operator,
     * before the tracer are destroyed
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

    void print(PrintColor color, const std::string &msg, uint8_t flags);

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
 * @brief Implement tracing
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
     *
     * @param level Minimum trace level at which messages will be printed
     * @param name Tracer name.
     */
    Logger();

    /**
     * @brief Class destructor.
     */
    ~Logger();

    /**
     * @brief Method for setting file for a specific tracer
     *
     * @param filename The name of the file where the log will be printed.
     * @param name Tracer name. If name is empty, this file will be set for all
     * tracers.
     *
     * @return true if a tracer was found by name
     */
    static bool setSaveLogParams(std::string folder, std::string filePrefix,
                          int maxFolderSizeMb, int maxFileSizeMb);

    /**
     * @brief Methods to prints the received message through the operator "<<"
     *
     * @param msgLevel Message importance level. If it is higher or equal to
     * the minimum level of the tracer, then the message will be printed
     *
     * @return object that, when destroyed, will output a message to the stream.
     */
    ColorPrint print(PrintColor msgColor, PrintFlag flags = PrintFlag::CONSOLE);

private:
    static LoggerSettings m_settings;
    static std::string m_fileName;
    static std::mutex m_fileSysMutex;

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
    static std::vector<FileInfo> getFilesInFolder(std::string folder);

    /**
     * @brief Remove unnecessary files.
     * @param files List of files info.
     * @param maxFolderSizeMb Max folder size MB.
     */
    static void removeUnnecessaryFiles(std::vector<FileInfo> files,
                                int maxFolderSizeMb);

    /**
     * @brief Reinit Logging Process.
     */
    static bool _reinitLoggingProcess();
};
}
}

