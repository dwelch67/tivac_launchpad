

.cpu cortex-m4
.thumb

.thumb_func
.global _start
_start:
    b reset
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b systick_handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler
    b handler

reset:
    ldr r0,stacktop
    mov sp,r0
    bl notmain
    b hang
.thumb_func
.globl hang
hang:   b .



.thumb_func
systick_handler:
    push {r4,r5,r6,r7}
    mov r4,r8
    mov r5,r9
    mov r6,r10
    mov r7,r11
    push {r4,r5,r6,r7}

    mov r0,sp
    mov r4,lr
    bl systicker
    mov lr,r4
    mov sp,r0

    pop {r4,r5,r6,r7}
    mov r8,r4
    mov r9,r5
    mov r10,r6
    mov r11,r7
    pop {r4,r5,r6,r7}
    bx lr

.align
sp_table_add: .word sp_table


.align
stacktop: .word 0x20001000

.thumb_func
.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.thumb_func
.globl GET8
GET8:
    ldrb r0,[r0]
    bx lr

.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.thumb_func
.globl GET16
GET16:
    ldrh r0,[r0]
    bx lr

.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr

.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.thumb_func
.globl dummy
dummy:
    bx lr

.thumb_func
.globl GET_APSR
GET_APSR:
    mrs r0,APSR
    bx lr


.thumb_func
.globl GET_IPSR
GET_IPSR:
    mov r0,#4
    mrs r0,IPSR
    bx lr

.thumb_func
.globl GET_EPSR
GET_EPSR:
    mrs r0,EPSR
    bx lr

.thumb_func
.globl GET_CONTROL
GET_CONTROL:
    mrs r0,CONTROL
    bx lr

.thumb_func
.globl SP_PROCESS
SP_PROCESS:
    mrs r0,psp
    bx lr

.thumb_func
.globl SETPSP
SETPSP:
    msr psp,r0
    bx lr

.thumb_func
.globl SETCONTROL
SETCONTROL:
    msr CONTROL,r0
    ISB
    bx lr


.thumb_func
.globl SP_MAIN
SP_MAIN:
    mrs r0,msp
    bx lr

.thumb_func
.globl GET_SP
GET_SP:
    mov r0,sp
    bx lr

.end

