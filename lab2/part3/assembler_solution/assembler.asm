; x86-64 macOS, NASM
; Задача:
; Ввод: N, затем N натуральных чисел
; Вывод: для каждого числа:
;   - если число однозначное (1..9) -> "-"
;   - иначе количество чётных цифр
; Результаты выводятся через пробел

global _start

section .bss
    inbuf       resb 1
    numbuf      resb 32
    outbuf      resb 32

section .text

; ---------------- syscalls ----------------
; macOS x86-64 syscall numbers:
; rax=0x2000003 sys_read,  rdi=fd, rsi=buf, rdx=len
; rax=0x2000004 sys_write, rdi=fd, rsi=buf, rdx=len
; rax=0x2000001 sys_exit,  rdi=code

; ---------------- read_int ----------------
; Читает очередное натуральное число из stdin
; Возвращает: rax = число
read_int:
    xor rax, rax            ; result = 0

.skip_non_digit:
    mov rax, 0x2000003       ; read
    mov rdi, 0
    lea rsi, [rel inbuf]
    mov rdx, 1
    syscall
    cmp rax, 0
    je .done_eof            ; EOF -> вернём 0
    mov bl, [rel inbuf]
    cmp bl, '0'
    jb .skip_non_digit
    cmp bl, '9'
    ja .skip_non_digit

    ; первая цифра уже в bl
    xor rax, rax
.parse_loop:
    movzx rcx, bl
    sub rcx, '0'
    imul rax, rax, 10
    add rax, rcx

    ; читаем следующий символ
    mov r8, rax
    mov rax, 0x2000003
    mov rdi, 0
    lea rsi, [rel inbuf]
    mov rdx, 1
    syscall
    cmp rax, 0
    je .ret_saved
    mov bl, [rel inbuf]
    cmp bl, '0'
    jb .ret_saved
    cmp bl, '9'
    ja .ret_saved
    mov rax, r8
    jmp .parse_loop

.ret_saved:
    mov rax, r8

.ret_val:
    ret

.done_eof:
    xor rax, rax
    ret

; ---------------- write_str ----------------
; rsi=адрес, rdx=длина
write_str:
    mov rax, 0x2000004
    mov rdi, 1
    syscall
    ret

; ---------------- write_char ----------------
; al = символ
write_char:
    mov [rel outbuf], al
    lea rsi, [rel outbuf]
    mov rdx, 1
    call write_str
    ret

; ---------------- write_uint ----------------
; rax = беззнаковое число
write_uint:
    mov rbx, 10
    lea rdi, [rel numbuf + 31]
    mov byte [rdi], 0
    dec rdi

    cmp rax, 0
    jne .conv
    mov byte [rdi], '0'
    jmp .print

.conv:
    xor rdx, rdx
.loop:
    xor rdx, rdx
    div rbx                 ; rax = rax/10, rdx = remainder
    add dl, '0'
    mov [rdi], dl
    dec rdi
    test rax, rax
    jnz .loop
    inc rdi

.print:
    lea rsi, [rdi]
    lea rcx, [rel numbuf + 31]
    sub rcx, rsi
    mov rdx, rcx
    call write_str
    ret

; ---------------- main ----------------
_start:
    ; N
    call read_int
    mov r12, rax            ; N
    xor r13, r13            ; i = 0

.for_each:
    cmp r13, r12
    jge .exit

    call read_int           ; rax = x
    mov r14, rax            ; x
    mov r15, rax            ; copy for processing

    ; если 1..9 -> "-"
    cmp r14, 9
    jg .count_even
    cmp r14, 1
    jl .count_even          ; на случай некорректного ввода
    mov al, '-'
    call write_char
    jmp .after_print

.count_even:
    xor r8, r8              ; evenCount = 0
    mov rbx, 10
.digit_loop:
    cmp r15, 0
    je .print_count
    xor rdx, rdx
    mov rax, r15
    div rbx                 ; rax=quotient, rdx=digit
    ; digit в rdx
    test dl, 1
    jnz .odd
    inc r8
.odd:
    mov r15, rax
    jmp .digit_loop

.print_count:
    mov rax, r8
    call write_uint

.after_print:
    inc r13
    cmp r13, r12
    jge .exit
    mov al, ' '
    call write_char
    jmp .for_each

.exit:
    mov rax, 0x2000001
    xor rdi, rdi
    syscall