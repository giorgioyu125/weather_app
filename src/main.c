#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#define FENSTER_HEADER
#include "fenster.h"
#include "microui.h"
#include "renderer.h"

#define WIDTH  800
#define HEIGHT 600

uint32_t buf[WIDTH * HEIGHT];

static int mouse_button_map(int fenster_btn) {
    switch (fenster_btn) {
        case 1: return MU_MOUSE_LEFT;
        case 2: return MU_MOUSE_RIGHT;
        case 3: return MU_MOUSE_MIDDLE;
    }
    return 0;
}

int main(void) {
    struct fenster f = {
        .title = "Weather App",
        .width = WIDTH,
        .height = HEIGHT,
        .buf = buf
    };
    fenster_open(&f);

    mu_Context *ctx = malloc(sizeof(mu_Context));
    mu_init(ctx);
    ctx->text_width = r_text_width;
    ctx->text_height = r_text_height;

    r_init(&f);

    int prev_mouse_x = 0;
    int prev_mouse_y = 0;
    int prev_mouse_down = 0;

    while (fenster_loop(&f) == 0) {
        int64_t start_time = fenster_time();
        if (f.keys[27]) break;

        if (f.x != prev_mouse_x || f.y != prev_mouse_y) {
            mu_input_mousemove(ctx, f.x, f.y);
            prev_mouse_x = f.x;
            prev_mouse_y = f.y;
        }

        if (f.mouse != prev_mouse_down) {
            if (f.mouse && !prev_mouse_down) {
                mu_input_mousedown(ctx, f.x, f.y, mouse_button_map(f.mouse));
            } else if (!f.mouse && prev_mouse_down) {
                mu_input_mouseup(ctx, f.x, f.y, mouse_button_map(prev_mouse_down));
            }
            prev_mouse_down = f.mouse;
        }

        mu_begin(ctx);

        if (mu_begin_window(ctx, "Demo Window", mu_rect(40, 40, 300, 200))) {
            mu_layout_row(ctx, 1, (int[]){-1}, 30);

            if (mu_button(ctx, "Click Me!")) {
                // Button effect
            }

            mu_label(ctx, "Hello from microui!");

            mu_layout_row(ctx, 1, (int[]){-1}, 50);
            mu_Rect rect = mu_layout_next(ctx);
            mu_draw_rect(ctx, rect, mu_color(0, 150, 200, 255));
            mu_draw_box(ctx, rect, mu_color(50, 80, 120, 255));

            mu_end_window(ctx);
        }

        mu_end(ctx);

        r_clear(0x2A2A);
        r_render(ctx);

        int64_t work_time = fenster_time() - start_time;
        if (work_time < TARGET_FRAME_TIME) {
            fenster_sleep(TARGET_FRAME_TIME - work_time);
        }
    }

    free(ctx);
    fenster_close(&f);

    return EXIT_SUCCESS;
}
