
;@-----------------------
.cpu cortex-m0
.thumb
;@-----------------------

.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
.word reset @; reset
.word hang  @; NMI
.word hang  @; HardFault
.word hang  @; MemManage
.word hang  @; BusFault
.word hang  @; UsageFault
.word hang  @; Reserved
.word hang  @; Reserved
.word hang  @; Reserved
.word hang  @; Reserved
.word hang  @; SVCall
.word hang  @; DebugMonitor
.word hang  @; Reserved
.word hang  @; PendSV
.word hang  @; SysTick 
.word hang  @; External interrupt 0
.word hang  @; External interrupt 1
.word hang  @; External interrupt 2
.word hang  @; External interrupt 3
.word hang  @; External interrupt 4
.word hang  @; External interrupt 5
.word hang  @; External interrupt 6
.word hang  @; External interrupt 7
.word hang  @; External interrupt 8
.word hang  @; External interrupt 9
.word hang  @; External interrupt 10
.word hang  @; External interrupt 11
.word hang  @; External interrupt 12
.word hang  @; External interrupt 13
.word hang  @; External interrupt 14
.word hang  @; External interrupt 15
.word hang  @; External interrupt 16
.word hang  @; External interrupt 17
.word hang  @; External interrupt 18
.word hang  @; External interrupt 19
.word hang  @; External interrupt 20
.word hang  @; External interrupt 21
.word hang  @; External interrupt 22
.word hang  @; External interrupt 23
.word hang  @; External interrupt 24
.word interrupt_25_handler  @; External interrupt 25
.word hang  @; External interrupt 26
.word hang  @; External interrupt 27
.word hang  @; External interrupt 28
.word hang  @; External interrupt 29

.thumb_func
reset:
    bl notmain
    b hang
.thumb_func
hang:   b .
;@-----------------------
.align
;@-----------------------
.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl PUT32
PUT32:
    str r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl GET32
GET32:
    ldr r0,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl dummy
dummy:
    bx lr
;@-----------------------
.thumb_func
.globl CPSIE
CPSIE:
    cpsie i
    bx lr
    
;@-----------------------
.end
;@-----------------------
