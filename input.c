#include "scroll.h"
#include <sense.h>
#include <stddef.h>

static pi_joystick_t *js = NULL;

void open_input(void) {
    if (js == NULL) {
        js = getJoystickDevice();
    }
}

void close_input(void) {
    if (js != NULL) {
        freeJoystick(js);
        js = NULL;
    }
}

void check_input(void (*callback)(unsigned int), int delay) {
    if (js == NULL) return;
    pollJoystick(js, callback, delay);
}
