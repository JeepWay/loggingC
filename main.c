#include <stdio.h>
#include <stdlib.h>

#include "logger/logger.h"

void custom_fmt(record_t *rec, const char *time_buf)
{
    static const char *fmt = "%s\x1b[0m \x1b[90m[%s:%d] %s%-5s:\x1b[0m ";
    fprintf(rec->hd_fp, fmt, time_buf, rec->file, rec->line, 
        level_colors[rec->level], level_strings[rec->level]);
}

int main(void)
{
    /* 1. Change default log level */
    // log_set_level("root", LOG_INFO); 
    
    /* 2. Add a file handler */
    // log_add_file_handler("logger/program.log", "a", LOG_INFO, "file1");
    
    /* 3. Add a stream handler */
    // log_add_stream_handler(DEFAULT, DEFAULT_LEVEL, "bash2");
    
    /* 4. Make a handler quiet */
    // log_set_quiet("root", true);
    // log_add_stream_handler(DEFAULT, DEFAULT_LEVEL, "bash2");
    
    /* 5. Change data and time format */
    // log_set_date_fmt("root", DEFAULT_DATE_FORMAT3);

    /* 6. Change text format */
    // log_set_fmt_fn("root", color_fmt2);
    // log_set_fmt_fn("root", no_color_fmt2);
    // log_set_fmt_fn("root", custom_fmt);

    /* Example 1 */
    // char *f1 = "file1";
    // log_add_file_handler("logger/program.log", "w", LOG_WARN, f1);
    // log_set_fmt_fn(f1, no_color_fmt2);
    // log_set_quiet("root", true);

    log_trace("Trace message %d", 1);
    log_debug("Debug message %d", 2);
    log_info("Info message %d", 3);
    log_warn("Warn message %d", 4);
    log_error("Error message %d", 5);
    log_fatal("Fatal message %d", 6);
    return 0;
}