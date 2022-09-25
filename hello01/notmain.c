
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void dummy ( unsigned int );

#define GPIOCBASE 0x02000000
#define GPIOC_CFG (GPIOCBASE+0x0060)
#define GPIOC_DATA (GPIOCBASE+0x0070)

int notmain ( void )
{
    unsigned int rx;

    rx=GET32(GPIOC_CFG);
    rx&=~(0x000000F0);
    rx|= (0x00000010);
    PUT32(GPIOC_CFG,rx);

    while(1)
    {
        PUT32(GPIOC_DATA,2);
        for(rx=0;rx<0x1000000;rx++) dummy(rx);
        PUT32(GPIOC_DATA,0);
        for(rx=0;rx<0x1000000;rx++) dummy(rx);
    }
    return(0);
}
