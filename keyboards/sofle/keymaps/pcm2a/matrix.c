// matrix.c
#include "matrix.h"

#define NUM_STARS 50
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define MAX_SPEED 5

typedef struct {
    int x;
    int y;
    int speed;
} Star;

Star stars[NUM_STARS];
static uint32_t last_activity_time = 0; // Tracks the last activity time

void init_star(Star *star) {
    star->x = rand() % SCREEN_WIDTH;
    star->y = rand() % SCREEN_HEIGHT;
    star->speed = 1 + rand() % MAX_SPEED;
}

void init_stars(void) {
    for (int i = 0; i < NUM_STARS; i++) {
        init_star(&stars[i]);
    }
    last_activity_time = timer_read32(); // Initialize activity timer
}

static void update_stars(void) {
    for (int i = 0; i < NUM_STARS; i++) {
        stars[i].x -= stars[i].speed;
        if (stars[i].x < 0) {
            stars[i].x = SCREEN_WIDTH - 1;
            stars[i].y = rand() % SCREEN_HEIGHT;
            stars[i].speed = 1 + rand() % MAX_SPEED;
        }
    }
}

static void render_stars(int flag) {
    for (int i = 0; i < NUM_STARS; i++) {
        if (flag == 0) {
            oled_write_pixel(stars[i].x, stars[i].y, false);
        } else {
            oled_write_pixel(stars[i].x, stars[i].y, true);
        }

    }
}

void update_and_render_stars(void) {
    if (timer_read32() < (last_activity_time + (30 * 1000))) {
        render_stars(0);
        update_stars();
        render_stars(1);
    }
}

// Function to reset the OLED timer, call this on any activity
void reset_oled_timer(void) {
    last_activity_time = timer_read32();
}
