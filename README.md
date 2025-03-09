# LoggingC
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE) 
![root-default.png](fig/root-default.png)

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
2. Include the header file [logging.h](loggingC/logging.h) and source file [logging.c](loggingC/logging.c) in your project. Below in an example (main.c): 
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
The following `DEFAULT` values are defined in [logging.h](loggingC/logging.h#L26), you can use them to set the default values for convenience, otherwise, you can type what you want.

### 1. Change default log level
The default log level of root handler is `TRACE`. You can change the log level by using `log_set_level(char* name, size_t value)`, where `name` is the handler name and `value` is the log level.
```cpp
log_set_level(DEFAULT, LOG_INFO);
// Equal to the following line
log_set_level("root", LOG_INFO); 
```
![root-info.png](fig/root-info.png)

### 2. Add a file handler
The defualt setting only contains a stream handler (root handler). You can add a file handler to log messages to a file by using `log_add_file_handler(char *filename, char *filemode, size_t level, char *name)`, where `filename` is the name of the file, `filemode` is the mode to open the file, `level` is the log level, and `name` (can't be `NULL`) is the handler name.
```cpp
log_add_file_handler(DEFAULT, DEFAULT, DEFAULT_LEVEL, "file1");
// Equal to the following line
log_add_file_handler("loggingC/program.log", "a", LOG_INFO, "file1");
```
And then you can see the log messages in the file `loggingC/program.log`.
![file-default.png](fig/file-default.png)

## 3. Add a stream handler
You can add a stream handler to log messages to the console by using `log_add_stream_handler(FILE *fp, size_t level, char *name)`, where `fp` is the file pointer, `level` is the log level, and `name` (can't be `NULL`) is the handler name.
```cpp
log_add_stream_handler(DEFAULT, DEFAULT_LEVEL, "bash2");
// Equal to the following line
log_add_stream_handler(stderr, LOG_INFO, "bash2");
```
You will see the log messages in the console, as the log level of `bash2` is `INFO`, the log level of `root` is `TRACE`, so only the log messages with level `INFO` and above will appear twice in the console.
![two-stream-handlers.png](fig/two-stream-handlers.png)

## 4. Make handler quiet
You can make a handler quiet by using `log_set_quiet(char* name, bool value)`, where `name` is the handler name and `value` is a boolean value.
```cpp
log_set_quiet("root", true);
log_add_stream_handler(DEFAULT, DEFAULT_LEVEL, "bash2");
```
The console will only show the log messages from the `bash2` handler, as the `root` handler is quiet.
![root-quiet.png](fig/root-quiet.png)

## 5. Change data and time format







## Advanced Usage


## Contributions
Contributions are welcome! Please submit a pull request or open an issue on the repository. Here are some ways you can contribute:
* Implement filter function, like [filter class](https://docs.python.org/3/library/logging.html#filter-objects) in Python's logging package, to filter log messages based on max log level, handler name.
* Make the sequential search of handlers more efficient, for example, hashing the handler names into id.
* Preventing from adding the handler with the same name.
* Add more safety checks in [logging.c](loggingC/logging.c#L175) to ensure robustness and prevent potential issues.
* Add more sequence diagrams and documentation to help new users understand the internal workings of this library.

## Reference
* [log4.c](https://github.com/rxi/log.c) - The project that LoggingC based on.
* [Python logging module](https://docs.python.org/3/library/logging.html) - Comprehensive guide on Python's logging package.
* [Logging in Python](https://realpython.com/python-logging/) - An atricle to introduce logging package.

## License
This library is free, you can redistribute and modify it under the MIT License, see the [LICENSE](https://github.com/JeepWay/loggingC/blob/master/LICENSE) for details.
