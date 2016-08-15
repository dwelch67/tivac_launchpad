
//-------------------------------------------------------------------
//-------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

unsigned int GET_SP ( void );

//-------------------------------------------------------------------

#define SYST_CSR 0xE000E010
#define SYST_RVR 0xE000E014
#define SYST_CVR 0xE000E018
#define SYST_CALIB 0xE000E01C


#define UART0_BASE 0x40001000
#define UCA0CTLW0   (UART0_BASE+0x00)
#define UCA0BRW     (UART0_BASE+0x06)
#define UCA0MCTLW   (UART0_BASE+0x08)
#define UCA0RXBUF   (UART0_BASE+0x0C)
#define UCA0TXBUF   (UART0_BASE+0x0E)
#define UCA0IE      (UART0_BASE+0x1A)
#define UCA0IFG     (UART0_BASE+0x1C)

#define PORT_BASE 0x40004C00
#define PASEL0_L (PORT_BASE+0x0A)
#define PASEL1 PASEL1_L
#define PASEL1_L (PORT_BASE+0x0C)


#define P3SEL0_L (PORT_BASE+0x2A)
#define P3SEL1_L (PORT_BASE+0x2C)

#define WDTCTL   0x4000480C

volatile int next_sp;
unsigned int sp_table[8];
volatile unsigned int tx0;
volatile unsigned int tx1;
volatile unsigned int tx2;
volatile unsigned int tx3;

//-------------------------------------------------------------------

//P1.2 UCA0RXD P1SEL1 2b01
//P1.3 UCA0TXD P1SEL1 2b01

//P3.2 UCA2RXD P2SEL1 2b01 (default)
//P3.3 UCA2TXD P2SEL1 2b01 (default

//-------------------------------------------------------------------
static void uart_init ( void )
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
//-------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET16(UCA0IFG)&2) break;
    }
    PUT16(UCA0TXBUF,x);
}
//-------------------------------------------------------------------
//static unsigned int uart_recv ( void )
//{
    //while(1)
    //{
        //if(GET16(UCA0IFG)&1) break;
    //}
    //return(GET16(UCA0RXBUF));
//}
//-------------------------------------------------------------------
static void hexstrings ( unsigned int d )
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
//-------------------------------------------------------------------
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//-------------------------------------------------------------------
extern void hang ( void );

void handler ( void )
{
    PUT32(SYST_CSR,4);
    hexstring(0x11223344);
    hang();
}
//-------------------------------------------------------------------
unsigned int systicker ( unsigned int sp )
{
    unsigned int ra;

    GET32(SYST_CSR);
    ra=next_sp;
    if(ra<4)
    {
        sp_table[ra]=sp;
        ra=(ra+1)&3;
    }
    else
    {
        ra=0;
    }
    next_sp=ra;
    return(sp_table[ra]);
}
//-------------------------------------------------------------------
void setup_thread ( unsigned int tx, unsigned int add, unsigned int sp )
{
    unsigned int ra;
    unsigned int rb;

    sp-=64;
    rb=sp;
    for(ra=0;ra<13;ra++)
    {
        PUT32(rb,0);
        rb+=4;
    }
    PUT32(rb,(unsigned int)hang); rb+=4;
    PUT32(rb,add&0xFFFFFFFE); rb+=4;
    PUT32(rb,0x01000000); rb+=4;
    sp_table[tx]=sp;
}
//-------------------------------------------------------------------
void task0 ( void )
{
    while(1) tx0+=1;
}
//-------------------------------------------------------------------
void task1 ( void )
{
    while(1) tx1+=2;
}
//-------------------------------------------------------------------
void task2 ( void )
{
    while(1) tx2+=3;
}
//-------------------------------------------------------------------
void task3 ( void )
{
    while(1)
    {
        tx3+=4;
        hexstrings(tx0);
        hexstrings(tx1);
        hexstrings(tx2);
        hexstring(tx3);
    }
}
//-------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    PUT16(WDTCTL,0x5A84); //stop WDT

    uart_init();
    hexstring(0x12345678);

    tx0=0;
    tx1=0;
    tx2=0;
    tx3=0;

    setup_thread(0,(unsigned int)task0,0x2000F000);
    setup_thread(1,(unsigned int)task1,0x2000E000);
    setup_thread(2,(unsigned int)task2,0x2000D000);
    setup_thread(3,(unsigned int)task3,0x2000C000);

    next_sp=4;
    for(ra=0;ra<4;ra++) hexstring(sp_table[ra]);
    for(rc=0;rc<4;rc++)
    {
        hexstring(sp_table[rc]);
        for(ra=0;ra<16;ra++)
        {
            rb=sp_table[rc]+(ra<<2);
            hexstrings(rb);
            hexstring(GET32(rb));
        }
    }

    PUT32(SYST_CSR,4);
    PUT32(SYST_RVR,1111);
    PUT32(SYST_CVR,1111);
    PUT32(SYST_CSR,7);

    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
