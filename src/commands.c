#include "commands.h"
#include "fp.h"
#include "input.h"
#include "timer.h"
#include "watchlist.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

struct Command fpCommands[COMMAND_MAX] = {
    {"show/hide menu",   COMMAND_PRESS_ONCE, 0, NULL                     },
    {"return from menu", COMMAND_PRESS_ONCE, 0, NULL                     },
    {"levitate",         COMMAND_HOLD,       0, commandLevitateProc      },
    {"turbo",            COMMAND_PRESS_ONCE, 0, commandTurboProc         },
    {"save position",    COMMAND_PRESS_ONCE, 0, commandSavePosProc       },
    {"load position",    COMMAND_PRESS_ONCE, 0, commandLoadPosProc       },
    {"lzs",              COMMAND_PRESS_ONCE, 0, commandLzsProc           },
    {"reload map",       COMMAND_PRESS_ONCE, 0, commandReloadProc        },
    {"reload last warp", COMMAND_PRESS_ONCE, 0, commandReloadLastWarpProc},
    {"toggle watches",   COMMAND_PRESS_ONCE, 0, commandToggleWatchesProc },
    {"reimport save",    COMMAND_PRESS_ONCE, 0, commandImportSaveProc    },
    {"save game",        COMMAND_PRESS_ONCE, 0, commandSaveGameProc      },
    {"load game",        COMMAND_PRESS_ONCE, 0, commandLoadGameProc      },
    {"start/stop timer", COMMAND_PRESS_ONCE, 0, commandStartTimerProc    },
    {"reset timer",      COMMAND_PRESS_ONCE, 0, commandResetTimerProc    },
    {"show/hide timer",  COMMAND_PRESS_ONCE, 0, commandShowHideTimerProc },
    {"break free",       COMMAND_PRESS_ONCE, 0, commandBreakFreeProc     },
};

void showMenu(void) {
    menuSignalEnter(fp.mainMenu, MENU_SWITCH_SHOW);
    fp.menuActive = TRUE;
    inputReserve(BUTTON_D_UP | BUTTON_D_DOWN | BUTTON_D_LEFT | BUTTON_D_RIGHT | BUTTON_L);
}

void hideMenu(void) {
    menuSignalLeave(fp.mainMenu, MENU_SWITCH_HIDE);
    fp.menuActive = FALSE;
    inputFree(BUTTON_D_UP | BUTTON_D_DOWN | BUTTON_D_LEFT | BUTTON_D_RIGHT | BUTTON_L);
}

