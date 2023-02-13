## CONTENTS
- [OVERVIEW](#overview)
- [API DESCRIPTION](#api-description)
  - [getVersion(..)](#getversion)
  - [CTOR and DTOR(..)](#ctro-and-dtor)
  - [setSaveLogParams(..)](#setsavelogparams)
  - [print(..)](#print)
- [USAGE EXAMPLE](#usage-example)

## OVERVIEW

This project helps implement simple color logging logic.

## API DESCRIPTION

The **Logger** contains one main class `Logger` which can be used 
to simplify logging. 

### getVersion(..)

```cpp
/**
* @brief Method to get string of current version of library.
*
* @return String of current library version.
*/
static std::string getVersion();
```

##### Description

*Static method to get string of current version of library.*

### CTOR and DTOR(..)

```cpp
/**
 * @brief Class constructor.
 */
Logger();

/**
 * @brief Class destructor.
 */
~Logger();
```
##### Description

*constructor and destructor methods to create Logger.*

### setSaveLogParams(..)

```cpp
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
```

##### Description

*Static method for setting params for loggers.*

### print(..)

```cpp
 /**
 * @brief Methods to prints message through the operator "<<"
 *
 * @param color Print color
 * @param flags Options for print
 *
 * @return object that will output a message to the stream when destroyed.
 */
ColorPrint print(PrintColor color, PrintFlag flags = PrintFlag::CONSOLE);
```

##### Description

*Methods to prints the received message through the operator "<<".*

## USAGE EXAMPLE

```cpp
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
```
