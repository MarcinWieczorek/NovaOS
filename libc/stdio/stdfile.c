#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <../internal/stdio_impl.h>

// stdin
FILE __stdin_FILE = {
    .buf = NULL,
    .buf_size = 0,
    .fd = STDIN_FILENO,
    .flags = 0,
    .write = NULL,
    .seek = NULL,
    .close = NULL,
};

FILE *const stdin = &__stdin_FILE;

// stdout
FILE __stdout_FILE = {
    .buf = NULL,
    .buf_size = 0,
    .fd = STDOUT_FILENO,
    .flags = 0,
    .lbf = '\n',
    .write = NULL,
    .seek = NULL,
    .close = NULL,
    .lock = -1,
};

FILE *const stdout = &__stdout_FILE;

// stderr
FILE __stderr_FILE = {
    .buf = NULL,
    .buf_size = 0,
    .fd = STDERR_FILENO,
    .flags = 0,
    .lbf = '\n',
    .write = NULL,
    .seek = NULL,
    .close = NULL,
    .lock = -1,
};

FILE *const stderr = &__stderr_FILE;
