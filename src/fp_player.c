#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "settings.h"
#include "gfx.h"
#include "resource.h"

const char *PARTNERS = "none\0""goombario\0""kooper\0""bombette\0"
"parakarry\0""goompa\0""watt\0""sushie\0""lakilester\0""bow\0""goombaria\0"
"twink\0";
const char *RANK = "none\0""super\0""ultra\0";
const char *SPELL_TYPE = "none\0""+3 ATK\0""+3 DEF\0""EXP x2\0""Coins x2\0";
const char *ITEM_LIST = "-\0""Jump\0""Spin Jump\0""Tornado Jump\0""Hammer\0""Super Hammer\0""Ultra Hammer\0"
"Lucky Star\0""Map\0""Big Map\0""1st Degree Card\0""2nd Degree Card\0""3rd Degree Card\0""4th Degree Card\0""Diploma\0"
"Ultra Stone\0""Fortress Key\0""Ruins Key\0""Pulse Stone\0""Castle Key 1\0""Palace Key\0""Lunar Stone\0"
"Pyramid Stone\0""Diamond Stone\0""Golden Vase\0""Koopers Shell\0""Castle Key 2\0""Forest Pass\0""Weight\0"
"Boos Portrait\0""Crystal Berry\0""Mystical Key\0""Storeroom Key\0""Toy Train\0""Record\0""Frying Pan\0""Dictionary\0"
"Mystery Note\0""Suspicious Note\0""Crystal Ball\0""Screwdriver\0""Cookbook\0""Jade Raven\0""Magical Seed 1\0"
"Magical Seed 2\0""Magical Seed 3\0""Magical Seed 4\0""Toad Doll\0""Calculator\0""Bucket\0""Scarf\0""Red Key\0"
"Blue Key\0""a\0""Letter 01\0""Letter 02\0""Letter 03\0""Letter 04\0""Letter 05\0""Letter 06\0""Letter 07\0"
"Letter 08\0""Letter 09\0""a\0""Letter 10\0""Letter 11\0""a\0""a\0""a\0""Letter 12\0""Letter 13\0""Letter 14\0"
"Letter 15\0""Letter 16\0""Letter 17\0""Letter 18\0""Letter 19\0""a\0""Letter 20\0""Letter 21\0""Letter 22\0"
"Letter 23\0""Letter 24\0""Artifact\0""Letter 25\0""a\0""Dolly\0""Water Stone\0""Magical Bean\0""Fertile Soil\0"
"Miracle Water\0""Volcano Vase\0""Tape\0""Sugar\0""Salt\0""Egg 1\0""Cream\0""Strawberry\0""Butter\0""Cleanser\0"
"Water\0""Flour\0""Milk\0""Lyrics\0""Melody\0""Mailbag\0""Castle Key 3\0""Odd Key\0""Star Stone\0""Sneaky Parasol\0"
"Koopa Legends\0""Autograph 1\0""Empty Wallet\0""Autograph 2\0""Koopa Shell\0""Old Photo\0""Glasses\0""Photograph\0"
"Package\0""Red Jar\0""Castle Key 4\0""Warehouse Key\0""Prison Key 1\0""Silver Credit\0""Gold Credit\0""Prison Key 2\0"
"Prison Key 3\0""Prison Key 4\0""Fire Flower\0""Snowman Doll\0""Thunder Rage\0""Shooting Star\0""Thunder Bolt\0"
"Pebble\0""Dusty Hammer\0""Insecticide Herb\0""Stone Cap\0""Tasty Tonic\0""Mushroom\0""Volt Shroom\0""Super Shroom\0"
"Dried Shroom\0""Ultra Shroom\0""Sleepy Sheep\0""POW Block\0""Hustle Drink\0""Stop Watch\0""Whackas Bump\0""Apple\0"
"Life Shroom\0""Mystery\0""Repel Gel\0""Fright Jar\0""Please Come Back\0""Dizzy Dial\0""Super Soda\0""Lemon\0""Lime\0"
"Blue Berry\0""Red Berry\0""Yellow Berry\0""Bubble Berry\0""Jammin Jelly\0""Maple Syrup\0""Honey Syrup\0""Goomnut\0"
"Koopa Leaf\0""Dried Pasta\0""Dried Fruit\0""Strange Leaf\0""Cake Mix\0""Egg 2\0""Coconut\0""Melon\0""Stinky Herb\0"
"Iced Potato\0""Spicy Soup\0""Apple Pie\0""Honey Ultra\0""Maple Ultra\0""Jelly Ultra\0""Koopasta\0""Fried Shroom\0"
"Shroom Cake\0""Shroom Steak\0""Hot Shroom\0""Sweet Shroom\0""Yummy Meal\0""Healthy Juice\0""Bland Meal\0"
"Deluxe Feast\0""Special Shake\0""Big Cookie\0""Cake\0""Mistake\0""Koopa Tea\0""Honey Super\0""Maple Super\0"
"Jelly Super\0""Spaghetti\0""Egg Missile\0""Fried Egg\0""Honey Shroom\0""Honey Candy\0""Electro Pop\0""Fire Pop\0"
"Lime Candy\0""Coco Pop\0""Lemon Candy\0""Jelly Pop\0""Strange Cake\0""Kooky Cookie\0""Frozen Fries\0""Potato Salad\0"
"Nutty Cake\0""Maple Shroom\0""Boiled Egg\0""Yoshi Cookie\0""Jelly Shroom 1\0""Jelly Shroom 2\0""Jelly Shroom 3\0"
"Jelly Shroom 4\0""Jelly Shroom 5\0""Jelly Shroom 6\0""Spin Smash\0""Multibounce\0""Power Plus A\0""Dodge Master\0"
"Power Bounce\0""Spike Shield\0""First Attack\0""HP Plus A\0""Quake Hammer\0""Double Dip\0""Mystery Scroll\0"
"Sleep Stomp\0""Fire Shield\0""Quick Change\0""D Down Pound\0""Dizzy Stomp\0""Smash Charge 0\0""Pretty Lucky\0"
"Feeling Fine\0""Attack FXA\0""All or Nothing\0""HP Drain\0""Jump Charge 0\0""Slow Go\0""FP Plus A\0""Mega Rush\0"
"Ice Power\0""Defend Plus A\0""Pay Off\0""Money Money\0""Chill Out\0""Happy Heart A\0""Zap Tap\0""Berserker\0"
"Right On\0""Runaway Pay\0""Refund\0""Flower Saver A\0""Triple Dip\0""Hammer Throw\0""Mega Quake\0""Smash Charge\0"
"Jump Charge\0""S Smash Chg\0""S Jump Chg\0""Power Rush\0""Auto Jump\0""Auto Smash\0""Crazy Heart\0""Last Stand\0"
"Close Call\0""P Up D Down\0""Lucky Day\0""Mega HP Drain\0""P Down D Up\0""Power Quake\0""Auto Multibounce\0"
"Flower Fanatic\0""Heart Finder\0""Flower Finder\0""Spin Attack\0""Dizzy Attack\0""I Spy\0""Speedy Spin\0"
"Bump Attack\0""Power Jump\0""Super Jump\0""Mega Jump\0""Power Smash 1\0""Super Smash\0""Mega Smash\0""Power Smash 2\0"
"Power Smash 3\0""Deep Focus 1\0""Super Focus\0""Shrink Smash\0""Shell Crack\0""Kaiden\0""D Down Jump\0""Shrink Stomp\0"
"Damage Dodge A\0""Earthquake Jump\0""Deep Focus 2\0""Deep Focus 3\0""HP Plus B\0""FP Plus B\0""Happy Heart B\0"
"Happy Heart X\0""Flower Saver B\0""Flower Saver X\0""Damage Dodge B\0""Damage Dodge X\0""Power Plus B\0"
"Power Plus X\0""Defend Plus X\0""Defend Plus Y\0""Happy Flower A\0""Happy Flower B\0""Happy Flower X\0""Group Focus\0"
"Peekaboo\0""Attack FXD\0""Attack FXB\0""Attack FXE\0""Attack FXC\0""Attack FXF\0""HP Plus C\0""HP Plus X\0"
"HP Plus Y\0""FP Plus C\0""FP Plus X\0""FP Plus Y\0""Healthy Healthy\0""Attack FXF 2\0""Attack FXF 3\0""Attack FXF 4\0"
"Attack FXF 5\0""Partner Attack\0""Heart\0""Coin\0""Heart Piece\0""Star Point\0""Full Heal\0""Flower\0""Star Piece\0"
"Present\0""Complete Cake\0""Bare Cake\0""Empty Cake Pan\0""Full Cake Pan\0""Empty Mixing Bowl\0""Full Mixing Bowl\0"
"Cake With Icing\0""Cake With Berries\0""Hammer 1 Icon\0""Hammer 2 Icon\0""Hammer 3 Icon\0""Boots 1 Icon\0"
"Boots 2 Icon\0""Boots 3 Icon\0""Items Icon\0";

