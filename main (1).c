#include "scroll.h"
#include <sense.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <stdlib.h>

static char letters[2] = {'S', '3'};
static int num_letters = 2;
static int current_index = 0;

static double pos_x = 0.0;
static double pos_y = 0.0;
static double vel_x = 0.0;
static double vel_y = 0.0;

static int running = 1;

uint16_t scroll_color = 0;
int rotation = 0;
int speed_scale = 1;

void print_help(const char *prog) {
    printf("Usage: %s [options]\n", prog);
    printf("  -h              Show help\n");
    printf("  -c COLOR        white, red, green, blue\n");
    printf("  -r DEGREES      0, 90, 180, 270\n");
    printf("  -s SCALE        Speed multiplier\n");
    exit(0);
}

void handler(unsigned int code) {
    if (code == KEY_LEFT) {
        vel_x -= 1.0 * speed_scale;
        if (vel_x < -10.0) vel_x = -10.0;
    } else if (code == KEY_RIGHT) {
        vel_x += 1.0 * speed_scale;
        if (vel_x > 10.0) vel_x = 10.0;
    } else if (code == KEY_UP) {
        vel_y -= 1.0 * speed_scale;
        if (vel_y < -10.0) vel_y = -10.0;
    } else if (code == KEY_DOWN) {
        vel_y += 1.0 * speed_scale;
        if (vel_y > 10.0) vel_y = 10.0;
    } else if (code == KEY_ENTER) {
        current_index++;
        if (current_index >= num_letters) running = 0;
    }
}

int main(int argc, char **argv) {
    scroll_color = getColor(255,255,255);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0) {
            print_help(argv[0]);
        } else if (strcmp(argv[i], "-c") == 0) {
            if (++i >= argc) print_help(argv[0]);
            if (strcmp(argv[i], "white") == 0) scroll_color = getColor(255,255,255);
            else if (strcmp(argv[i], "red") == 0) scroll_color = getColor(255,0,0);
            else if (strcmp(argv[i], "green") == 0) scroll_color = getColor(0,255,0);
            else if (strcmp(argv[i], "blue") == 0) scroll_color = getColor(0,0,255);
            else print_help(argv[0]);
        } else if (strcmp(argv[i], "-r") == 0) {
            if (++i >= argc) print_help(argv[0]);
            rotation = atoi(argv[i]);
            if (rotation != 0 && rotation != 90 && rotation != 180 && rotation != 270)
                print_help(argv[0]);
        } else if (strcmp(argv[i], "-s") == 0) {
            if (++i >= argc) print_help(argv[0]);
            speed_scale = atoi(argv[i]);
            if (speed_scale < 1 || speed_scale > 10) print_help(argv[0]);
        } else {
            print_help(argv[0]);
        }
    }

    set_display_rotation(rotation);
    set_display_color(scroll_color);

    open_display();
    open_input();

    const int delay_ms = 50;
    const double dt = delay_ms / 1000.0;

    while (running) {
        check_input(handler, delay_ms);

        pos_x += vel_x * dt;
        pos_y += vel_y * dt;

        while (pos_x < 0.0) pos_x += 8.0;
        while (pos_x >= 8.0) pos_x -= 8.0;
        while (pos_y < 0.0) pos_y += 8.0;
        while (pos_y >= 8.0) pos_y -= 8.0;

        int ix = ((int)round(pos_x)) % 8;
        int iy = ((int)round(pos_y)) % 8;

        display_letter(letters[current_index], ix, iy);
    }

    clear_display();
    close_input();
    close_display();
    return 0;
}