
MEMORY
{
    ram : ORIGIN = 0x45000, LENGTH = 0x1000
}
SECTIONS
{
    .text   : { *(.text*)   } > ram
    .rodata : { *(.rodata*) } > ram
    .bss    : { *(.bss*)    } > ram
}

