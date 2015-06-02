
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
#define UCA0CTLW0   (UART0_BASE+0x00)
#define UCA0BRW     (UART0_BASE+0x06)
#define UCA0MCTLW   (UART0_BASE+0x08)
#define UCA0TXBUF   (UART0_BASE+0x0E)
#define UCA0IE      (UART0_BASE+0x1A)
#define UCA0IFG     (UART0_BASE+0x1C)
#define PASEL0 PASEL0_L
#define PASEL0_L (PORT_BASE+0x0A)
#define PASEL0_H (PORT_BASE+0x0B)
#define PASEL1 PASEL1_L
#define PASEL1_L (PORT_BASE+0x0C)
#define PASEL1_H (PORT_BASE+0x0D)

#define PORT_BASE 0x40004C00

//------------------------------------------------------------------------
void uart_init ( void )
{
    ////PUT8(PASEL0_L,GET8(PASEL0_L)&0xF3);
    ////PUT8(PASEL1_L,GET8(PASEL1_L)|0x0C);
    //PUT16(UCA0CTLW0,0x0081);
    //PUT16(UCA0BRW,6);
    //PUT16(UCA0MCTLW,0x2081);
    //PUT16(UCA0IE,0);
    //PUT16(UCA0CTLW0,0x0081);
    //PUT16(UCA0CTLW0,0x0080);
}
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

    uart_init();
    for(ra=0;;ra++)
    {
        uart_send(0x30+(ra&7));
    }
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
