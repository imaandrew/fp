local rom = gru.n64rom_load("pape.z64")

local old_ldr = rom:copy(0x25E2C, 0x54)
local ldr = gru.blob_load("ldr.bin")
rom:write(0x25E2C, ldr)
rom:write(0x2800000, old_ldr)

local payload = gru.blob_load("fp.bin")
rom:write(0x2800084, payload)

local hooks = gru.gsc_load("hooks.gsc")
hooks:apply_be(rom)

rom:crc_update()
rom:save_file("fp.z64")
