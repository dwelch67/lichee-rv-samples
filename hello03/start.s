

.globl _start
_start:
    li x10,0x02500000
    li x11,0x55
    sw x11,(x10)
    li x2,0x4A000000
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

.globl dummy
dummy:
    ret

