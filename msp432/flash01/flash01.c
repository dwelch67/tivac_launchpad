
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

#define FLCTL_BASE 0x40011000

#define FLCTL_PRG_CTLSTAT       (FLCTL_BASE+0x050)
#define FLCTL_PRGBRST_CTLSTAT   (FLCTL_BASE+0x054)
#define FLCTL_ERASE_CTLSTAT     (FLCTL_BASE+0x0A0)
#define FLCTL_BANK0_INFO_WEPROT (FLCTL_BASE+0x0B0)
#define FLCTL_BANK0_MAIN_WEPROT (FLCTL_BASE+0x0B4)
#define FLCTL_BANK1_INFO_WEPROT (FLCTL_BASE+0x0C0)
#define FLCTL_BANK1_MAIN_WEPROT (FLCTL_BASE+0x0C4)

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
//------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//------------------------------------------------------------------------
int notmain ( void )
{
    hexstring(0x12345678);
    hexstring(GET32(FLCTL_BANK0_INFO_WEPROT));
    hexstring(GET32(FLCTL_BANK0_MAIN_WEPROT));
    hexstring(GET32(FLCTL_BANK1_INFO_WEPROT));
    hexstring(GET32(FLCTL_BANK1_MAIN_WEPROT));
    hexstring(GET32(FLCTL_PRG_CTLSTAT));
    hexstring(GET32(FLCTL_PRGBRST_CTLSTAT));
    hexstring(GET32(FLCTL_ERASE_CTLSTAT));
    hexstring(0x12345678);
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