static int halfword_mod_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    uint16_t *p = data;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_intinput_get(item) != *p)
            menu_intinput_set(item, *p);
    }
    else if (reason == MENU_CALLBACK_CHANGED)
        *p = menu_intinput_get(item);
    return 0;
}

static int byte_mod_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    uint8_t *p = data;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_intinput_get(item) != *p)
            menu_intinput_set(item, *p);
    }
    else if (reason == MENU_CALLBACK_CHANGED)
        *p = menu_intinput_get(item);
    return 0;
}

static int action_commands_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    if (reason == MENU_CALLBACK_SWITCH_ON) {
        pm_player.stats.has_action_command = 1;
    }
    else if (reason == MENU_CALLBACK_SWITCH_OFF) {
        pm_player.stats.has_action_command = 0;
    }
    else if (reason == MENU_CALLBACK_THINK) {
        menu_checkbox_set(item, pm_player.stats.has_action_command);
    }
    return 0;
}

static int max_hp_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_intinput_get(item) != pm_player.stats.max_hp)
            menu_intinput_set(item, pm_player.stats.max_hp);
    }
    else if (reason == MENU_CALLBACK_CHANGED) {
        pm_player.stats.max_hp = menu_intinput_get(item);
        pm_player.stats.menu_max_hp = menu_intinput_get(item);
    }
    return 0;
}