void fpLog(const char *fmt, ...) {
    struct LogEntry *ent = &fp.log[SETTINGS_LOG_MAX - 1];
    if (ent->msg) {
        free(ent->msg);
    }
    for (s32 i = SETTINGS_LOG_MAX - 1; i > 0; --i) {
        fp.log[i] = fp.log[i - 1];
    }

    va_list va;
    va_start(va, fmt);
    s32 l = vsnprintf(NULL, 0, fmt, va);
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

void fpSetInputMask(u16 pad, u8 x, u8 y) {
    fp.inputMask.buttons = pad;
    fp.inputMask.xCardinal = x;
    fp.inputMask.yCardinal = y;
}

bool fpWarp(enum Areas area, u16 map, u16 entrance) {
    pm_func_800554A4(0); // stop koopa village radio from playing
    pm_func_800554A4(1);
    pm_func_800554A4(2);
    pm_func_800554A4(3);
    pm_bgmSetSong(1, -1, 0, 0, 8); // clear secondary songs
    pm_sfxStopSound(0xA5);         // clear Goomba King's Castle rumble
    pm_sfxStopSound(0x19C);        // clear upward vine sound
    pm_sfxStopSound(0x19D);        // clear downward vine sound
    pm_disable_player_input();
    pm_gGameStatus.loadingZoneTangent = 0;
    pm_gGameStatus.areaID = area;
    pm_gGameStatus.mapID = map;
    pm_gGameStatus.entryID = entrance;

    PRINTF("***** WARP TRIGGERED *****\n");
    if (pm_gGameStatus.isBattle || pm_popupMenuVar == 1) {
        // prevent crashes from warping when in battle menus or with partner/item menu open
        pm_clearWindows();
    }

    if (pm_gameMode == 0xA) { // paused
        fp.warpDelay = 5;
        pm_setGameMode(0xB);
    } else {
        fp.warpDelay = 0;
    }

    // set the global curtain to default+off state
    // this is mainly to prevent a crash when warping from "card obtained"
    pm_setCurtainScaleGoal(2.0f);
    pm_setCurtainDrawCallback(NULL);
    pm_setCurtainFadeGoal(0.0f);

    fp.warp = TRUE;

    return TRUE;
}

static void setFlag(u32 *flags, s32 flagIndex, bool value) {
    s32 wordIndex = flagIndex / 32;
    s32 bit = flagIndex % 32;
    if (value) {
        flags[wordIndex] |= (1 << bit);
    } else {
        flags[wordIndex] &= ~(1 << bit);
    }
}

void fpSetGlobalFlag(s32 flagIndex, bool value) {
    setFlag(pm_gCurrentSaveFile.globalFlags, flagIndex, value);
}

void fpSetAreaFlag(s32 flagIndex, bool value) {
    setFlag(pm_gCurrentSaveFile.areaFlags, flagIndex, value);
}

void fpSetGlobalByte(s32 byteIndex, s8 value) {
    pm_gCurrentSaveFile.globalBytes[byteIndex] = value;
}

void commandLevitateProc(void) {
    // TODO: figure out how to get some version of this working for peach
    if (!(pm_gGameStatus.peachFlags & (1 << 0))) {
        pm_gPlayerStatus.flags |= 1 << 1;
        pm_gPlayerStatus.flags &= ~(1 << 2);
        pm_gPlayerStatus.ySpeed = 11;
        pm_gPlayerStatus.framesInAir = 1;
        // these are the default starting values for when you fall
        pm_gPlayerStatus.yAcceleration = -0.350080013275f;
        pm_gPlayerStatus.yJerk = -0.182262003422f;
        pm_gPlayerStatus.ySnap = 0.0115200001746f;
    }
}

void commandTurboProc(void) {
    if (fp.turbo) {
        fp.turbo = FALSE;
        fpLog("turbo disabled");
    } else {
        fp.turbo = TRUE;
        fpLog("turbo enabled");
    }
}

void commandSavePosProc(void) {
    uint32_t x[] = {0x3C088010, 0x3508F604, 0x24092A26, 0xA5090000, 0x3C08800D, 0x3508BD70, 0x3C09800B, 0x35291010, 0xAD090000, 0x24090121, 0xA5090004, 0x3C08800B, 0x35081010, 0x25090570, 0xA1000000, 0x25080001, 0x1509FFFD, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x3C08800B, 0x35081540, 0x00000000, 0x3C090080, 0x35290DBD, 0x00000000, 0xAD090000, 0x34090080, 0xA1090004, 0x340900FD, 0xA1090005, 0x3C08800D, 0x3508BD80, 0x3C093C08, 0x35298010, 0xAD090000, 0x3C093508, 0x3529F45D, 0xAD090004, 0x3C093C09, 0x3529800B, 0xAD090008, 0x3C093529, 0x35291D56, 0xAD09000C, 0x3C09812A, 0xAD090010, 0x3C09000A, 0x35290FC3, 0xAD090014, 0x3C090141, 0x35295026, 0xAD090018, 0x3C090141, 0x35295023, 0xAD09001C, 0x3C09A10A, 0x35290003, 0xAD090020, 0x3C09812A, 0x35290001, 0xAD090024, 0x3C09000A, 0x35290FC3, 0xAD090028, 0x3C090141, 0x35295026, 0xAD09002C, 0x3C090141, 0x35295023, 0xAD090030, 0x3C09A10A, 0xAD090034, 0x3C093C1F, 0x3529800F, 0xAD090038, 0x3C0937FF, 0x35293E94, 0xAD09003C, 0x3C0903E0, 0x35290008, 0xAD090040, 0x0C0B7054, 0x00000000, 0x03E00008, 0x00000000};
    memcpy((void*)0x807c0000, &x, sizeof(x));
    fpLog("copied payload");
    func();
}

void commandLoadPosProc(void) {
    pm_gPlayerStatus.position.x = fp.savedPos.x;
    pm_gPlayerStatus.position.y = fp.savedPos.y;
    pm_gPlayerStatus.position.z = fp.savedPos.z;
    pm_gPlayerStatus.currentYaw = fp.savedFacingAngle;
    pm_gPlayerStatus.targetYaw = fp.savedMovementAngle;
    fpLog("position loaded");
}

void commandLzsProc(void) {
    if (pm_timeFreezeMode == 0) {
        pm_timeFreezeMode = 1;
        fpLog("easy lzs enabled");
    } else if (pm_timeFreezeMode == 1) {
        pm_timeFreezeMode = 0;
        fpLog("easy lzs disabled");
    }
}

void commandReloadProc(void) {
    fpWarp(pm_gGameStatus.areaID, pm_gGameStatus.mapID, pm_gGameStatus.entryID);
}

void commandReloadLastWarpProc(void) {
    if (fp.savedArea != 0x1c) {
        fpWarp(fp.savedArea, fp.savedMap, fp.savedEntrance);
    }
}

void commandToggleWatchesProc(void) {
    settings->bits.watchesVisible = !settings->bits.watchesVisible;
    if (settings->bits.watchesVisible) {
        watchlistShow(fp.menuWatchlist);
    } else {
        watchlistHide(fp.menuWatchlist);
    }
}

void commandImportSaveProc(void) {
    if (fp.lastImportedSavePath) {
        fpImportFile(fp.lastImportedSavePath, NULL);
    }
}

void commandSaveGameProc(void) {
    pm_saveGame();
    pm_playSfx(0x10);
    fpLog("saved to slot %d", pm_gGameStatus.saveSlot);
}

void commandStartTimerProc(void) {
    timerStart();
}

void commandResetTimerProc(void) {
    timerReset();
}

void commandShowHideTimerProc(void) {
    settings->bits.timerShow = !settings->bits.timerShow;
}

void commandLoadGameProc(void) {
    pm_SaveData *file = malloc(sizeof(*file));
    pm_fioFetchSavedFileInfo();
    pm_fioReadFlash(pm_logicalSaveInfo[pm_gGameStatus.saveSlot][0], file, sizeof(*file));
    if (pm_fioValidateFileChecksum(file)) {
        pm_gCurrentSaveFile = *file;
        pm_fioDeserializeState();
        fpWarp(file->areaID, file->mapID, file->entryID);
        fpLog("loaded from slot %d", pm_gGameStatus.saveSlot);
    } else {
        fpLog("no file in slot %d", pm_gGameStatus.saveSlot);
    }
    free(file);
}

void commandBreakFreeProc(void) {
    s32 thirdByteMask = 0xFFFF00FF;
    s32 checkMask = 0x0000FF00;

    if ((pm_gPlayerStatus.flags & checkMask) == 0x2000) {
        pm_gPlayerStatus.flags &= thirdByteMask;
    }
    fpLog("broke free");
}
