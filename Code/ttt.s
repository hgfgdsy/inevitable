.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
li $v0, 4
la $a0, _prompt
syscall
li $v0, 5
syscall
jr $ra

write:
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
jr $ra


main:
subu $sp, $sp, 4
sw $fp, 0($sp)
addi $fp, $sp, 4
subu $sp, $fp, 8
li $t0, 10
sw $t0, -8($fp)
subu $sp, $fp, 12
lw $t1, -8($fp)
sw $t1, -12($fp)
subu $sp, $sp, 40
subu $sp, $fp, 56
li $t0, 0
sw $t0, -56($fp)
subu $t0, $fp, 52
subu $sp, $fp, 60
sw $t0, -60($fp)
subu $sp, $fp, 64
li $t0, 0
sw $t0, -64($fp)
lw $t0, -64($fp)
li $t2, 10
mul $t1, $t0, $t2
sw $t1, -64($fp)
subu $sp, $fp, 64
lw $t0, -64($fp)
lw $t1, -56($fp)
add $t2, $t0, $t1
sw $t2, -68($fp)
subu $sp, $fp, 72
li $t0, 2
sw $t0, -72($fp)
lw $t0, -68($fp)
li $t2, 4
mul $t1, $t0, $t2
sw $t1, -68($fp)
lw $t0, -68($fp)
lw $t1, -60($fp)
add $t2, $t0, $t1
sw $t2, -68($fp)
lw $t0, -68($fp)
lw $t1, -72($fp)
sw $t1, 0($t0)
subu $sp, $fp, 76
li $t0, 1
sw $t0, -76($fp)
subu $sp, $fp, 80
lw $t1, -76($fp)
sw $t1, -80($fp)
subu $sp, $fp, 84
li $t0, 3
sw $t0, -84($fp)
subu $sp, $fp, 88
lw $t1, -84($fp)
sw $t1, -88($fp)
label1:
subu $sp, $fp, 92
lw $t1, -80($fp)
sw $t1, -92($fp)
subu $sp, $fp, 96
lw $t1, -12($fp)
sw $t1, -96($fp)
lw $t0, -92($fp)
lw $t1, -96($fp)
blt $t0, $t1, label2
j label3
label2:
subu $sp, $fp, 100
li $t0, 0
sw $t0, -100($fp)
subu $sp, $fp, 104
lw $t1, -100($fp)
sw $t1, -104($fp)
label4:
subu $sp, $fp, 108
lw $t1, -104($fp)
sw $t1, -108($fp)
subu $t0, $fp, 52
subu $sp, $fp, 112
sw $t0, -112($fp)
subu $sp, $fp, 116
li $t0, 0
sw $t0, -116($fp)
lw $t0, -116($fp)
li $t2, 10
mul $t1, $t0, $t2
sw $t1, -116($fp)
lw $t0, -116($fp)
lw $t1, -108($fp)
add $t2, $t0, $t1
sw $t2, -116($fp)
lw $t0, -116($fp)
li $t2, 4
mul $t1, $t0, $t2
sw $t1, -116($fp)
lw $t0, -116($fp)
lw $t1, -112($fp)
add $t2, $t0, $t1
sw $t2, -116($fp)
lw $t0, -116($fp)
lw $t1, 0($t0)
subu $sp, $fp, 120
sw $t1, -120($fp)
subu $sp, $fp, 124
lw $t1, -104($fp)
sw $t1, -124($fp)
subu $t0, $fp, 52
subu $sp, $fp, 128
sw $t0, -128($fp)
subu $sp, $fp, 132
li $t0, 0
sw $t0, -132($fp)
lw $t0, -132($fp)
li $t2, 10
mul $t1, $t0, $t2
sw $t1, -132($fp)
lw $t0, -132($fp)
lw $t1, -124($fp)
add $t2, $t0, $t1
sw $t2, -132($fp)
lw $t0, -132($fp)
li $t2, 4
mul $t1, $t0, $t2
sw $t1, -132($fp)
lw $t0, -132($fp)
lw $t1, -128($fp)
add $t2, $t0, $t1
sw $t2, -132($fp)
lw $t0, -132($fp)
lw $t1, 0($t0)
subu $sp, $fp, 136
sw $t1, -136($fp)
subu $sp, $fp, 136
lw $t0, -120($fp)
lw $t1, -136($fp)
mul $t2, $t0, $t1
sw $t2, -140($fp)
subu $sp, $fp, 144
lw $t1, -88($fp)
sw $t1, -144($fp)
lw $t0, -140($fp)
lw $t1, -144($fp)
ble $t0, $t1, label5
j label6
label5:
subu $sp, $fp, 148
lw $t1, -88($fp)
sw $t1, -148($fp)
subu $sp, $fp, 152
lw $t1, -104($fp)
sw $t1, -152($fp)
subu $t0, $fp, 52
subu $sp, $fp, 156
sw $t0, -156($fp)
subu $sp, $fp, 160
li $t0, 0
sw $t0, -160($fp)
lw $t0, -160($fp)
li $t2, 10
mul $t1, $t0, $t2
sw $t1, -160($fp)
lw $t0, -160($fp)
lw $t1, -152($fp)
add $t2, $t0, $t1
sw $t2, -160($fp)
lw $t0, -160($fp)
li $t2, 4
mul $t1, $t0, $t2
sw $t1, -160($fp)
lw $t0, -160($fp)
lw $t1, -156($fp)
add $t2, $t0, $t1
sw $t2, -160($fp)
lw $t0, -160($fp)
lw $t1, 0($t0)
subu $sp, $fp, 164
sw $t1, -164($fp)
subu $sp, $fp, 168
lw $t1, -88($fp)
sw $t1, -168($fp)
subu $sp, $fp, 172
lw $t1, -104($fp)
sw $t1, -172($fp)
subu $t0, $fp, 52
subu $sp, $fp, 176
sw $t0, -176($fp)
subu $sp, $fp, 180
li $t0, 0
sw $t0, -180($fp)
lw $t0, -180($fp)
li $t2, 10
mul $t1, $t0, $t2
sw $t1, -180($fp)
lw $t0, -180($fp)
lw $t1, -172($fp)
add $t2, $t0, $t1
sw $t2, -180($fp)
lw $t0, -180($fp)
li $t2, 4
mul $t1, $t0, $t2
sw $t1, -180($fp)
lw $t0, -180($fp)
lw $t1, -176($fp)
add $t2, $t0, $t1
sw $t2, -180($fp)
lw $t0, -180($fp)
lw $t1, 0($t0)
subu $sp, $fp, 184
sw $t1, -184($fp)
subu $sp, $fp, 184
lw $t0, -168($fp)
lw $t1, -184($fp)
div $t0, $t1
mflo $t2
sw $t2, -188($fp)
subu $sp, $fp, 188
lw $t0, -164($fp)
lw $t1, -188($fp)
mul $t2, $t0, $t1
sw $t2, -192($fp)
lw $t0, -148($fp)
lw $t1, -192($fp)
beq $t0, $t1, label7
j label8
label7:
subu $sp, $fp, 196
lw $t1, -88($fp)
sw $t1, -196($fp)
subu $sp, $fp, 200
li $t0, 2
sw $t0, -200($fp)
subu $sp, $fp, 200
lw $t0, -196($fp)
lw $t1, -200($fp)
add $t2, $t0, $t1
sw $t2, -204($fp)
lw $t1, -204($fp)
sw $t1, -88($fp)
subu $sp, $fp, 208
li $t0, 1
sw $t0, -208($fp)
lw $t1, -208($fp)
sw $t1, -104($fp)
j label9
label8:
subu $sp, $fp, 212
lw $t1, -104($fp)
sw $t1, -212($fp)
subu $sp, $fp, 216
li $t0, 1
sw $t0, -216($fp)
subu $sp, $fp, 216
lw $t0, -212($fp)
lw $t1, -216($fp)
add $t2, $t0, $t1
sw $t2, -220($fp)
lw $t1, -220($fp)
sw $t1, -104($fp)
label9:
j label4
label6:
subu $sp, $fp, 224
lw $t1, -80($fp)
sw $t1, -224($fp)
subu $t0, $fp, 52
subu $sp, $fp, 228
sw $t0, -228($fp)
subu $sp, $fp, 232
li $t0, 0
sw $t0, -232($fp)
lw $t0, -232($fp)
li $t2, 10
mul $t1, $t0, $t2
sw $t1, -232($fp)
subu $sp, $fp, 232
lw $t0, -232($fp)
lw $t1, -224($fp)
add $t2, $t0, $t1
sw $t2, -236($fp)
subu $sp, $fp, 240
lw $t1, -88($fp)
sw $t1, -240($fp)
lw $t0, -236($fp)
li $t2, 4
mul $t1, $t0, $t2
sw $t1, -236($fp)
lw $t0, -236($fp)
lw $t1, -228($fp)
add $t2, $t0, $t1
sw $t2, -236($fp)
lw $t0, -236($fp)
lw $t1, -240($fp)
sw $t1, 0($t0)
subu $sp, $fp, 244
lw $t1, -80($fp)
sw $t1, -244($fp)
subu $sp, $fp, 248
li $t0, 1
sw $t0, -248($fp)
subu $sp, $fp, 248
lw $t0, -244($fp)
lw $t1, -248($fp)
add $t2, $t0, $t1
sw $t2, -252($fp)
lw $t1, -252($fp)
sw $t1, -80($fp)
subu $sp, $fp, 256
lw $t1, -88($fp)
sw $t1, -256($fp)
subu $sp, $fp, 260
li $t0, 2
sw $t0, -260($fp)
subu $sp, $fp, 260
lw $t0, -256($fp)
lw $t1, -260($fp)
add $t2, $t0, $t1
sw $t2, -264($fp)
lw $t1, -264($fp)
sw $t1, -88($fp)
j label1
label3:
subu $sp, $fp, 268
li $t0, 0
sw $t0, -268($fp)
lw $t1, -268($fp)
sw $t1, -104($fp)
label10:
subu $sp, $fp, 272
lw $t1, -104($fp)
sw $t1, -272($fp)
subu $sp, $fp, 276
lw $t1, -80($fp)
sw $t1, -276($fp)
lw $t0, -272($fp)
lw $t1, -276($fp)
blt $t0, $t1, label11
j label12
label11:
subu $sp, $fp, 280
lw $t1, -104($fp)
sw $t1, -280($fp)
subu $t0, $fp, 52
subu $sp, $fp, 284
sw $t0, -284($fp)
subu $sp, $fp, 288
li $t0, 0
sw $t0, -288($fp)
lw $t0, -288($fp)
li $t2, 10
mul $t1, $t0, $t2
sw $t1, -288($fp)
lw $t0, -288($fp)
lw $t1, -280($fp)
add $t2, $t0, $t1
sw $t2, -288($fp)
lw $t0, -288($fp)
li $t2, 4
mul $t1, $t0, $t2
sw $t1, -288($fp)
lw $t0, -288($fp)
lw $t1, -284($fp)
add $t2, $t0, $t1
sw $t2, -288($fp)
lw $t0, -288($fp)
lw $t1, 0($t0)
subu $sp, $fp, 292
sw $t1, -292($fp)
lw $a0, -292($fp)
subu $sp, $fp, 292
subu $sp, $sp, 4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addu $sp, $sp, 4
subu $sp, $fp, 296
lw $t1, -104($fp)
sw $t1, -296($fp)
subu $sp, $fp, 300
li $t0, 1
sw $t0, -300($fp)
subu $sp, $fp, 300
lw $t0, -296($fp)
lw $t1, -300($fp)
add $t2, $t0, $t1
sw $t2, -304($fp)
lw $t1, -304($fp)
sw $t1, -104($fp)
j label10
label12:
subu $sp, $fp, 308
li $t0, 0
sw $t0, -308($fp)
lw $t0, -308($fp)
move $t1, $fp
lw $fp, -4($t1)
move $sp, $t1
move $v0, $t0
jr $ra
