
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
#define FLCTL_ERASE_SECTADDR    (FLCTL_BASE+0x0A4)
#define FLCTL_BANK0_INFO_WEPROT (FLCTL_BASE+0x0B0)
#define FLCTL_BANK0_MAIN_WEPROT (FLCTL_BASE+0x0B4)
#define FLCTL_BANK1_INFO_WEPROT (FLCTL_BASE+0x0C0)
#define FLCTL_BANK1_MAIN_WEPROT (FLCTL_BASE+0x0C4)
#define FLCTL_IFG               (FLCTL_BASE+0x0F0)
#define FLCTL_IE                (FLCTL_BASE+0x0F4)
#define FLCTL_CLRIFG            (FLCTL_BASE+0x0F8)
#define UART0_BASE 0x40001000
#define UCA0TXBUF   (UART0_BASE+0x0E)
#define UCA0IFG     (UART0_BASE+0x1C)

//------------------------------------------------------------------------
void uart_init ( void )
{
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
void uart_sendx ( unsigned int x )
{
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
void write_word ( unsigned int a, unsigned int d )
{
    unsigned int ra,rb,rc;
    PUT32(a,d);
    ra=0;
    while(1)
    {
        rb=GET32(FLCTL_PRG_CTLSTAT);
        rc=rb&0x30000;
        if(rc!=ra)
        {
            ra=rc;
            hexstring(ra);
        }
        if(rc==0x30000) break;
    }
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int re;

    uart_init();
    hexstring(0x12345678);
    PUT32(FLCTL_BANK0_MAIN_WEPROT,0xFFFFFFFE);
    PUT32(FLCTL_ERASE_CTLSTAT,0x00000000); //??
    PUT32(FLCTL_ERASE_CTLSTAT,0x00080000);
    PUT32(FLCTL_CLRIFG,0xFFFFFFFF);
    PUT32(FLCTL_IE,0x00000000);
    PUT32(FLCTL_ERASE_SECTADDR,0x00000000);
    PUT32(FLCTL_ERASE_CTLSTAT,0x00000001); //start erasing
    ra=0;
    rd=0;
    while(1)
    {
        rb=GET32(FLCTL_ERASE_CTLSTAT);
        re=GET32(FLCTL_IFG);
        rc=rb&0x30000;
        if((rc!=ra)||(rd!=re))
        {
            ra=rc;
            rd=re;
            hexstrings(ra); hexstring(rd);
        }
        if(rc==0x30000) break;
    }
    if(rb&0x40000) hexstring(0xBAD);
    PUT32(FLCTL_ERASE_CTLSTAT,0x00080000);
    PUT32(FLCTL_BANK0_MAIN_WEPROT,0xFFFFFFFF);
    hexstring(0x12345678);
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
