
//------------------------------------------------------------------------
//------------------------------------------------------------------------

void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );


unsigned int GET_APSR ( void );
unsigned int GET_IPSR ( void );
unsigned int GET_EPSR ( void );
unsigned int GET_CONTROL ( void );
unsigned int SP_PROCESS ( void );
unsigned int SP_MAIN ( void );
unsigned int GET_SP ( void );
void SETPSP ( unsigned int );
void SETCONTROL ( unsigned int );

//------------------------------------------------------------------------

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

//------------------------------------------------------------------------

//P1.2 UCA0RXD P1SEL1 2b01
//P1.3 UCA0TXD P1SEL1 2b01

//P3.2 UCA2RXD P2SEL1 2b01 (default)
//P3.3 UCA2TXD P2SEL1 2b01 (default

//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET16(UCA0IFG)&2) break;
    }
    PUT16(UCA0TXBUF,x);
}
//------------------------------------------------------------------------
static unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET16(UCA0IFG)&1) break;
    }
    return(GET16(UCA0RXBUF));
}
//------------------------------------------------------------------------
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
//------------------------------------------------------------------------
static void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
extern void hang ( void );
void iwait ( void);
void handler ( void )
{
    unsigned int ra;


    hexstring(GET_APSR());
    hexstring(GET_IPSR());
    hexstring(GET_EPSR());
    hexstring(GET_CONTROL());
    hexstring(SP_PROCESS());
    hexstring(SP_MAIN());
    hexstring(GET_SP());
    ra=GET_SP();
    for(;ra<0x20001000;ra+=4)
    {
        hexstrings(ra);
        hexstring(GET32(ra));
    }
    ra=SP_PROCESS();
    for(;ra<0x20000D00;ra+=4)
    {
        hexstrings(ra);
        hexstring(GET32(ra));
    }

    iwait();
}
//------------------------------------------------------------------------
int notmain ( void )
{
    unsigned int ra;

    PUT16(WDTCTL,0x5A84); //stop WDT

    uart_init(); //see readme
    for(ra=0;ra<1000;ra++) dummy(ra);
    hexstring(0x12345678);

    hexstring(GET_APSR());
    hexstring(GET_IPSR());
    hexstring(GET_EPSR());
    hexstring(SP_PROCESS());
    SETPSP(0x20000D00);
    hexstring(SP_PROCESS());
    hexstring(GET_CONTROL());
    ra=GET_CONTROL();
    //SETCONTROL(ra|(1<<1));
    hexstring(GET_CONTROL());
    hexstring(SP_MAIN());
    hexstring(GET_SP());
    ra=GET_SP();
    for(;ra<0x20000D00;ra+=4)
    {
        hexstrings(ra);
        hexstring(GET32(ra));
    }

    PUT32(SYST_CSR,4);
    PUT32(SYST_RVR,6000000-1);
    PUT32(SYST_CVR,6000000-1);
    PUT32(SYST_CSR,7);
    iwait();

    return(0);
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
