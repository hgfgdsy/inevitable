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


fact:
subu $sp, $sp, 4
sw $fp, 0($sp)
addi $fp, $sp, 4
lw $t0, 4($fp)
subu $sp, $sp, 4
sw $t0, -8($fp)
subu $sp, $sp, 4
lw $t1, -8($fp)
sw $t1, -12($fp)
subu $sp, $sp, 4
li $t0, 1
sw $t0, -16($fp)
lw $t0, -12($fp)
lw $t1, -16($fp)
beq $t0, $t1, label1
j label2
label1:
subu $sp, $sp, 4
lw $t1, -8($fp)
sw $t1, -20($fp)
lw $t0, -20($fp)
move $t1, $fp
lw $fp, -4($t1)
move $sp, $t1
move $v0, $t0
jr $ra
j label3
label2:
subu $sp, $sp, 4
lw $t1, -8($fp)
sw $t1, -24($fp)
subu $sp, $sp, 4
lw $t1, -8($fp)
sw $t1, -28($fp)
subu $sp, $sp, 4
li $t0, 1
sw $t0, -32($fp)
subu $sp, $sp, 4
lw $t0, -28($fp)
lw $t1, -32($fp)
sub $t2, $t0, $t1
sw $t2, -36($fp)
subu $sp, $sp, 4
lw $t0, -36($fp)
sw $t0, -40($fp)
subu $sp, $fp, 40
subu $sp, $sp, 4
sw $ra, 0($sp)
jal fact
lw $ra, 0($sp)
addu $sp, $sp, 4
addu $sp, $sp, 4
subu $sp, $sp, 4
sw $v0, -40($fp)
subu $sp, $sp, 4
lw $t0, -24($fp)
lw $t1, -40($fp)
mul $t2, $t0, $t1
sw $t2, -44($fp)
lw $t0, -44($fp)
move $t1, $fp
lw $fp, -4($t1)
move $sp, $t1
move $v0, $t0
jr $ra
label3:


main:
subu $sp, $sp, 4
sw $fp, 0($sp)
addi $fp, $sp, 4
subu $sp, $fp, 4
subu $sp, $sp, 4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addu $sp, $sp, 4
subu $sp, $sp, 4
sw $v0, -8($fp)
subu $sp, $sp, 4
lw $t1, -8($fp)
sw $t1, -12($fp)
subu $sp, $sp, 4
lw $t1, -12($fp)
sw $t1, -16($fp)
subu $sp, $sp, 4
li $t0, 1
sw $t0, -20($fp)
lw $t0, -16($fp)
lw $t1, -20($fp)
bgt $t0, $t1, label4
j label5
label4:
subu $t0, $fp, 12
subu $sp, $sp, 4
sw $t0, -24($fp)
subu $sp, $sp, 4
lw $t1, -12($fp)
sw $t1, -28($fp)
subu $sp, $sp, 4
lw $t0, -28($fp)
sw $t0, -32($fp)
subu $sp, $fp, 32
subu $sp, $sp, 4
sw $ra, 0($sp)
jal fact
lw $ra, 0($sp)
addu $sp, $sp, 4
addu $sp, $sp, 4
subu $sp, $sp, 4
sw $v0, -32($fp)
subu $sp, $sp, 4
lw $t1, -32($fp)
sw $t1, -36($fp)
j label6
label5:
subu $sp, $sp, 4
li $t0, 1
sw $t0, -40($fp)
lw $t1, -40($fp)
sw $t1, -36($fp)
label6:
subu $t0, $fp, 36
subu $sp, $sp, 4
sw $t0, -44($fp)
subu $sp, $sp, 4
lw $t1, -36($fp)
sw $t1, -48($fp)
lw $a0, -48($fp)
subu $sp, $fp, 48
subu $sp, $sp, 4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addu $sp, $sp, 4
subu $sp, $sp, 4
li $t0, 0
sw $t0, -52($fp)
lw $t0, -52($fp)
move $t1, $fp
lw $fp, -4($t1)
move $sp, $t1
move $v0, $t0
jr $ra
