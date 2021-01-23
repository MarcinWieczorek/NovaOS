#pragma once

#define __NEED_size_t
#include <stdint.h>
#include <stddef.h>

#define TTY_COUNT 8

struct tty {
    int id;
    char buffer[25 * 80];
    size_t cursor;
    struct proc *proc_fg;
};

struct tty *tty_create();

device_t *tty_dev_init(struct tty *);

void tty_free(struct tty *);

void tty_switch(struct tty *);

struct tty *tty_get(int index);

struct tty *tty_get_current();

struct tty *tty_get_next(struct tty *);

struct tty *tty_get_prev(struct tty *);

void tty_print(struct tty *, char *string);

void tty_print_char(struct tty *tty, char c);

void tty_draw(struct tty *);

int tty_get_cursor(struct tty *);

void tty_set_cursor(struct tty *, int row, int col);

void tty_keyboard_input(struct tty *tty, char c);

