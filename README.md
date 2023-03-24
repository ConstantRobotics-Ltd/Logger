![logger_logo](_static/logger_logo.png)

**v1.1.0**

------

# Table of contents

- [Overview](#Overview)
- [Versions](#Versions)
- [Supported pixel formats](#Supported-pixel-formats)
- [Frame class description](#Frame-class-description)
  - [Frame class declaration](#Frame-class-declaration)
  - [Default constructor](#Default-constructor)
  - [Constructor with parameters](#Constructor-with-parameters)
  - [Copy-constructor](#Copy-constructor)
  - [getVersion method](#getVersion-method)
  - [Copy operator =](#Copy-operator-=)
  - [cloneTo method](#cloneTo-method)
  - [Compare operator ==](#Compare-operator-==)
  - [Compare operator !=](#Compare-operator-!=)
  - [release method](#release-method)
  - [serialize method](#serialize-method)
  - [deserialize method](#deserialize-method)
  - [Frame class public members](#Frame-class-public-members)

# Overview

**Logger** C++ library provides logging functions: printing in terminal and(or) printing in file. File **Logger.h** contains declaration of class **Logger** which provides logging functions. **Logger** class provide thread safe method to print info file or terminal. The user can create multiple **Logger** class objects and the library will provide thread-safe output to the terminal and thread-safe writing to the file (all data will be written to the file in sequence). The **Logger** class also provides color output to the terminal. When writing the log to a text file, the **Logger** class controls the size of the file as well as the total size of the files in the directory and deletes old files if the directory size is exceeded.

# Versions

**Table 1** - Library versions.

| Version | Release date | What's new                                                   |
| ------- | ------------ | ------------------------------------------------------------ |
| 1.0.0   | 13.02.2023   | First version.                                               |
| 1.0.0   | 24.03.2023   | - Added new print option DISABLE.<br />- Example updated.<br />- Documentation updated. |

# Logger class description

## Logger class declaration

**Logger.h** file contains **Logger** class declaration. Class declaration:

```cpp
namespace cr
{
namespace utils
{
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
};
}
}
```

## Default constructor

Default Logger class constructor doesn't do anything. It doesn't allocate memory. Constructor declaration:

```cpp
Logger();
```

## getVersion method

**getVersion()** method return string of current version of **Logger** class. Method declaration:

```cpp
static std::string getVersion();
```

Method can be used without **Logger** class instance. Example:

```cpp
std::cout << "Logger class version: " << cr::utils::Logger::getVersion() << std::endl;
```

## setSaveLogParams method

**setSaveLogParams(...)** static method intended to set global parameter for writing logs into text files. This method is static. The method sets parameters for all objects of the **Logger** class. If a method is called in more than one place in the program, the last parameters will be applied to all objects of the **Logger** class. If the method is not called, all objects of the **Logger** class will only be able to output information to the console. The methos used without **Logger** class instance. Method declaration:

```cpp
static bool setSaveLogParams(std::string folder, std::string filePrefix, int maxFolderSizeMb, int maxFileSizeMb);
```

| Parameter       | Description                                                  |
| --------------- | ------------------------------------------------------------ |
| folder          | Path to folder. Example: /home/pi/Log                        |
| filePrefix      | File prefix. The library creates files with names: PREFIX_DATE_TIME.txt. Example: LOG_2023.03.24_15.55.59.txt. |
| maxFolderSizeMb | Maximum folder size. the library controls size of all files in folder. If overall size of files in folder > maxFolderSizeMb the library deletes oldest files in folder so the overall size of files < maxFolderSizeMb. |
| maxFileSizeMb   | Maximum file size, MB. Library creates files and controls their size. If size of file > maxFileSizeMb the library creates new file. |

**Returns:** TRUE if parameters are set or FALSE if not. 

Example:

```cpp
string folder = "LoggerExample";
string filePrefix = "LOG";
int maxFolderSizeMb = 3;
int maxFileSizeMb = 1;
Logger::setSaveLogParams(folder, filePrefix, maxFolderSizeMb, maxFileSizeMb);
```

## print method

**print(...)** method intended to print data in console and(or) file. The method provides the same interface as **std::cout** method and supports all **std::cout** formats. The method allows the user to define the colour of the output to the console and the direction of the output (console, file or file and console). Method declaration:

```cpp
ColourPrint print(PrintColor color, PrintFlag flags = PrintFlag::CONSOLE);
```

| Parameter | Description                                                  |
| --------- | ------------------------------------------------------------ |
| color     | Print color. Colors defined in **PrintColor** enum (logger.h file). |
| flags     |                                                              |

**Returns:** TRUE if parameters are set or FALSE if not. 











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
