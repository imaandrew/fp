#include "menu.h"

enum tricks {
    JR_SKIP,
    BLACK_TOAD_SKIP,
    RETRIGGER_PROLOGUE,
    OOT_ACE,
    STAIRCASE_SKIP,
    PIE_JUMPS,
    LOG_SKIP,
    EARLY_SEED,
    BUZZAR_SKIP,
    SLOW_GO_EARLY,
    CH2_CARD_LZS,
    RECORD_SKIP,
    BOW_SKIP,
    STANLEY_SAVE,
    YAKKEY_TRICK_SHOT,
    QUICK_ESCAPE,
    CH3_CARD_LZS,
    EARLY_TRAIN,
    EARLY_WHALE_FAST_MUSIC,
    FRYING_PAN_WALL_CLIP,
    CH4_CARD_LZS,
    BHS,
    EARLY_WHALE_SLOW_MUSIC,
    RAPH_SKIP,
    PIRANHA_FIRST_STRIKE,
    LAVA_PLATFORM_CYCLE,
    LAVA_PUZZLE_SKIP,
    FLARAKARRY,
    LAVA_PIRANHA_SKIP,
    CH5_CARD_LZS,
    EARLY_LAKI,
    YELLOW_BERRY_SKIP,
    PEACH_WARP,
    CH6_CARD_LZS,
    CLIPPY_BOOTS,
    MURDER_SOLVED_EARLY,
    SUSHIE_GLITCH,
    ICE_STAIRCASE_SKIP,
    MIRROR_CLIP,
    KOOPER_PUZZLE_SKIP,
    FAST_BASEMENT,
    BASEMENT_SKIP,
    BLIND_BASEMENT,
    FAST_FLOOD_ROOM,
    CANNONLESS
};

enum partners {
    NONE,
    GOOMBARIO,
    KOOPER,
    BOMBETTE,
    PARAKARRY,
    GOOMPA,
    WATT,
    SUSHIE,
    LAKILESTER,
    BOW,
    GOOMBARIA,
    TWINK
};

void create_tricks_menu(struct menu *menu);
void load_trick(int8_t trick);