#include "gfx.h"
#include "menu.h"
#include "resource.h"
#include <stdlib.h>

struct item_data {
    menu_generic_callback callback_proc;
    void *callback_data;
    bool state;
    s32 anim_state;
};

static s32 enter_proc(struct menu_item *item, enum menu_switch_reason reason) {
    struct item_data *data = item->data;
    data->anim_state = 0;
    return 0;
}

static s32 think_proc(struct menu_item *item) {
    struct item_data *data = item->data;
    if (data->callback_proc) {
        return data->callback_proc(item, MENU_CALLBACK_THINK, data->callback_data);
    }
    return 0;
}

static s32 draw_proc(struct menu_item *item, struct menu_draw_params *draw_params) {
    struct item_data *data = item->data;
    gfx_mode_set(GFX_MODE_COLOR, GPACK_RGB24A8(draw_params->color, draw_params->alpha));
    static struct gfx_texture *texture = NULL;
    if (!texture) {
        texture = resource_load_grc_texture("checkbox");
    }
    s32 cw = menu_get_cell_width(item->owner, TRUE);
    struct gfx_sprite sprite = {
        texture,
        data->anim_state == 0 ? 0 : 1,
        0,
        draw_params->x + (cw - texture->tile_width) / 2,
        draw_params->y - (gfx_font_xheight(draw_params->font) + texture->tile_height + 1) / 2,
        1.f,
        1.f,
    };
    gfx_sprite_draw(&sprite);
    if ((data->anim_state > 0) != data->state) {
        gfx_mode_set(GFX_MODE_COLOR, GPACK_RGBA8888(0xFF, 0xFF, 0xFF, draw_params->alpha));
        sprite.texture_tile = 2;
        gfx_sprite_draw(&sprite);
    }
    if (data->anim_state > 0) {
        data->anim_state = (data->anim_state + 1) % 3;
    }
    return 1;
}

static s32 activate_proc(struct menu_item *item) {
    struct item_data *data = item->data;
    if (!data->callback_proc ||
        !data->callback_proc(item, data->state ? MENU_CALLBACK_SWITCH_OFF : MENU_CALLBACK_SWITCH_ON,
                             data->callback_data)) {
        data->state = !data->state;
        data->anim_state = 1;
        if (data->callback_proc) {
            data->callback_proc(item, MENU_CALLBACK_CHANGED, data->callback_data);
        }
    }
    return 1;
}

struct menu_item *menu_add_checkbox(struct menu *menu, s32 x, s32 y, menu_generic_callback callback_proc,
                                    void *callback_data) {
    struct item_data *data = malloc(sizeof(*data));
    data->callback_proc = callback_proc;
    data->callback_data = callback_data;
    data->anim_state = 0;
    struct menu_item *item = menu_item_add(menu, x, y, NULL, 0xFFFFFF);
    item->data = data;
    item->enter_proc = enter_proc;
    item->think_proc = think_proc;
    item->draw_proc = draw_proc;
    item->activate_proc = activate_proc;
    return item;
}

bool menu_checkbox_get(struct menu_item *item) {
    struct item_data *data = item->data;
    return data->state;
}

void menu_checkbox_set(struct menu_item *item, bool state) {
    struct item_data *data = item->data;
    data->state = state;
}
