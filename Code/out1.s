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
subu $sp, $sp, 4
li $t0, 0
sw $t0, -8($fp)
subu $sp, $sp, 4
lw $t1, -8($fp)
sw $t1, -12($fp)
subu $sp, $sp, 4
li $t0, 1
sw $t0, -16($fp)
subu $sp, $sp, 4
lw $t1, -16($fp)
sw $t1, -20($fp)
subu $sp, $sp, 4
li $t0, 0
sw $t0, -24($fp)
subu $sp, $sp, 4
lw $t1, -24($fp)
sw $t1, -28($fp)
subu $sp, $sp, 4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addu $sp, $sp, 4
subu $sp, $sp, 4
sw $v0, -32($fp)
subu $sp, $sp, 4
lw $t1, -32($fp)
sw $t1, -36($fp)
label1:
subu $sp, $sp, 4
lw $t1, -28($fp)
sw $t1, -40($fp)
subu $sp, $sp, 4
lw $t1, -36($fp)
sw $t1, -44($fp)
lw $t0, -40($fp)
lw $t1, -44($fp)
blt $t0, $t1, label2
j label3
label2:
subu $sp, $sp, 4
lw $t1, -12($fp)
sw $t1, -48($fp)
subu $sp, $sp, 4
lw $t1, -20($fp)
sw $t1, -52($fp)
subu $sp, $sp, 4
lw $t0, -48($fp)
lw $t1, -52($fp)
add $t2, $t0, $t1
sw $t2, -56($fp)
subu $sp, $sp, 4
lw $t1, -56($fp)
sw $t1, -60($fp)
subu $t0, $fp, 20
subu $sp, $sp, 4
sw $t0, -64($fp)
subu $sp, $sp, 4
lw $t1, -20($fp)
sw $t1, -68($fp)
lw $a0, -68($fp)
subu $sp, $sp, 4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addu $sp, $sp, 4
subu $sp, $sp, 4
lw $t1, -20($fp)
sw $t1, -72($fp)
lw $t1, -72($fp)
sw $t1, -12($fp)
subu $sp, $sp, 4
lw $t1, -60($fp)
sw $t1, -76($fp)
lw $t1, -76($fp)
sw $t1, -20($fp)
subu $sp, $sp, 4
lw $t1, -28($fp)
sw $t1, -80($fp)
subu $sp, $sp, 4
li $t0, 1
sw $t0, -84($fp)
subu $sp, $sp, 4
lw $t0, -80($fp)
lw $t1, -84($fp)
add $t2, $t0, $t1
sw $t2, -88($fp)
lw $t1, -88($fp)
sw $t1, -28($fp)
j label1
label3:
subu $sp, $sp, 4
li $t0, 0
sw $t0, -92($fp)
lw $t0, -92($fp)
lw $fp, 88($sp)
addi $sp, $sp, 92
move $v0, $t0
jr $ra
