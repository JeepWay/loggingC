/**
 * Copyright (c) 2025 JeepWay
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "logger.h"

#include <assert.h>
#include <string.h>
#include <time.h>

struct logger {
    log_LockFn lock;
    handler_t handlers[MAX_HANDLERS];
    size_t count;
};

static logger_t L = {
    .lock = NULL,
    .count = 0,
};

static void lock(void)
{
    if (L.lock) {
        L.lock(true, L.handlers[0].fp);
    }
}

static void unlock(void)
{
    if (L.lock) {
        L.lock(false, L.handlers[0].fp);
    }
}

void log_set_lock(log_LockFn fn, void *fp)
{
    L.lock = fn;
    L.handlers[0].fp = fp;
}

__attribute__((constructor)) static void init_logger(void)
{
    L.handlers[ROOT_HANDLER] = (handler_t){
        .name = ROOT_HANDLER_NAME,
        .dump_fn = dump_log,
        .fmt_fn = color_fmt1,
        .fp = DEFAULT_STRAEM,
        .level = LOG_TRACE,
        .quiet = false,
        .date_fmt = DEFAULT_DATE_FORMAT1,
    };
    L.count = 1;
}

static int log_add_handler(const char *name, log_dump_fn dump_fn,
                           log_fmt_fn fmt_fn, void *fp, size_t level,
                           bool quiet, const char *date_fmt)
{
    if (L.count == MAX_HANDLERS) {
        fprintf(DEFAULT_STRAEM,
                "[Logger C] Maximum number of handlers reached: %d\n",
                MAX_HANDLERS);
        return -1;
    }

    L.handlers[L.count++] = (handler_t){
        .name = name,
        .dump_fn = dump_fn,
        .fmt_fn = fmt_fn,
        .fp = fp,
        .level = level,
        .quiet = quiet,
        .date_fmt = date_fmt,
    };
    return 0;
}

int log_add_file_handler(const char *filename, const char *filemode,
                         size_t level, const char *name)
{
    assert(level >= LOG_TRACE && level <= LOG_FATAL);
    assert(name);
    filename = filename ? filename : DEFAULT_FILE_NAME;
    filemode = filemode ? filemode : DEFAULT_FILE_MODE;
    FILE *fp = fopen(filename, filemode);

    if (!fp) {
        fprintf(DEFAULT_STRAEM, "[Logger C] Unable to open log file: %s\n",
                filename);
        return -1;
    }

    return log_add_handler(name, dump_log, no_color_fmt1, fp, level, false,
                           DEFAULT_DATE_FORMAT3);
}

int log_add_stream_handler(FILE *fp, size_t level, const char *name)
{
    assert(level >= LOG_TRACE && level <= LOG_FATAL);
    assert(name);
    fp = fp ? fp : DEFAULT_STRAEM;
    return log_add_handler(name, dump_log, color_fmt1, fp, level, false,
                           DEFAULT_DATE_FORMAT1);
}

static void update_record(record_t *rec, handler_t *hd)
{
    if (!rec->time) {
        time_t t = time(NULL);
        rec->time = localtime(&t);
    }
    rec->hd_name = hd->name;
    rec->hd_fmt_fn = hd->fmt_fn;
    rec->hd_fp = hd->fp;
    rec->hd_date_fmt = hd->date_fmt;
}

void _log_message(int level, const char *file, int line, const char *msg_fmt,
                  ...)
{
    lock();
    record_t rec = {
        .level = level,
        .file = file,
        .line = line,
        .msg_fmt = msg_fmt,
    };

    handler_t *rh = &L.handlers[ROOT_HANDLER];
    if (!rh->quiet && level >= rh->level) {
        update_record(&rec, rh);
        va_start(rec.ap, msg_fmt);
        rh->dump_fn(&rec);
        va_end(rec.ap);
    }

    for (int i = ROOT_HANDLER + 1; i < L.count && L.handlers[i].dump_fn; i++) {
        handler_t *hd = &L.handlers[i];
        if (!hd->quiet && level >= hd->level) {
            update_record(&rec, hd);
            va_start(rec.ap, msg_fmt);
            hd->dump_fn(&rec);
            va_end(rec.ap);
        }
    }
    unlock();
}

static const char *modifiable_members = "dump_fn fmt_fn level quiet date_fmt";

void _log_set_attribute(const char *name, const char *member, size_t offset,
                        size_t size, void *value)
{
    name = name ? name : ROOT_HANDLER_NAME;

    if (!strstr(modifiable_members, member)) {
        fprintf(DEFAULT_STRAEM,
                "[Logger C] Handler's member can't be modified: %s\n", member);
        return;
    }

    for (int i = ROOT_HANDLER; i < L.count; i++) {
        if (strcmp(L.handlers[i].name, name) == 0) {
            memcpy((void *) &L.handlers[i] + offset, value, size);
            return;
        }
    }
    fprintf(DEFAULT_STRAEM, "[Logger C] Handler's name not found: %s\n", name);
}

void dump_log(record_t *rec)
{
    char time_buf[32];
    time_buf[strftime(time_buf, sizeof(time_buf), rec->hd_date_fmt,
                      rec->time)] = '\0';
    rec->hd_fmt_fn(rec, time_buf);
    vfprintf(rec->hd_fp, rec->msg_fmt, rec->ap);
    fprintf(rec->hd_fp, "\n");
    fflush(rec->hd_fp);
}

void color_fmt1(record_t *rec, const char *time_buf)
{
    static const char *fmt = "%s %s%-5s\x1b[0m \x1b[90m[%s:%d]:\x1b[0m ";
    fprintf(rec->hd_fp, fmt, time_buf, level_colors[rec->level],
            level_strings[rec->level], rec->file, rec->line);
}

void color_fmt2(record_t *rec, const char *time_buf)
{
    static const char *fmt = "%s (%s) %s%-5s\x1b[0m \x1b[90m[%s:%d]:\x1b[0m ";
    fprintf(rec->hd_fp, fmt, time_buf, rec->hd_name, level_colors[rec->level],
            level_strings[rec->level], rec->file, rec->line);
}

void no_color_fmt1(record_t *rec, const char *time_buf)
{
    static const char *fmt = "%s %-5s [%s:%d]: ";
    fprintf(rec->hd_fp, fmt, time_buf, level_strings[rec->level], rec->file,
            rec->line);
}

void no_color_fmt2(record_t *rec, const char *time_buf)
{
    static const char *fmt = "%s (%s) %-5s [%s:%d]: ";
    fprintf(rec->hd_fp, fmt, time_buf, rec->hd_name, level_strings[rec->level],
            rec->file, rec->line);
}
