
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

//------------------------------------------------------------------------

static unsigned int dig_t1;
static unsigned int dig_t2;
static unsigned int dig_t3;


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

void spi_write_byte ( unsigned int add, unsigned int data )
{
    unsigned int ra;
    unsigned int rb;

    rb=add;
    PUT8(P4OUT,MOSI|MISO|SCK|CS); 
    PUT8(P4OUT,MOSI|MISO|SCK); //drop cs
    
    //write bit
    PUT8(P4OUT,MOSI|MISO|SCK); 
    PUT8(P4OUT,MISO); 
    PUT8(P4OUT,MISO|SCK);
    //address bits
    for(ra=0;ra<7;ra++,rb<<=1)
    {
        if(rb&0x40)
        {
            PUT8(P4OUT,MOSI|MISO);
            PUT8(P4OUT,MOSI|MISO|SCK);
        }
        else
        {
            PUT8(P4OUT,MISO);
            PUT8(P4OUT,MISO|SCK);
        }
    }
    //data bits
    rb=data;
    for(ra=0;ra<8;ra++,rb<<=1)
    {
        if(rb&0x80)
        {
            PUT8(P4OUT,MOSI|MISO);
            PUT8(P4OUT,MOSI|MISO|SCK);
        }
        else
        {
            PUT8(P4OUT,MISO);
            PUT8(P4OUT,MISO|SCK);
        }
    }
    PUT8(P4OUT,MOSI|MISO|SCK|CS); //raise cs
}

unsigned int spi_read_byte ( unsigned int add )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int data;

    data=0;
    rb=add;
    PUT8(P4OUT,MOSI|MISO|SCK|CS); 
    PUT8(P4OUT,MOSI|MISO|SCK); //drop cs
    
    //read bit
    PUT8(P4OUT,MOSI|MISO|SCK); 
    PUT8(P4OUT,MOSI|MISO); 
    PUT8(P4OUT,MOSI|MISO|SCK);
    //address bits
    for(ra=0;ra<7;ra++,rb<<=1)
    {
        if(rb&0x40)
        {
            PUT8(P4OUT,MOSI|MISO);
            PUT8(P4OUT,MOSI|MISO|SCK);
        }
        else
        {
            PUT8(P4OUT,MISO);
            PUT8(P4OUT,MISO|SCK);
        }
    }
    //data bits
    data=0;
    for(ra=0;ra<8;ra++)
    {
        PUT8(P4OUT,MISO);
        data<<=1;
        if(GET8(P4IN)&MISO) data|=1;
        PUT8(P4OUT,MISO|SCK);
    }
    PUT8(P4OUT,MOSI|MISO|SCK|CS); //raise cs
    return(data);
}

unsigned int spi_read_three ( unsigned int add )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int data;

    data=0;
    rb=add;
    PUT8(P4OUT,MOSI|MISO|SCK|CS); 
    PUT8(P4OUT,MOSI|MISO|SCK); //drop cs
    
    //read bit
    PUT8(P4OUT,MOSI|MISO|SCK); 
    PUT8(P4OUT,MOSI|MISO); 
    PUT8(P4OUT,MOSI|MISO|SCK);
    //address bits
    for(ra=0;ra<7;ra++,rb<<=1)
    {
        if(rb&0x40)
        {
            PUT8(P4OUT,MOSI|MISO);
            PUT8(P4OUT,MOSI|MISO|SCK);
        }
        else
        {
            PUT8(P4OUT,MISO);
            PUT8(P4OUT,MISO|SCK);
        }
    }
    //data bits
    data=0;
    for(ra=0;ra<24;ra++)
    {
        PUT8(P4OUT,MISO);
        data<<=1;
        if(GET8(P4IN)&MISO) data|=1;
        PUT8(P4OUT,MISO|SCK);
    }
    PUT8(P4OUT,MOSI|MISO|SCK|CS); //raise cs
    return(data);
}