static int max_fp_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_intinput_get(item) != pm_player.stats.max_fp)
            menu_intinput_set(item, pm_player.stats.max_fp);
    }
    else if (reason == MENU_CALLBACK_CHANGED) {
        pm_player.stats.max_fp = menu_intinput_get(item);
        pm_player.stats.menu_max_fp = menu_intinput_get(item);
    }
    return 0;
}

static int boots_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_option_get(item) != pm_player.stats.boots_upgrade)
            menu_option_set(item, pm_player.stats.boots_upgrade);
    }
    else if (reason == MENU_CALLBACK_DEACTIVATE)
        pm_player.stats.boots_upgrade = menu_option_get(item);
    return 0;
}

static int hammer_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    uint8_t menu_hammer = menu_option_get(item) - 1;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_hammer != pm_player.stats.hammer_upgrade)
            menu_option_set(item, (uint8_t)pm_player.stats.hammer_upgrade + 1);
    }
    else if (reason == MENU_CALLBACK_DEACTIVATE)
        pm_player.stats.hammer_upgrade = (uint8_t)menu_option_get(item) - 1;
    return 0;
}

static int active_partner_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_option_get(item) != pm_player.stats.current_partner)
            menu_option_set(item, pm_player.stats.current_partner);
    }
    else if (reason == MENU_CALLBACK_DEACTIVATE)
        pm_player.stats.current_partner = menu_option_get(item);
    return 0;
}

static int in_party_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    partner_t *partner = (partner_t *)data;
    if (reason == MENU_CALLBACK_SWITCH_ON) {
        partner->in_party = 1;
    }
    else if (reason == MENU_CALLBACK_SWITCH_OFF) {
        partner->in_party = 0;
    }
    else if (reason == MENU_CALLBACK_THINK) {
        menu_checkbox_set(item, partner->in_party);
    }
    return 0;
}

static int rank_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    partner_t *partner = (partner_t *)data;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_option_get(item) != partner->upgrade)
            menu_option_set(item, partner->upgrade);
    }
    else if (reason == MENU_CALLBACK_DEACTIVATE)
        partner->upgrade = menu_option_get(item);
    return 0;
}

static int spell_type_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_option_get(item) != pm_player.merlee.spell_type)
            menu_option_set(item, pm_player.merlee.spell_type);
    }
    else if (reason == MENU_CALLBACK_DEACTIVATE)
        pm_player.merlee.spell_type = menu_option_get(item);
    return 0;
}

static int item_int_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    uint16_t *p = data;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_intinput_get(item) != *p)
            menu_intinput_set(item, *p);
    }
    else if (reason == MENU_CALLBACK_CHANGED) {
        if (menu_intinput_get(item) > 0x16c) {
            *p = 0x16c;
        }
        else {
            *p = menu_intinput_get(item);
        }
    }
    return 0;
}

