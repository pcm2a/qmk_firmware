// Step 1.
// Declare custom effects using the RGB_MATRIX_EFFECT macro
// (note the lack of semicolon after the macro!)
RGB_MATRIX_EFFECT(my_cool_effect2)

// Step 2.
// Define effects inside the `RGB_MATRIX_CUSTOM_EFFECT_IMPLS` ifdef block
#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static void my_cool_effect2_complex_init(effect_params_t* params) {
    //
}

static bool my_cool_effect2_complex_run(effect_params_t* params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    uint8_t saturation = 255;
    uint8_t brightness = rgb_matrix_get_val(); // Get the current brightness level

    // Use g_rgb_timer to keep both halves in sync
    uint32_t timer_val = g_rgb_timer;
    // uint16_t hue = (timer_val / 100) % 256; // Adjust the division factor to change speed

    for (uint8_t i = led_min; i < led_max; i++) {
        uint16_t hue = ((timer_val / 100) + (i * 50)) % 256;
        // uint16_t n = hue + (i * 50);
        // if (n > 255) {
        //     n = n - 255;
        // }
        // rgb_matrix_set_color(i, 0xff, some_global_state++, 0xff);
        RGB rgb = hsv_to_rgb((HSV){ .h = hue, .s = saturation, .v = brightness });
        rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}
static bool my_cool_effect2(effect_params_t* params) {
    if (params->init) my_cool_effect2_complex_init(params);
    return my_cool_effect2_complex_run(params);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
