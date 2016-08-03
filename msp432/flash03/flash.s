
;@-----------------------
.cpu cortex-m0
.thumb
;@-----------------------

.thumb_func
.global _start
_start:
stacktop: .word 0x20001000
.word reset
.word 0x01000003
.word 0x01000005
.word 0x01000007
.word 0x01000009
.word 0x0100000B
.word 0x0100000D
.word 0x0100000F
.word 0x01000011
.word 0x01000013
.word 0x01000015
.word 0x01000017
.word 0x01000019
.word 0x0100001B
.word 0x0100001D
.word 0x0100001F
.word 0x01000021
.word 0x01000023
.word 0x01000025
.word 0x01000027
.word 0x01000029
.word 0x0100002B
.word 0x0100002D
.word 0x0100002F
.word 0x01000031
.word 0x01000033
.word 0x01000035
.word 0x01000037
.word 0x01000039
.word 0x0100003B
.word 0x0100003D
.word 0x0100003F
.word 0x01000041
.word 0x01000043
.word 0x01000045
.word 0x01000047
.word 0x01000049
.word 0x0100004B
.word 0x0100004D
.word 0x0100004F
.word 0x01000051
.word 0x01000053
.word 0x01000055
.word 0x01000057
.word 0x01000059
.word 0x0100005B
.word 0x0100005D
.word 0x0100005F
.word 0x01000061
.word 0x01000063
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang
;@.word hang

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
.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl GET8
GET8:
    ldrb r0,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr
;@-----------------------
.thumb_func
.globl GET16
GET16:
    ldrh r0,[r0]
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
.end
;@-----------------------
