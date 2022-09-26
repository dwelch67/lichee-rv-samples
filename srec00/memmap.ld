
MEMORY
{
    dram : ORIGIN = 0x4F000000, LENGTH = 0x1000
}
SECTIONS
{
    .text   : { *(.text*)   } > dram
}

