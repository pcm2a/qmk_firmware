// starfield.c
#include "starfield.h"
#include <math.h>

#define NUM_STARS 50
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define CENTER_X (SCREEN_WIDTH / 2)
#define CENTER_Y (SCREEN_HEIGHT / 2)
#define MAX_SPEED 5

typedef struct {
    float x;
    float y;
    float speed;
    float angle;
} Star;

static Star stars[NUM_STARS];
static uint32_t last_activity_time = 0; // Tracks the last activity time

void init_star(Star *star) {
    star->x = CENTER_X;
    star->y = CENTER_Y;
    star->speed = ((float)rand() / RAND_MAX) * MAX_SPEED;
    star->angle = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
}

void init_stars(void) {
    for (int i = 0; i < NUM_STARS; i++) {
        init_star(&stars[i]);
    }
    last_activity_time = timer_read32(); // Initialize activity timer
}

static void update_stars(void) {
    for (int i = 0; i < NUM_STARS; i++) {
        stars[i].x += stars[i].speed * cosf(stars[i].angle);
        stars[i].y += stars[i].speed * sinf(stars[i].angle);

        if (stars[i].x < 0 || stars[i].x >= SCREEN_WIDTH || stars[i].y < 0 || stars[i].y >= SCREEN_HEIGHT) {
            init_star(&stars[i]);
        }
    }
}

static void render_stars(int flag) {
    for (int i = 0; i < NUM_STARS; i++) {
        if (flag == 0) {
            oled_write_pixel((int)stars[i].x, (int)stars[i].y, false);
        } else {
            oled_write_pixel((int)stars[i].x, (int)stars[i].y, true);
        }
    }
}

void update_and_render_stars(void) {
    if (timer_read32() < (last_activity_time + (60U * 1000U))) {
        render_stars(0);
        update_stars();
        render_stars(1);
    } else {
        oled_off();
    }
}

// Function to reset the OLED timer, call this on any activity
void reset_oled_timer(void) {
    last_activity_time = timer_read32();
}
