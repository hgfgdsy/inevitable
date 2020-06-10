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
lw $t0, -12($fp)
move $t1, $fp
lw $fp, -4($t1)
move $sp, $t1
move $v0, $t0
jr $ra


main:
subu $sp, $sp, 4
sw $fp, 0($sp)
addi $fp, $sp, 4
subu $sp, $sp, 40
subu $sp, $sp, 4
li $t0, 1
sw $t0, -48($fp)
subu $sp, $sp, 4
lw $t1, -48($fp)
sw $t1, -52($fp)
subu $sp, $sp, 4
lw $t1, -52($fp)
sw $t1, -56($fp)
subu $sp, $sp, 4
lw $t1, -52($fp)
sw $t1, -60($fp)
subu $sp, $sp, 4
lw $t0, -56($fp)
lw $t1, -60($fp)
mul $t2, $t0, $t1
sw $t2, -64($fp)
subu $sp, $sp, 4
li $t0, 1
sw $t0, -68($fp)
subu $sp, $sp, 4
lw $t0, -64($fp)
lw $t1, -68($fp)
add $t2, $t0, $t1
sw $t2, -72($fp)
subu $sp, $sp, 4
lw $t1, -72($fp)
sw $t1, -76($fp)
subu $sp, $sp, 4
lw $t1, -76($fp)
sw $t1, -80($fp)
subu $sp, $sp, 4
lw $t1, -80($fp)
sw $t1, -84($fp)
subu $t0, $fp, 76
subu $sp, $sp, 4
sw $t0, -88($fp)
subu $sp, $sp, 4
lw $t1, -76($fp)
sw $t1, -92($fp)
subu $sp, $sp, 4
lw $t0, -92($fp)
sw $t0, -96($fp)
subu $sp, $fp, 96
subu $sp, $sp, 4
sw $ra, 0($sp)
jal fact
lw $ra, 0($sp)
addu $sp, $sp, 4
addu $sp, $sp, 4
subu $sp, $sp, 4
sw $v0, -96($fp)
lw $t1, -96($fp)
sw $t1, -84($fp)
subu $sp, $fp, 96
subu $sp, $sp, 4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addu $sp, $sp, 4
subu $sp, $sp, 4
sw $v0, -100($fp)
lw $t1, -100($fp)
sw $t1, -84($fp)
subu $sp, $sp, 4
lw $t1, -84($fp)
sw $t1, -104($fp)
subu $sp, $sp, 4
lw $t1, -84($fp)
sw $t1, -108($fp)
subu $sp, $sp, 4
lw $t0, -104($fp)
lw $t1, -108($fp)
mul $t2, $t0, $t1
sw $t2, -112($fp)
subu $sp, $sp, 4
li $t0, 111
sw $t0, -116($fp)
subu $sp, $sp, 4
lw $t0, -112($fp)
lw $t1, -116($fp)
sub $t2, $t0, $t1
sw $t2, -120($fp)
lw $a0, -120($fp)
subu $sp, $fp, 120
subu $sp, $sp, 4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addu $sp, $sp, 4
subu $sp, $sp, 4
lw $t1, -52($fp)
sw $t1, -124($fp)
lw $t0, -124($fp)
move $t1, $fp
lw $fp, -4($t1)
move $sp, $t1
move $v0, $t0
jr $ra
