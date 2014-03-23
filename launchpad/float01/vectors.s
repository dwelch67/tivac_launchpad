
/* vectors.s */
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


.thumb_func
.globl fpu_init
fpu_init:
    ;@ basically straight from ti datasheet
    ldr r0,=0xE000ED88
    ldr r1,[r0]
    ldr r2,=0xF00000
    orr r1,r2
    str r1,[r0]
    dsb
    isb
    bx lr

.thumb_func
.globl fun
fun:
    vmov s0,r0
    vcvt.f32.u32 s0,s0
    vmov s1,r1
    vcvt.f32.u32 s1,s1
    vmul.f32 s0,s0,s1
    vcvt.u32.f32 s0,s0
    vmov r0,s0
    bx lr

.end
