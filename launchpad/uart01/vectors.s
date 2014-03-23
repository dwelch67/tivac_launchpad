
.cpu cortex-m3
.thumb

.word   0x20008000  /* stack top address */
.word   _start      /* Reset */
.word   hang
.word   hang
/* ... */

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
