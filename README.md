# LoggingC
![default.png](fig/default.png)

A lightweight, flexible logging library for C language, inspired by [log4.c](https://github.com/rxi/log.c) project and Python's [logging](https://docs.python.org/3/library/logging.html) package. 
LoggingC provides a simple yet powerful way to log messages with different levels, handlers, and formatting options. It supports both stream and file handlers, customizable log formats.

## Features
- Default root handler (a stream handler) for immediate use, you don't need to create it.
- More file and stream handler can be added.
- Multiple log levels: `TRACE`, `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`.
- Customizable log formatting with color support for terminals.
- Flexible date/time formatting.

## Simple Usage
1. Clone or download the repository:
   ```bash
   git clone https://github.com/JeepWay/loggingC.git
    ```
2. Include the header file [logging.h](https://github.com/JeepWay/loggingC/blob/main/loggingC/logging.h) and source file [logging.c](https://github.com/JeepWay/loggingC/blob/main/loggingC/logging.c) in your project. Below in an example (main.c): 
    ```cpp
    #include <stdio.h>
    #include <stdlib.h>
    #include "loggingC/logging.h"
    int main(void)
    {
        log_trace("Trace message %d", 1);
        log_debug("Debug message %d", 2);
        log_info("Info message %d", 3);
        log_warn("Warn message %d", 4);
        log_error("Error message %d", 5);
        log_fatal("Fatal message %d", 6);
        return 0;
    }
    ```
3. Compile your program with a C compiler (e.g., gcc), and execute it.
    ```bash
    gcc -o main loggingC/logging.c main.c
    ./main
    ```
4. You will see the result in the console like the top picture.

## Basic Usage


## Advanced Usage


## Contributions
Contributions are welcome! Please submit a pull request or open an issue on the repository. Here are some ways you can contribute:
* Implement filter function, like [filter class](https://docs.python.org/3/library/logging.html#filter-objects) in Python's logging package, to filter log messages based on max log level, handler name.
* Make the sequential search of handlers more efficient, for example, hashing the handler names into id.
* Add more safety checks in [logging.c](https://github.com/JeepWay/loggingC/blob/main/loggingC/logging.c#L175) to ensure robustness and prevent potential issues.
* Add more sequence diagrams and documentation to help new users understand the internal workings of this library.

## Reference
* [log4.c](https://github.com/rxi/log.c) - The project that LoggingC based on.
* [Python logging module](https://docs.python.org/3/library/logging.html) - Comprehensive guide on Python's logging package.
* [Logging in Python](https://realpython.com/python-logging/) - An atricle to introduce logging package.

## License
This library is free, you can redistribute and modify it under the MIT License, see the [LICENSE](https://github.com/JeepWay/loggingC/blob/master/LICENSE) for details.