static int item_option_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    uint16_t *p = data;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_option_get(item) != *p)
            menu_option_set(item, *p);
    }
    else if (reason == MENU_CALLBACK_CHANGED)
        *p = menu_option_get(item);
    return 0;
}

static int star_power_full_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    uint8_t *p = data;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_intinput_get(item) != *p)
            menu_intinput_set(item, *p);
        if (pm_player.star_power.full_bars_filled >= pm_player.star_power.star_spirits_saved) {
            menu_intinput_set(item, pm_player.star_power.star_spirits_saved);
            *p = pm_player.star_power.star_spirits_saved;
        }
    }
    else if (reason == MENU_CALLBACK_CHANGED) {
        if (menu_intinput_get(item) > pm_player.star_power.star_spirits_saved) {
            *p = pm_player.star_power.star_spirits_saved;
        }
        else {
            *p = menu_intinput_get(item);
        }
    }
    return 0;
}

static int star_power_partial_proc(struct menu_item *item, enum menu_callback_reason reason, void *data) {
    uint8_t *p = data;
    if (reason == MENU_CALLBACK_THINK_INACTIVE) {
        if (menu_intinput_get(item) != *p)
            menu_intinput_set(item, *p);
        if (pm_player.star_power.full_bars_filled >= pm_player.star_power.star_spirits_saved) {
            menu_intinput_set(item, 0);
            *p = 0;
        }
    }
    else if (reason == MENU_CALLBACK_CHANGED) {
        if (pm_player.star_power.full_bars_filled > 6) {
            *p = 0;
        }
        else {
            *p = menu_intinput_get(item);
        }
    }
    return 0;
}

static void tab_prev_proc(struct menu_item *item, void *data)
{
    menu_tab_previous(data);
}

static void tab_next_proc(struct menu_item *item, void *data)
{
    menu_tab_next(data);
}

struct menu *create_player_menu(void) {
    static struct menu menu;
    static struct menu partners;
    static struct menu stats;
    static struct menu items;
    static struct menu key_items;
    static struct menu badges;
    static struct menu merlee;
    static struct menu star_power;

    /* initialize menu */
    menu_init(&menu, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&stats, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&partners, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&items, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&key_items, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&badges, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&merlee, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
    menu_init(&star_power, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);

    int y_value = 0;
    menu.selector = menu_add_submenu(&menu, 0, y_value++, NULL, "return");

    /*build player menu*/
    menu_add_submenu(&menu, 0, y_value++, &stats, "stats");
    menu_add_submenu(&menu, 0, y_value++, &partners, "partners");
    menu_add_submenu(&menu, 0, y_value++, &items, "items");
    menu_add_submenu(&menu, 0, y_value++, &key_items, "key items");
    menu_add_submenu(&menu, 0, y_value++, &badges, "badges");
    menu_add_submenu(&menu, 0, y_value++, &merlee, "merlee");
    menu_add_submenu(&menu, 0, y_value++, &star_power, "star power");

    /*build stats menu*/
    const int STATS_X_0 = 0;
    const int STATS_X_1 = 16;
    y_value = 0;
    stats_t *pm_stats = &pm_player.stats;

    stats.selector = menu_add_submenu(&stats, STATS_X_0, y_value++, NULL, "return");

    menu_add_static(&stats, STATS_X_0, y_value, "boots", 0xC0C0C0);
    menu_add_option(&stats, STATS_X_1, y_value++, "normal\0""super\0""ultra\0", boots_proc, NULL);

    menu_add_static(&stats, STATS_X_0, y_value, "hammer", 0xC0C0C0);
    menu_add_option(&stats, STATS_X_1, y_value++, "none\0""normal\0""super\0""ultra\0", hammer_proc, NULL);

    menu_add_static(&stats, STATS_X_0, y_value, "action commands", 0xC0C0C0);
    menu_add_checkbox(&stats, STATS_X_1, y_value++, action_commands_proc, NULL);

    menu_add_static(&stats, STATS_X_0, y_value, "hp", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1, y_value, 10, 2, byte_mod_proc, &pm_stats->hp);
    menu_add_static(&stats, STATS_X_1 + 2, y_value, "/", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1 + 3, y_value++, 10, 2, max_hp_proc, NULL);

    menu_add_static(&stats, STATS_X_0, y_value, "fp", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1, y_value, 10, 2, byte_mod_proc, &pm_stats->fp);
    menu_add_static(&stats, STATS_X_1 + 2, y_value, "/", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1 + 3, y_value++, 10, 2, max_fp_proc, NULL);

    menu_add_static(&stats, STATS_X_0, y_value, "bp", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1, y_value++, 10, 2, byte_mod_proc, &pm_stats->bp);

    menu_add_static(&stats, STATS_X_0, y_value, "level", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1, y_value++, 10, 2, byte_mod_proc, &pm_stats->level);

    menu_add_static(&stats, STATS_X_0, y_value, "star points", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1, y_value++, 10, 2, byte_mod_proc, &pm_stats->star_points);

    menu_add_static(&stats, STATS_X_0, y_value, "star pieces", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1, y_value++, 10, 2, byte_mod_proc, &pm_stats->star_pieces);

    menu_add_static(&stats, STATS_X_0, y_value, "coins", 0xC0C0C0);
    menu_add_intinput(&stats, STATS_X_1, y_value++, 10, 3, halfword_mod_proc, &pm_stats->coins);

    /*build partners menu*/
    const int PARTNERS_X_0 = 0;
    const int PARTNERS_X_1 = 11;
    const int PARTNERS_X_2 = 13;
    y_value = 0;


    partners.selector = menu_add_submenu(&partners, 0, y_value++, NULL, "return");

    menu_add_static(&partners, PARTNERS_X_0, y_value, "active", 0xC0C0C0);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, PARTNERS, active_partner_proc, NULL);

