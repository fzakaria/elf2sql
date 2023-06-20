; tiny.asm
BITS 64
GLOBAL _start
SECTION .text
_start:
              ; Arch/ABI      arg1  arg2  arg3  arg4  arg5  arg6  arg7  Notes
              ;  x86-64        rdi   rsi   rdx   r10   r8    r9    -
              ; https://github.com/torvalds/linux/blob/v4.17/arch/x86/entry/syscalls/syscall_64.tbl#L71
              mov     rax, 60
              ; rdi is the value to put in the return value
              mov     rdi, 42
              syscall