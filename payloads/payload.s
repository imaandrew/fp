li $t0, 0x8010F45D
li $t1, 0x800B1D56

lb $t2, 0($t1)
abs $t2, $t2
sb $t2, 3($t0)

lb $t2, 1($t1)
abs $t2, $t2
sb $t2, 0($t0)
li $ra, 0x800F3E94
jr $ra