unsigned int compensate_temp ( unsigned int raw )
{
    unsigned int var1;
    unsigned int var2;
    unsigned int temp;
    unsigned int t_fine;
    var1 = ((((raw>>3) - (dig_t1<<1))) * (dig_t2)) >> 11;
    var2 = (((((raw>>4) - (dig_t1)) * ((raw>>4) - (dig_t1))) >> 12) * (dig_t3)) >> 14;
    t_fine = var1 + var2;
    temp = (t_fine * 5 + 128) >> 8;
    return(temp);
}

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int a,b,c,d,e,f;
    
    PUT16(WDTCTL,0x5A84); //stop WDT
    uart_init();
    hexstring(0x12345678);

    //4.0 CSB out
    //4.1 SCK/SCL out
    //4.2 SDI/SDA/MOSI out
    //4.3 SDO/MISO in

    ra=GET8(P4DIR);
    ra|=0x07;
    ra&=0xF7;
    PUT8(P4DIR,ra);
    //PUT8(PAREN_L,GET8(PAREN_L)|0x12); //enable pull up/down
    //PUT8(PAOUT_L,GET8(PAOUT_L)|0x12); //pull up

    PUT8(P4OUT,CS|SCK|MISO|MOSI);
    
    ra=spi_read_byte(0xD0); hexstring(ra);
    spi_write_byte(0xE0,0xB6);
    ASMDELAY(1000000);
    ra=spi_read_byte(0xD0); hexstring(ra);

    dig_t1=spi_read_byte(0x89);
    dig_t1<<=8;
    dig_t1|=spi_read_byte(0x88);
    dig_t2=spi_read_byte(0x8B);
    dig_t2<<=8;
    dig_t2|=spi_read_byte(0x8A);
    dig_t3=spi_read_byte(0x8D);
    dig_t3<<=8;
    dig_t3|=spi_read_byte(0x8C);
    
    ra=spi_read_byte(0xF4); hexstring(ra);
    spi_write_byte(0xF4,0x83);
    ra=spi_read_byte(0xF4); hexstring(ra);


    
    ra=spi_read_byte(0xF3); hexstring(ra);
    while(1)
    {
        ra=spi_read_byte(0xF3); hexstrings(ra);
        ra=spi_read_three(0xFA); hexstrings(ra);
        rb=compensate_temp(ra>>4);
        hexstrings(rb);
        //to decimal without dividingce
        rc=rb;
        for(a=0;a<10;a++) if(rc<(a*1000)) break;
        a--;
        rc-=(a*1000);
        for(b=0;b<10;b++) if(rc<(b*100)) break;
        b--;
        rc-=(b*100);
        for(c=0;c<10;c++) if(rc<(c*10)) break;
        c--;
        rc-=(c*10);
        d=rc;
        uart_send(0x30+a);
        uart_send(0x30+b);
        uart_send('.');
        uart_send(0x30+c);
        uart_send(0x30+d);
        //uart_send(0x0D);
        //uart_send(0x0A);
        uart_send(0x20);
        rc=(18*rb)+32000;
        rb=rc;
        for(a=0;a<10;a++) if(rc<(a*100000)) break;
        a--;
        rc-=(a*100000);
        for(b=0;b<10;b++) if(rc<(b*10000)) break;
        b--;
        rc-=(b*10000);
        for(c=0;c<10;c++) if(rc<(c*1000)) break;
        c--;
        rc-=(c*1000);
        for(d=0;d<10;d++) if(rc<(d*100)) break;
        d--;
        rc-=(d*100);
        for(e=0;e<10;e++) if(rc<(e*10)) break;
        e--;
        rc-=(e*10);
        f=rc;
        uart_send(0x30+a);
        uart_send(0x30+b);
        uart_send(0x30+c);
        uart_send('.');
        uart_send(0x30+d);
        uart_send(0x30+e);
        uart_send(0x30+f);
        uart_send(0x0D);
        

        
    }
    hexstring(0x12345678);

    return(0);
}
