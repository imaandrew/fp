#ifndef HB_H
#include <stdint.h>
#include "pm64.h"

#define hb_regs               (*(volatile hb_regs_t *)0xA8050000)

#define HB_STATUS_RESET       (0b1 << 9)
#define HB_STATUS_ERROR       (0b1111 << 5)
#define HB_STATUS_SD_INIT     (0b1 << 4)
#define HB_STATUS_SD_HC       (0b1 << 3)
#define HB_STATUS_SD_INSERTED (0b1 << 2)
#define HB_STATUS_SD_BUSY     (0b1 << 1)
#define HB_STATUS_SD_READY    (0b1 << 0)

#define HB_ERROR_SUCCESS      0
#define HB_ERROR_INVAL        1
#define HB_ERROR_QUEUEFULL    2
#define HB_ERROR_NOMEM        3
#define HB_ERROR_NOBUFFER     4
#define HB_ERROR_OTHER        5

#define HB_TIMEBASE_FREQ      60750000

typedef struct {
    u32 key;            /* 0x0000 */
    u32 sd_dram_addr;   /* 0x0004 */
    u32 sd_write_lba;   /* 0x0008 */
    u32 sd_read_lba;    /* 0x000C */
    u32 sd_n_blocks;    /* 0x0010 */
    u32 status;         /* 0x0014 */
    u32 dram_save_addr; /* 0x0018 */
    u32 dram_save_len;  /* 0x001C */
    u32 dram_save_key;  /* 0x0020 */
    u32 timebase_hi;    /* 0x0024 */
    u32 timebase_lo;    /* 0x0028 */
                        /* 0x002C */
} hb_regs_t;

#endif
