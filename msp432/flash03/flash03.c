
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );


extern const unsigned int flash_data_words;
extern const unsigned int flash_data[];


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
#define UCA0CTLW0   (UART0_BASE+0x00)
#define UCA0BRW     (UART0_BASE+0x06)
#define UCA0MCTLW   (UART0_BASE+0x08)
#define UCA0TXBUF   (UART0_BASE+0x0E)
#define UCA0IE      (UART0_BASE+0x1A)
#define UCA0IFG     (UART0_BASE+0x1C)

#define PORT_BASE 0x40004C00
#define PASEL0_L (PORT_BASE+0x0A)
#define PASEL1 PASEL1_L
#define PASEL1_L (PORT_BASE+0x0C)

#define WDTCTL   0x4000480C



//------------------------------------------------------------------------
void uart_init ( void )
{
    PUT8(PASEL1_L,GET8(PASEL1_L)&0xF3);
    PUT8(PASEL0_L,GET8(PASEL0_L)|0x0C);

    PUT16(UCA0CTLW0,0x0081);
    //PUT16(UCA0BRW,104); //12000000/115200 = 104
    PUT16(UCA0BRW,26); //3000000/115200 = 26
    PUT16(UCA0MCTLW,0x0000);
    PUT16(UCA0IE,0);
    PUT16(UCA0CTLW0,0x0081);
    PUT16(UCA0CTLW0,0x0080);
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

hexstrings(a); hexstring(d);
    
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
        if(rc==0x00000) break;
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

    PUT16(WDTCTL,0x5A84); //stop WDT

    //uart_init();
    hexstring(0x12345678);
    PUT32(FLCTL_BANK0_MAIN_WEPROT,0xFFFFFFFE);
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
    hexstring(GET32(FLCTL_PRG_CTLSTAT));
    hexstring(flash_data_words);
    PUT32(FLCTL_PRG_CTLSTAT,0xC1);
    for(ra=0;ra<flash_data_words;ra++)
    {
        write_word(ra<<2,flash_data[ra]);
    }
    PUT32(FLCTL_BANK0_MAIN_WEPROT,0xFFFFFFFF);

    for(ra=0;ra<flash_data_words;ra++)
    {
        hexstrings(ra<<2);
        hexstrings(GET32(ra<<2));
        hexstring(flash_data[ra]);
    }

    hexstring(0x12345678);
    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
