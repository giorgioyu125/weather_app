#include "renderer.h"
#include <string.h>

static struct fenster *fen;

#define FONT_WIDTH  8
#define FONT_HEIGHT 8

void r_init(struct fenster *f) {
    fen = f;
}

void r_clear(uint32_t color) {
    for (int i = 0; i < fen->width * fen->height; i++) {
        fen->buf[i] = color;
    }
}

static void r_set_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < fen->width && y >= 0 && y < fen->height) {
        fen->buf[y * fen->width + x] = color;
    }
}

static uint32_t mu_color_to_uint32(mu_Color c) {
    return (c.r << 16) | (c.g << 8) | c.b;
}

void r_rect(mu_Rect rect, mu_Color color) {
    uint32_t col = mu_color_to_uint32(color);
    for (int y = rect.y; y < rect.y + rect.h; y++) {
        for (int x = rect.x; x < rect.x + rect.w; x++) {
            r_set_pixel(x, y, col);
        }
    }
}

void r_text(const char *text, int x, int y, mu_Color color) {
    uint32_t col = mu_color_to_uint32(color);
    
    while (*text) {
        unsigned char c = *text;
        if (c >= 32 && c < 128) {
            const unsigned char *glyph = font_8x8[c - 32];
            for (int row = 0; row < FONT_HEIGHT; row++) {
                for (int col_bit = 0; col_bit < FONT_WIDTH; col_bit++) {
                    if (glyph[row] & (0x80 >> col_bit)) {
                        r_set_pixel(x + col_bit, y + row, col);
                    }
                }
            }
        }
        x += FONT_WIDTH;
        text++;
    }
}

int r_text_width(mu_Font font, const char *text, int len) {
    (void)font;
    if (len == -1) {
        len = strlen(text);
    }
    return len * FONT_WIDTH;
}

int r_text_height(mu_Font font) {
    (void)font;
    return FONT_HEIGHT;
}

void r_render(mu_Context *ctx) {
    mu_Command *cmd = NULL;
    while (mu_next_command(ctx, &cmd)) {
        switch (cmd->type) {
            case MU_COMMAND_TEXT:
                r_text(cmd->text.str, cmd->text.pos.x, cmd->text.pos.y, cmd->text.color);
                break;
            case MU_COMMAND_RECT:
                r_rect(cmd->rect.rect, cmd->rect.color);
                break;
            case MU_COMMAND_ICON:
                r_rect(cmd->icon.rect, cmd->icon.color);
                break;
            case MU_COMMAND_CLIP:
            default:
                break;

        }
    }
}
