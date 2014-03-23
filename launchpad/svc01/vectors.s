
.cpu cortex-m3
.thumb

.word   0x20008000  /* stack top address */
.word   _start      /* 1 Reset */
.word   hang        /* 2 NMI */
.word   hang        /* 3 HardFault */
.word   hang        /* 4 MemManage */
.word   hang        /* 5 BusFault */
.word   hang        /* 6 UsageFault */
.word   hang        /* 7 RESERVED */
.word   hang        /* 8 RESERVED */
.word   hang        /* 9 RESERVED*/
.word   hang        /* 10 RESERVED */
.word   svc_handler /* 11 SVCall */
.word   hang        /* 12 Debug Monitor */
.word   hang        /* 13 RESERVED */
.word   hang        /* 14 PendSV */
.word   hang        /* 15 SysTick */
.word   hang        /* 16 External Interrupt(0) */
.word   hang        /* 17 External Interrupt(1) */
.word   hang        /* 18 External Interrupt(2) */
.word   hang        /* 19 ...   */

.thumb_func
hang:   b .

.thumb_func
.globl _start
_start:
    bl notmain
    b hang

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
svc_handler:
    push {lr}
    ldr r0,[sp,#0x1C] @ return address
    sub r0,r0,#2
    ldrh r0,[r0]
    mov r1,#0xFF
    and r0,r1
    bl hexstring
    pop {pc}


.thumb_func
.globl svc_test
svc_test:
    mov r1,#1
    mov r2,#2
    mov r3,#3
    svc 13
    bx lr





