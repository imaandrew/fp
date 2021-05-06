#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include "commands.h"
#include "input.h"
#include "fp.h"
#include "settings.h"
#include "tricks.h"

struct command fp_commands[COMMAND_MAX] = {
    {"show/hide menu",   COMMAND_PRESS_ONCE,  0,   NULL},
    {"return from menu", COMMAND_PRESS_ONCE,  0,   NULL},
    {"levitate",         COMMAND_HOLD,        0,   command_levitate_proc},
    {"turbo",            COMMAND_PRESS_ONCE,  0,   command_turbo_proc},
    {"save position",    COMMAND_PRESS_ONCE,  0,   command_save_pos_proc},
    {"load position",    COMMAND_PRESS_ONCE,  0,   command_load_pos_proc},
    {"lzs",              COMMAND_PRESS_ONCE,  0,   command_lzs_proc},
    {"reload room",      COMMAND_PRESS_ONCE,  0,   command_reload_proc},
    {"show coordinates", COMMAND_PRESS_ONCE,  0,   command_coords_proc},
    {"load trick",       COMMAND_PRESS_ONCE,  0,   command_trick_proc},
    {"save game",        COMMAND_PRESS_ONCE,  0,   command_save_game_proc},
    {"load game",        COMMAND_PRESS_ONCE,  0,   command_load_game_proc},
    {"start timer",      COMMAND_PRESS_ONCE,  0,   command_start_timer_proc},
    {"reset timer",      COMMAND_PRESS_ONCE,  0,   command_reset_timer_proc}
};

void show_menu() {
    menu_signal_enter(fp.main_menu, MENU_SWITCH_SHOW);
    fp.menu_active = 1;
    input_reserve(BUTTON_D_UP | BUTTON_D_DOWN | BUTTON_D_LEFT | BUTTON_D_RIGHT | BUTTON_L);
    input_reservation_set(1);
}

void hide_menu() {
    menu_signal_leave(fp.main_menu, MENU_SWITCH_HIDE);
    fp.menu_active = 0;
    input_free(BUTTON_D_UP | BUTTON_D_DOWN | BUTTON_D_LEFT | BUTTON_D_RIGHT | BUTTON_L);
    input_reservation_set(0);
}

void add_log(const char *fmt, ...) {
    struct log_entry *ent = &fp.log[SETTINGS_LOG_MAX - 1];
    if (ent->msg) {
        free(ent->msg);
    }
    for (int i = SETTINGS_LOG_MAX - 1; i > 0; --i) {
        fp.log[i] = fp.log[i - 1];
    }
      
    va_list va;
    va_start(va, fmt);
    int l = vsnprintf(NULL, 0, fmt, va);
    va_end(va);

    ent = &fp.log[0];
    ent->msg = malloc(l + 1);
    if (!ent->msg) {
        return;
    }
    va_start(va, fmt);
    vsprintf(ent->msg, fmt, va);
    va_end(va);
    ent->age = 0;
}

void command_levitate_proc() {
    if (pm_status.peach_flags == 0) {
        pm_player.flags |= 3;
        pm_player.y_speed = 11;
        pm_player.y_snap = -0.75;
        pm_player.frames_in_air = 1;
    }
}

void command_turbo_proc() {
    if (fp.turbo) {
        fp.turbo = 0;
        add_log("turbo disabled");
    }
    else {
        fp.turbo = 1;
        add_log("turbo enabled");
    }
}

void command_save_pos_proc() {
    fp.saved_x = pm_player.position.x;
    fp.saved_y = pm_player.position.y;
    fp.saved_z = pm_player.position.z;
    fp.saved_facing_angle = pm_player.facing_angle;
    fp.saved_movement_angle = pm_player.movement_angle;
    add_log("position saved");
}

void command_load_pos_proc() {
    pm_player.position.x = fp.saved_x;
    pm_player.position.y = fp.saved_y;
    pm_player.position.z = fp.saved_z;
    pm_player.facing_angle = fp.saved_facing_angle;
    pm_player.movement_angle = fp.saved_movement_angle;
    add_log("position loaded");
}

void command_lzs_proc() {
    if(pm_unk1.saveblock_freeze == 0) {
        pm_unk1.saveblock_freeze = 1;
        add_log("lzs enabled");
    } else if(pm_unk1.saveblock_freeze == 1) {
        pm_unk1.saveblock_freeze = 0;
        add_log("lzs disabled");
    }
}

void command_reload_proc() {
    pm_status.loading_zone_tangent = 0;
    pm_unk2.room_change_state = 1;
    uint32_t val = 0x80035DFC;
    pm_warp.room_change_ptr = val;
}

void command_coords_proc() {
    fp.coord_active = !fp.coord_active;
}

void command_trick_proc() {
    load_trick(fp.saved_trick);
}

void command_save_game_proc() {
    pm_SaveGame();
    pm_PlaySfx(0x10);
    add_log("saved to slot %d", pm_status.save_slot);
}

void command_start_timer_proc() {
    if (fp.timer.state == 0) {
        fp.timer.state = 1;
        add_log("timer set to start");
    }
    else if (fp.timer.state == 3) {
        fp.timer.state = 1;
        fp.timer.cutscene_count = 0;
        add_log("timer set to start");
    }
}

void command_reset_timer_proc() {
    fp.timer.state = 0;
    fp.timer.cutscene_count = 0;
    add_log("timer reset");
}

void command_load_game_proc() {
    pm_LoadGame(pm_status.save_slot);
    pm_status.loading_zone_tangent = 0;
    pm_unk2.room_change_state = 1;
    uint32_t val = 0x80035DFC;
    pm_warp.room_change_ptr = val;
    add_log("loaded from slot %d", pm_status.save_slot);
}