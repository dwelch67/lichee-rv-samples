

.globl _start
_start:
    li x2,0x50000000
    jal notmain
    j .

.globl PUT32
PUT32:
    sw x11,(x10)
    ret

.globl GET32
GET32:
    lw x10,(x10)
    ret