    y_value++;
    menu_add_static(&partners, PARTNERS_X_2, y_value++, "rank", 0xC0C0C0);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "goombario", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.goombario);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.goombario);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "kooper", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.kooper);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.kooper);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "bombette", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.bombette);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.bombette);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "parakarry", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.parakarry);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.parakarry);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "bow", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.bow);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.bow);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "watt", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.watt);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.watt);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "sushie", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.sushie);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.sushie);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "lakilester", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.lakilester);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.lakilester);

    y_value++;

    menu_add_static(&partners, PARTNERS_X_0, y_value, "goompa", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.goompa);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.goompa);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "goombaria", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.goombaria);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.goombaria);

    menu_add_static(&partners, PARTNERS_X_0, y_value, "twink", 0xC0C0C0);
    menu_add_checkbox(&partners, PARTNERS_X_1, y_value, in_party_proc, &pm_player.party.twink);
    menu_add_option(&partners, PARTNERS_X_2, y_value++, RANK, rank_proc, &pm_player.party.twink);

    /*build items menu*/
    const int ITEMS_X_0 = 0;
    const int ITEMS_X_1 = 3;
    const int ITEMS_X_2 = 7;
    y_value = 0;

    items.selector = menu_add_submenu(&items, ITEMS_X_0, y_value++, NULL, "return");

    int i;
    for (i = 0; i < 10; i++) {
        char buffer[3];
        sprintf(buffer, "%d:", i);
        menu_add_static(&items, ITEMS_X_0, y_value, buffer, 0xC0C0C0);
        menu_add_intinput(&items, ITEMS_X_1, y_value, 16, 3, item_int_proc, &pm_player.items[i]);
        menu_add_option(&items, ITEMS_X_2, y_value++, ITEM_LIST, item_option_proc, &pm_player.items[i]);
    }

    /*build key items menu*/
    const int KEY_ITEMS_X_0 = 0;
    const int KEY_ITEMS_X_1 = 3;
    const int KEY_ITEMS_X_2 = 7;
    y_value = 0;

    key_items.selector = menu_add_submenu(&key_items, KEY_ITEMS_X_0, y_value++, NULL, "return");

    int key_items_page_count = 2;
    struct menu *key_items_pages = malloc(sizeof(*key_items_pages) * key_items_page_count);
    struct menu_item *key_items_tab = menu_add_tab(&key_items, KEY_ITEMS_X_0, y_value++, key_items_pages, key_items_page_count);
    int page_size = 16;
    for (int i = 0; i < key_items_page_count; ++i) {
        struct menu *page = &key_items_pages[i];
        menu_init(page, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
        y_value = 0;
        for (int j = 0; j < page_size; ++j) {
            char buffer[4];
            int item_index = j + i * page_size;
            sprintf(buffer, "%02d:", item_index);

            menu_add_static(page, KEY_ITEMS_X_0, y_value, buffer, 0xC0C0C0);
            menu_add_intinput(page, KEY_ITEMS_X_1, y_value, 16, 3, item_int_proc, &pm_player.key_items[item_index]);
            menu_add_option(page, KEY_ITEMS_X_2, y_value++, ITEM_LIST, item_option_proc, &pm_player.key_items[item_index]);
        }
    }
    menu_tab_goto(key_items_tab, 0);
    menu_add_button(&key_items, 8, 0, "<", tab_prev_proc, key_items_tab);
    menu_add_button(&key_items, 10, 0, ">", tab_next_proc, key_items_tab);

    /*build badges menu*/
    const int BADGES_ITEMS_X_0 = 0;
    const int BADGES_ITEMS_X_1 = 3;
    const int BADGES_ITEMS_X_2 = 7;
    y_value = 0;

    badges.selector = menu_add_submenu(&badges, BADGES_ITEMS_X_0, y_value++, NULL, "return");

    int badge_page_count = 8;
    struct menu *badge_pages = malloc(sizeof(*badge_pages) * badge_page_count);
    struct menu_item *badge_tab = menu_add_tab(&badges, BADGES_ITEMS_X_0, y_value++, badge_pages, badge_page_count);
    for (int i = 0; i < badge_page_count; ++i) {
        struct menu *page = &badge_pages[i];
        menu_init(page, MENU_NOVALUE, MENU_NOVALUE, MENU_NOVALUE);
        y_value = 0;
        for (int j = 0; j < page_size; ++j) {
            char buffer[4];
            int item_index = j + i * page_size;
            sprintf(buffer, "%02d:", item_index);

            menu_add_static(page, BADGES_ITEMS_X_0, y_value, buffer, 0xC0C0C0);
            menu_add_intinput(page, BADGES_ITEMS_X_1, y_value, 16, 3, item_int_proc, &pm_player.badges[item_index]);
            menu_add_option(page, BADGES_ITEMS_X_2, y_value++, ITEM_LIST, item_option_proc, &pm_player.badges[item_index]);
        }
    }
    menu_tab_goto(badge_tab, 0);
    menu_add_button(&badges, 8, 0, "<", tab_prev_proc, badge_tab);
    menu_add_button(&badges, 10, 0, ">", tab_next_proc, badge_tab);

    /*build merlee menu*/
    const int MERLEE_X_0 = 0;
    const int MERLEE_X_1 = 16;
    y_value = 0;

    merlee.selector = menu_add_submenu(&merlee, MERLEE_X_0, y_value++, NULL, "return");

    menu_add_static(&merlee, MERLEE_X_0, y_value, "spell type", 0xC0C0C0);
    menu_add_option(&merlee, MERLEE_X_1, y_value++, SPELL_TYPE, spell_type_proc, NULL);

    menu_add_static(&merlee, MERLEE_X_0, y_value, "casts remaining", 0xC0C0C0);
    menu_add_intinput(&merlee, MERLEE_X_1, y_value++, 10, 2, byte_mod_proc, &pm_player.merlee.casts_remaining);

    menu_add_static(&merlee, MERLEE_X_0, y_value, "turns remaining", 0xC0C0C0);
    menu_add_intinput(&merlee, MERLEE_X_1, y_value++, 10, 2, byte_mod_proc, &pm_player.merlee.turns_until_spell);

    /*build star power menu*/
    const int STAR_POWER_X_0 = 0;
    const int STAR_POWER_X_1 = 19;
    y_value = 0;

    star_power.selector = menu_add_submenu(&star_power, STAR_POWER_X_0, y_value++, NULL, "return");

    menu_add_static(&star_power, STAR_POWER_X_0, y_value, "star spirits saved", 0xC0C0C0);
    menu_add_intinput(&star_power, STAR_POWER_X_1 + 1, y_value++, 10, 1, byte_mod_proc, &pm_player.star_power.star_spirits_saved);

    menu_add_static(&star_power, STAR_POWER_X_0, y_value, "bars full", 0xC0C0C0);
    menu_add_intinput(&star_power, STAR_POWER_X_1 + 1, y_value++, 10, 1, star_power_full_proc, &pm_player.star_power.full_bars_filled);

    menu_add_static(&star_power, STAR_POWER_X_0, y_value, "partial bar", 0xC0C0C0);
    menu_add_intinput(&star_power, STAR_POWER_X_1, y_value++, 16, 2, star_power_partial_proc, &pm_player.star_power.partial_bars_filled);

    return &menu;
}