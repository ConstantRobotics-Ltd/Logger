#include <iostream>
#include <thread>
#include "Logger.h"

// Link namespaces.
using namespace std;
using namespace cr::utils;

// Print thread function prototype.
void printThreadFunction();

// Entry point.
int main(void)
{
    cout << "=================================================" << endl;
    cout << "Logger v" << Logger::getVersion() << " example"    << endl;
    cout << "=================================================" << endl;
    cout << endl;

    // Init logger.
    Logger log;

    // Set Logger global parameters to write log in files.
    string folder = "LoggerExample";
    string filePrefix = "LOG";
    int maxFolderSizeMb = 3;
    int maxFileSizeMb = 1;
    Logger::setSaveLogParams(folder, filePrefix, maxFolderSizeMb, maxFileSizeMb);

    // Run print thread.
    thread printThread(&printThreadFunction);

    // Main pring loop.
    int counter = 0;
    while (true)
    {
        // Print something in console and file.
        log.print(PrintColor::RED, PrintFlag::CONSOLE_AND_FILE) <<
        "Main thread output" << counter++ << endl;
    }

    return 1;
}

// Print thread function prototype.
void printThreadFunction()
{
    // Init local logger.
    Logger log;

    // Thread function.
    int counter = 0;
    while (true)
    {
        // Print something in console.
        log.print(PrintColor::GREEN, PrintFlag::CONSOLE) <<
        "Print thread output " << counter++ << endl;
    }

}
