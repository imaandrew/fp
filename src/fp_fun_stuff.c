#include "menu.h"
#include "pm64.h"
#include <stdlib.h>
#include "fp.h"
#include "watchlist.h"

static s32 checkbox_mod_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    u8 *p = data;
    if (reason == MENU_CALLBACK_SWITCH_ON) {
        *p = 1;
    } else if (reason == MENU_CALLBACK_SWITCH_OFF) {
        *p = 0;
    } else if (reason == MENU_CALLBACK_THINK) {
        menu_checkbox_set(item, *p);
    }
    return 0;
}

static s32 dump_rom_draw_proc(struct menu_item *item, struct menu_draw_params *draw_params) {
    gfx_mode_set(GFX_MODE_COLOR, GPACK_RGB24A8(draw_params->color, draw_params->alpha));
    struct gfx_font *font = draw_params->font;
    s32 chHeight = menu_get_cell_height(item->owner, 1);
    s32 chWidth = menu_get_cell_width(item->owner, 1);
    s32 x = draw_params->x;
    s32 y = draw_params->y;

    gfx_printf(font, x, y + chHeight * 1, "rom address: ");
    gfx_printf(font, x + chWidth * 20, y + chHeight * 1, "%#010x", fp.rom_addr > 0 ? fp.rom_addr - 4 : 0);
    gfx_printf(font, x + chWidth * 0, y + chHeight * 2, "rom value: ");
    gfx_printf(font, x + chWidth * 20, y + chHeight * 2, "%#010x", fp.rom_value);


    return 1;
}

static void crash_game(struct menu_item *item, void *data) {
    __asm__(".set noreorder;"
            "LA $ra, 0x13542678;"
            "JR $ra;"
            "NOP;");
}

struct menu *create_fun_menu(void) {
    static struct menu menu;
    static struct menu romMenu;

    /* initialize menu */
    menu_init(&menu, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&romMenu, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);

    /*build menu*/
    s32 y_value = 0;
    menu.selector = menu_add_submenu(&menu, 0, y_value++, NULL, "return");
    menu_add_static(&menu, 0, y_value, "heap randomizer", 0xC0C0C0);
    menu_add_checkbox(&menu, 20, y_value++, checkbox_mod_proc, &fp.heap_rando);
    menu_add_static(&menu, 0, y_value, "lz randomizer", 0xC0C0C0);
    menu_add_checkbox(&menu, 20, y_value++, checkbox_mod_proc, &fp.lz_rando);
    menu_add_static(&menu, 0, y_value, "mechanical noises", 0xC0C0C0);
    menu_add_checkbox_enabled(&menu, 20, y_value++, checkbox_mod_proc, &fp.moan_mode);
    menu_add_static(&menu, 0, y_value, "better camera", 0xC0C0C0);
    menu_add_checkbox(&menu, 20, y_value++, checkbox_mod_proc, &fp.free_cam);
    menu_add_submenu(&menu, 0, y_value++, &romMenu, "rom sharing tool");
    romMenu.selector = menu_add_submenu(&romMenu, 0, 0, NULL, "return");
    menu_add_static(&romMenu, 0, 1, "enabled", 0xC0C0C0);
    menu_add_checkbox(&romMenu, 8, 1, checkbox_mod_proc, &fp.dump_rom);
    menu_add_static_custom(&romMenu, 0, 2, dump_rom_draw_proc, NULL, 0xFFFFFF);
    menu_add_button(&menu, 0, y_value++, "close game", crash_game, NULL);
    return &menu;
}