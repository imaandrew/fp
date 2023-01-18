#include <stdlib.h>
#include "pm64.h"
#include "lz_rando.h"

u16 get_rand_area() {
    return rand() % AREA_COUNT;
}

u16 get_rand_map(u16 area) {
    return rand() % AREAS2[area].map_count;
}

u16 get_rand_entrance(u16 area, u16 map) {
    return rand() % AREAS2[area].maps[map].entrance_count;
}