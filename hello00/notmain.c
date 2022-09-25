
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define UART0BASE 0x02500000
#define UART0TX (UART0BASE+0x0000)
#define UART0STATUS (UART0BASE+0x007C)


int notmain ( void )
{
    unsigned int rx;

    for(rx=0x30;;rx++)
    {
        rx&=0x37;
        while(1)
        {
            if(GET32(UART0STATUS)&(1<<1)) break;
        }
        PUT32(UART0TX,rx);
    }
    return(0);
}
