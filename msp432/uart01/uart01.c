
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

//------------------------------------------------------------------------

#define UART0_BASE 0x40001000
#define UCA0TXBUF   (UART0_BASE+0x0E)
#define UCA0IFG     (UART0_BASE+0x1C)

//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET16(UCA0IFG)&2) break;
    }
    PUT16(UCA0TXBUF,x);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    for(ra=0;;ra++)
    {
        uart_send(0x30+(ra&7));
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
