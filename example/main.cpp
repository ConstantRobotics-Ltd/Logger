#include <iostream>
#include <thread>
#include "Logger.h"

// Link namespaces.
using namespace std;
using namespace cr::utils;

// Print thread function.
void printThreadFunction(PrintColor colour, PrintFlag flag)
{
    // Init local logger.
    Logger log;

    // Thread function.
    int counter = 0;
    while (true)
    {
        // Print something in console.
        log.print(colour, flag) << "Print thread output " << counter++ << endl;
    }
}

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

    // Run print threads.
    thread printThread1(&printThreadFunction, PrintColor::GREEN, PrintFlag::CONSOLE_AND_FILE);
    thread printThread2(&printThreadFunction, PrintColor::BLUE, PrintFlag::CONSOLE);

    // Main pring loop.
    int counter = 0;
    while (true)
    {
        // Print something in console and file.
        log.print(PrintColor::RED, PrintFlag::CONSOLE_AND_FILE) <<
        "Main thread output " << counter++ << endl;
    }

    return 1;
}


