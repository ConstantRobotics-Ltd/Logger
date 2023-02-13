#include <iostream>
#include <map>

#include "Logger.h"
using namespace cr::utils;


// Entry point.
int main(void)
{
    std::cout<<"=================================================" << std::endl;
    std::cout<<"LoggerExample " << Logger::getVersion()            << std::endl;
    std::cout<<"=================================================" << std::endl;
    std::cout<<std::endl;

    std::string folder = "LoggerExample";
    std::string filePrefix = "LOG";
    int maxFolderSizeMb = 100;
    int maxFileSizeMb = 1;
    Logger::setSaveLogParams(folder, filePrefix, maxFolderSizeMb, maxFileSizeMb);
    Logger log;

    std::thread test([=](Logger* log) {
        while (true)
            log->print(PrintColor::GREEN) << "THREAD" << std::endl;
    }, &log);

    while(true) {
        log.print(PrintColor::RED, PrintFlag::FILE) << "MAIN" << std::endl;
    }

	return 1;
}
