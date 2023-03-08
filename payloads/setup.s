; 8010F604 = 0x2A26
li $t0, 0x8010F604
addiu $t1, $r0, 0x2A26
sh $t1, 0($t0)

; 800DBD70 = 0x800B1010
li $t0, 0x800DBD70
li $t1, 0x800B1010
sw $t1, 0($t0)

; 800DBD74 = 0x0121
addiu $t1, $r0, 0x121
sh $t1, 4($t0)

; 800B1010 = 0x00 * 0x571
li $t0, 0x800B1010
addiu $t1, $t0, 0x570
L1:
sb $r0, 0($t0)
addiu $t0, $t0, 1
bne $t0, $t1, L1
nop

; 800B1541 = 0x800DBD80
li $t0, 0x800B1540
li $t1, 0x00800DBD
sw $t1, 0($t0)
li $t1, 0x80
sb $t1, 4($t0)

; 800B1545 = 0xFD
li $t1, 0xFD
sb $t1, 5($t0)

; write payload
li $t0, 0x800DBD80
li $t1, 0x3C088010
sw $t1, 0x00($t0)
li $t1, 0x3508F45D
sw $t1, 0x04($t0)
li $t1, 0x3C09800B
sw $t1, 0x08($t0)
li $t1, 0x35291D56
sw $t1, 0x0C($t0)
li $t1, 0x812A0000
sw $t1, 0x10($t0)
li $t1, 0x000A0FC3
sw $t1, 0x14($t0)
li $t1, 0x01415026
sw $t1, 0x18($t0)
li $t1, 0x01415023
sw $t1, 0x1C($t0)
li $t1, 0xA10A0003
sw $t1, 0x20($t0)
li $t1, 0x812A0001
sw $t1, 0x24($t0)
li $t1, 0x000A0FC3
sw $t1, 0x28($t0)
li $t1, 0x01415026
sw $t1, 0x2C($t0)
li $t1, 0x01415023
sw $t1, 0x30($t0)
li $t1, 0xA10A0000
sw $t1, 0x34($t0)
li $t1, 0x3C1F800F
sw $t1, 0x38($t0)
li $t1, 0x37FF3E94
sw $t1, 0x3C($t0)
li $t1, 0x03E00008
sw $t1, 0x40($t0)

; save game
jal 0x802DC150;
nop
jr $ra
nop