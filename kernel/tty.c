#include <kernel/tty.h>
#include <string.h>
#include <drivers/screen.h>
#include <stdlib.h>

struct tty *tty_array[TTY_COUNT] = { NULL };
struct tty *tty_current;

struct tty *tty_create() {
    struct tty *tty = malloc(sizeof(*tty));

    memset(tty->buffer, 0, 25 * 80);
    tty->cursor = 0;

    int index = -1;
    for(int i = 0; i < 8; i++) {
        if(tty_array[i] == NULL) {
            index = i;
            break;
        }
    }

    if(index == -1) {
        return NULL;
    }

    tty->id = index;
    tty_array[index] = tty;
    char str[25];
    sprintf(str, "--- [TTY %d] ---\n", tty->id);
    tty_print(tty, str);

    return tty;
}

void tty_switch(struct tty *tty) {
    tty_current = tty;
    /* printf("New TTY: %d\n", tty->id); */
    tty_draw(tty);
}

struct tty *tty_get_current() {
    return tty_current;
}

struct tty *tty_get_next(struct tty *tty) {
    struct tty *next = NULL;

    int i = 0;
    while(next == NULL && i < TTY_COUNT) {
        i++;
        next = tty_array[(tty->id + i) % TTY_COUNT];
    }

    return next;
}

struct tty *tty_get_prev(struct tty *tty) {
    struct tty *next = NULL;

    unsigned int i = 0;
    while(next == NULL && -i < TTY_COUNT) {
        i--;
        next = tty_array[(tty->id + i) % TTY_COUNT];
    }

    return next;
}

struct tty *tty_get_prev(struct tty *);

void tty_draw(struct tty *tty) {
    clear_screen();
    set_cursor(0, 0);
    int bufi = 0;
    print(tty->buffer);
    set_cursor_offset(tty->cursor);

}

void tty_print(struct tty *tty, char *string) {
    strcpy(tty->buffer + tty->cursor, string);
}

void tty_print_char(struct tty *tty, char c) {
    tty->buffer[tty->cursor] = c;
    tty->cursor++;
}
