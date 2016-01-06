
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );
void ASMDELAY ( unsigned int );

#define PORT_BASE 0x40004C00

#define PAOUT PAOUT_L
#define PAOUT_L (PORT_BASE+0x02)
#define PAOUT_H (PORT_BASE+0x03)
#define PADIR PADIR_L
#define PADIR_L (PORT_BASE+0x04)
#define PADIR_H (PORT_BASE+0x05)

#define PBOUT PBOUT_L
#define PBOUT_L (PORT_BASE+0x22)
#define PBOUT_H (PORT_BASE+0x23)
#define PBDIR PBDIR_L
#define PBDIR_L (PORT_BASE+0x24)
#define PBDIR_H (PORT_BASE+0x25)

#define P4IN  (PORT_BASE+0x21)
#define P4OUT (PORT_BASE+0x23)
#define P4DIR (PORT_BASE+0x25)


#define SYST_CSR 0xE000E010
#define SYST_RVR 0xE000E014
#define SYST_CVR 0xE000E018
#define SYST_CALIB 0xE000E01C

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

static unsigned char sdata[16];
static unsigned char rdata[16];

//------------------------------------------------------------------------

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

//-------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
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
//-------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}

void delay ( void )
{
    unsigned int ra;
    for(ra=0;ra<10;ra++)
    {
        while(1)
        {
            if(GET32(SYST_CSR)&0x10000) break;
        }
    }
}

#define CS   (1<<0)
#define SCK  (1<<1)
#define MOSI (1<<2)
#define MISO (1<<3)

static unsigned char sportout;

static void spi_delay ( void )
{
}

static void spi_cs ( unsigned char x )
{
    if(x) sportout|=CS;
    else  sportout&=~CS;
    PUT8(P4OUT,sportout);
}
static void spi_clk ( unsigned char x )
{
    if(x) sportout|=SCK;
    else  sportout&=~SCK;
    PUT8(P4OUT,sportout);
}
static void spi_mosi ( unsigned char x )
{
    if(x) sportout|=MOSI;
    else  sportout&=~MOSI;
    PUT8(P4OUT,sportout);
}
static unsigned int spi_miso ( void )
{
    if(GET8(P4IN)&MISO) return(1);
    return(0);
}

static int spi_command ( unsigned int len, unsigned int rlen )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    spi_cs(0);
    spi_delay();
    for(rc=0;rc<len;rc++)
    {
        for(rb=sdata[rc],ra=0;ra<8;ra++,rb<<=1)
        {
            spi_mosi((rb>>7)&1);
            spi_delay();
            spi_clk(1);
            spi_delay();
            spi_clk(0);
            spi_delay();
        }
    }
    for(ra=0;ra<32;ra++)
    {
        spi_delay();
        spi_clk(1);
        spi_delay();
        rb=spi_miso();
        spi_clk(0);
        spi_delay();
        if(rb==0) break;
    }
    if(rb)
    {
        spi_cs(1);
        spi_delay();
        for(ra=0;ra<rlen;ra++) rdata[ra]=0xFF;
        return(1);
    }
    //rb=0;
    for(ra=0;ra<7;ra++)
    {
        spi_delay();
        spi_clk(1);
        spi_delay();
        rb<<=1;
        if(spi_miso()) rb|=1;
        spi_clk(0);
        spi_delay();
    }
    rdata[0]=rb;
    for(rc=1;rc<rlen;rc++)
    {
        rb=0;
        for(ra=0;ra<8;ra++)
        {
            spi_delay();
            spi_clk(1);
            spi_delay();
            rb<<=1;
            if(spi_miso()) rb|=1;
            spi_clk(0);
            spi_delay();
        }
        rdata[rc]=rb;
    }
    spi_cs(1);
    spi_delay();
    return(0);
}

static void clock_some ( unsigned int x )
{
    unsigned int ra;
    spi_mosi(1);
    for(ra=0;ra<x;ra++)
    {
        spi_clk(1);
        spi_delay();
        spi_clk(0);
        spi_delay();
    }
}

static int cmd0 ( void )
{
    unsigned int ra;

    ra=0;
    sdata[ra++]=0x40;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x95;
    spi_command(ra,1);
    for(ra=0;ra<1;ra++) hexstring(rdata[ra]);
    if(rdata[0]==0x01) return(0);
    return(1);
}
static int cmd8 ( void )
{
    unsigned int ra;

    ra=0;
    sdata[ra++]=0x48;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x01;
    sdata[ra++]=0xAA;
    sdata[ra++]=0x87;
    spi_command(ra,5);
    for(ra=0;ra<5;ra++) hexstring(rdata[ra]);
    if(rdata[0]==0x01) return(0);
    return(1);
}
static int cmd58 ( void )
{
    unsigned int ra;

    ra=0;
    sdata[ra++]=0x7A;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0xFD;
    spi_command(ra,5);
    for(ra=0;ra<5;ra++) hexstring(rdata[ra]);
    if(rdata[0]==0x01) return(0);
    return(1);
}
static int cmd55 ( void )
{
    unsigned int ra;

    ra=0;
    sdata[ra++]=0x77;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x65;
    spi_command(ra,1);
    for(ra=0;ra<1;ra++) hexstring(rdata[ra]);
    if(rdata[0]==0x01) return(0);
    return(1);
}
static int cmd41 ( void )
{
    unsigned int ra;

    ra=0;
    sdata[ra++]=0x69;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0x00;
    sdata[ra++]=0xE5;
    spi_command(ra,5);
    for(ra=0;ra<5;ra++) hexstring(rdata[ra]);
    if(rdata[0]==0x01) return(0);
    return(1);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;


    PUT16(WDTCTL,0x5A84); //stop WDT
    uart_init();
    hexstring(0x12345678);

    //P4.0 CE out
    //P4.1 CLK out
    //P4.2 MOSI out
    //P4.3 MISO in

    //ra=GET8(P4DIR);
    //ra&=0xF7;
    //ra|=0x07;
    ra=0x07;
    PUT8(P4DIR,ra);
    //drive or pull up
    sportout=CS|MISO;
    PUT8(P4OUT,sportout);
    spi_delay();


    //spi_cs(0);
    //spi_delay();
    //for(ra=0;ra<74;ra++)
    //{
        //spi_clk(1);
        //spi_delay();
        //spi_clk(0);
        //spi_delay();
    //}
    //spi_cs(1);
    //for(ra=0;ra<16;ra++)
    //{
        //spi_clk(1);
        //spi_delay();
        //spi_clk(0);
        //spi_delay();
    //}

    clock_some(80);
    hexstring(0xCACA0000);
    if(cmd0()) return(1);
    ASMDELAY(1000000);
    hexstring(0xCACA0008);
    cmd8();
    hexstring(0xCACA0058);
    if(cmd58()) return(1);
//CACA0058
//00000001
//00000000
//000000FF
//00000080
//00000000
    for(ra=0;ra<8;ra++)
    {
        hexstring(0xDADA0000|ra);
        rb=0;
        hexstring(0xCACA0055);
        cmd55();
        if(rdata[0]!=0x00) rb++;
        clock_some(8);
        hexstring(0xCACA0041);
        cmd41();
        if(rdata[0]!=0x00) rb++;
        clock_some(8);
        if(rb==0) break;
    }
    if(rb) return(1);


    return(0);
}
