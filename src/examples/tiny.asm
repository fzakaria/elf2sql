.code32
.globl _start
.section .text
_start:
                movl    $1, %eax
                movl    $42, %ebx
                int     $0x80