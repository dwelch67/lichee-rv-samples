

.globl _start
_start:
    li x2,0x4F008000
    jal notmain
    j .

.globl PUT8
PUT8:
    sb x11,(x10)
    ret

.globl PUT32
PUT32:
    sw x11,(x10)
    ret

.globl GET32
GET32:
    lw x10,(x10)
    ret

.globl BRANCHTO
BRANCHTO:
    fence.i
    jalr x10
    ret
