; hello_world.asm
; Writes "Hello, World" to the console using only system calls.
; Runs on 64-bit Linux only.
BITS 64
GLOBAL _start
SECTION .rodata
    message: db "Hello, World!", 0xa   ; for reference: 0xa = "\n"
    len: equ $-message       ; "len" will calculate the current
                             ; offset minus the "msg" offset.
                             ; this should give us the size of
                             ; "msg".

SECTION .text
_start:
   ;######################################
   ; syscall - write(1, msg, len);
   ;######################################
    mov rax, 1 ; write system call
    mov rdi, 1 ; file descriptor (stdout)
    mov rsi, message  ; pointer to message
    mov rdx, len  ; length of message including newline
    syscall
   ;######################################
   ; syscall - exit(0);
   ;######################################
    mov rax, 60 ; exit system call
    xor rdi, rdi  ; exit status code of 0
    syscall