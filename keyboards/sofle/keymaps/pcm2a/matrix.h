// starfield.h
#ifndef STARFIELD_H
#define STARFIELD_H

#include QMK_KEYBOARD_H

void init_stars(void);
void update_and_render_stars(void);
void reset_oled_timer(void);

#endif // STARFIELD_H

