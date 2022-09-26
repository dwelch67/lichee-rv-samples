
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

#define UART0BASE 0x02500000
#define UART0TX (UART0BASE+0x0000)
#define UART0RX (UART0BASE+0x0000)
#define UART0USR (UART0BASE+0x007C)

void uart_init ( void )
{
}
void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET32(UART0USR)&(1<<1)) break;
    }
    PUT32(UART0TX,x);
}
unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(UART0USR)&(1<<3)) break;
    }
    return(GET32(UART0RX));
}
void uart_flush ( void )
{
    while(1)
    {
        if(GET32(UART0USR)&(1<<2)) break;
    }
}
void hexstrings ( unsigned int d )
{
    //unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    rb=32;
    while(1)
    {
        rb-=4;
        rc=(d>>rb)&0xF;
        if(rc>9) rc+=0x37; else rc+=0x30;
        uart_send(rc);
        if(rb==0) break;
    }
    uart_send(0x20);
}
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
