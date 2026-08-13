#include "scroll.h"
#include <sense.h>
#include <stddef.h>

static int rotation = 0;
static uint16_t draw_color = 0;

void set_display_rotation(int r) { rotation = r; }
void set_display_color(uint16_t c) { draw_color = c; }

static pi_framebuffer_t *fb = NULL;

static int wrap8(int v) {
    int r = v % 8;
    return (r < 0) ? r + 8 : r;
}

void open_display(void) {
    if (fb == NULL) {
        fb = getFrameBuffer();
        clear_display();
    }
}

void close_display(void) {
    if (fb != NULL) {
        clear_display();
        freeFrameBuffer(fb);
        fb = NULL;
    }
}

void clear_display(void) {
    if (fb != NULL) {
        uint16_t black = getColor(0, 0, 0);
        clearFrameBuffer(fb, black);
    }
}

static int letter_pixel(char letter, int x, int y) {
    if (letter == 'S') {
        int pattern[5][5] = {
            {1,1,1,1,1},
            {1,0,0,0,0},
            {1,1,1,1,1},
            {0,0,0,0,1},
            {1,1,1,1,1}
        };
        return (x>=0 && x<5 && y>=0 && y<5) ? pattern[y][x] : 0;
    }

    if (letter == '3') {
        int pattern[5][5] = {
            {1,1,1,1,1},
            {0,0,0,0,1},
            {0,1,1,1,1},
            {0,0,0,0,1},
            {1,1,1,1,1}
        };
        return (x>=0 && x<5 && y>=0 && y<5) ? pattern[y][x] : 0;
    }

    return 0;
}

void display_letter(char letter, int xOffset, int yOffset) {
    if (fb == NULL) return;

    clear_display();

    uint16_t color = draw_color;

    for (int ly = 0; ly < 5; ly++) {
        for (int lx = 0; lx < 5; lx++) {
            if (!letter_pixel(letter, lx, ly)) continue;

            int rx = lx;
            int ry = ly;

            if (rotation == 90) {
                rx = ly;
                ry = 4 - lx;
            } else if (rotation == 180) {
                rx = 4 - lx;
                ry = 4 - ly;
            } else if (rotation == 270) {
                rx = 4 - ly;
                ry = lx;
            }

            int x = wrap8(xOffset + rx);
            int y = wrap8(yOffset + ry);

            fb->bitmap->pixel[y][x] = color;
        }
    }
}

