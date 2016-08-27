
//------------------------------------------------------------------------
//------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
extern void ASMDELAY ( unsigned int );

#define RCGCGPIO 0x400FE608
#define PRGPIO   0x400FEA08

#define CM4_BASE 0xE000E000
#define STCTRL      (CM4_BASE+0x010)
#define STRELOAD    (CM4_BASE+0x014)
#define STCURRENT   (CM4_BASE+0x018)

#define RCGCUART 0x400FE618
#define PRUART   0x400FEA18

#define GPIOA_BASE 0x40004000
#define GPIOAFSEL (GPIOA_BASE+0x420)
#define GPIOADEN  (GPIOA_BASE+0x51C)

#define UART0_BASE 0x4000C000
#define UARTDR   (UART0_BASE+0x000)
#define UARTFR   (UART0_BASE+0x018)
#define UARTIBRD (UART0_BASE+0x024)
#define UARTFBRD (UART0_BASE+0x028)
#define UARTLCRH (UART0_BASE+0x02C)
#define UARTCTL  (UART0_BASE+0x030)
#define UARTCC   (UART0_BASE+0xFC8)

#define SYSTEM_CONTROL 0x400FE000
#define RCC (SYSTEM_CONTROL+0x60)
#define RCC2 (SYSTEM_CONTROL+0x70)
#define RIS (SYSTEM_CONTROL+0x50)

#define GPIOA_BASE 0x40004000
#define GPIOB_BASE 0x40005000
#define GPIOC_BASE 0x40006000
#define GPIOD_BASE 0x40007000
#define GPIOE_BASE 0x40024000
#define GPIOF_BASE 0x40025000

#define GPIOBDATA   (GPIOB_BASE+0x000)
#define GPIOBDIR    (GPIOB_BASE+0x400)
#define GPIOBAFSEL  (GPIOB_BASE+0x420)
#define GPIOBODR    (GPIOB_BASE+0x50C)
#define GPIOBPUR    (GPIOB_BASE+0x510)
#define GPIOBPDR    (GPIOB_BASE+0x514)
#define GPIOBDEN    (GPIOB_BASE+0x51C)
#define GPIOBOCR    (GPIOB_BASE+0x524)

#define GPIOEDATAX  (GPIOE_BASE+0x000)
#define GPIOEDIR    (GPIOE_BASE+0x400)
#define GPIOEAFSEL  (GPIOE_BASE+0x420)
#define GPIOEODR    (GPIOE_BASE+0x50C)
#define GPIOEPUR    (GPIOE_BASE+0x510)
#define GPIOEPDR    (GPIOE_BASE+0x514)
#define GPIOEDEN    (GPIOE_BASE+0x51C)
#define GPIOEOCR    (GPIOE_BASE+0x524)

//------------------------------------------------------------------------
static void uart_init ( void )
{
    unsigned int ra;
    //16000000 / (16 * 115200 ) = 1000000 / 115200 = 8.6805
    //0.6805... * 64 = 43.5   ~ 44
    //use 8 and 44
    ra=GET32(RCGCUART);
    ra|=1; //UART0
    PUT32(RCGCUART,ra);
    ra=GET32(RCGCGPIO);
    ra|=1; //GPIOA
    PUT32(RCGCGPIO,ra);

    ra=GET32(GPIOAFSEL);
    ra|=3;
    PUT32(GPIOAFSEL,ra);
    ra=GET32(GPIOADEN);
    ra|=3;
    PUT32(GPIOADEN,ra);

    PUT32(UARTCTL,0x00000000);
    PUT32(UARTIBRD, 8);
    PUT32(UARTFBRD, 44);
    PUT32(UARTLCRH,0x00000060);
    PUT32(UARTCC,0x00000005); //PIOSC
    PUT32(UARTFR,0);
    PUT32(UARTCTL,0x00000301);
}
//------------------------------------------------------------------------
static void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET32(UARTFR)&0x80) break;
    }
    PUT32(UARTDR,x);
}
//------------------------------------------------------------------------
//static unsigned int uart_recv ( void )
//{
    //while(1)
    //{
        //if(GET32(UARTFR)&0x40) break;
    //}
    //return(GET32(UARTDR)&0xFF);;
//}
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
//------------------------------------------------------------------------
static void switch_to_80Mhz ( void )
{
    unsigned int ra;

//1. Bypass the PLL and system clock divider by setting the BYPASS bit and
//clearing the USESYS bit in the RCC register, thereby configuring the
//microcontroller to run off a "raw" clock source and allowing for the
//new PLL configuration to be validated before switching the system clock
//to the PLL.

    ra=GET32(RCC);
    ra|=1<<11; //BYPASS
    ra&=~(1<<22); //USESYS
    PUT32(RCC,ra);

//2. Select the crystal value (XTAL) and oscillator source (OSCSRC),
//and clear the PWRDN bit in RCC/RCC2. Setting the XTAL field
//automatically pulls valid PLL configuration data for the appropriate
//crystal, and clearing the PWRDN bit powers and enables the PLL and
//its output.

    //0x15  16mhz
    ra=GET32(RCC);
    ra&=~(0x1F<<6); //XTAL
    ra|=0x15<<6; //XTAL
    ra&=~(0x3<<4); //OSCSRC
    ra|=0x0<<4; //OSCSRC
    ra&=~(1<<13); //PWRDN
    PUT32(RCC,ra);

//3. Select the desired system divider (SYSDIV) in RCC/RCC2 and set the
//USESYS bit in RCC. The SYSDIV field determines the system frequency
//for the microcontroller.

    //0x1 80 mhz
    ra=GET32(RCC);
    ra&=~(0xF<<23); //SYSDIV
    ra|=0x1<<23; //SYSDIV
    ra|=0x1<<22; //USESYSDIV
    PUT32(RCC,ra);

//4. Wait for the PLL to lock by polling the PLLLRIS bit in the Raw
//Interrupt Status (RIS) register.

    while(1)
    {
        if(GET32(RIS)&(1<<6)) break;
    }

//5. Enable use of the PLL by clearing the BYPASS bit in RCC/RCC2.
    ra=GET32(RCC);
    ra&=~(1<<11); //BYPASS
    PUT32(RCC,ra);
}














    //1.07 PB4    SPI CLK
    //1.02 PB5    LCD Power Control
    //2.06 PB7    SPI MOSI

    //1.05 PE4    LCD Enable
    //1.06 PE5    SPI CS

#define SCK     4
#define LCDPC   5
#define MOSI    7

#define LCDE    4
#define CS      5

#define SPI_CS_ON   (PUT32(GPIOEDATAX|( (1<<CS) <<2),0xFF))
#define SPI_CS_OFF  (PUT32(GPIOEDATAX|( (1<<CS) <<2),0x00))

#define SPI_CLK_ON  (PUT32(GPIOBDATA|( (1<<SCK) <<2),0xFF))
#define SPI_CLK_OFF (PUT32(GPIOBDATA|( (1<<SCK) <<2),0x00))

#define SPI_MOSI_ON  (PUT32(GPIOBDATA|( (1<<MOSI) <<2),0xFF))
#define SPI_MOSI_OFF (PUT32(GPIOBDATA|( (1<<MOSI) <<2),0x00))

//-------------------------------------------------------------------
static void spi_delay ( void )
{
    //unsigned short ra;
    //for(ra=0;ra<1000;ra++) dummy(ra);
}
//-------------------------------------------------------------------
static void spi_write_byte ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x80;ca;ca>>=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
    }
}
//-------------------------------------------------------------------
static void spi_write_sharp_address ( unsigned char d )
{
    unsigned char ca;

    for(ca=0x01;;ca<<=1)
    {
        if(ca&d) SPI_MOSI_ON; else SPI_MOSI_OFF;
        spi_delay();
        SPI_CLK_ON;
        spi_delay();
        SPI_CLK_OFF;
        spi_delay();
        if(ca==0x80) break;
    }
}
//-------------------------------------------------------------------
static void clear_screen ( void )
{
    SPI_CS_ON;
    spi_delay();
    spi_write_byte(0x20);
    spi_write_byte(0x00);
    spi_delay();
    SPI_CS_OFF;
}


//------------------------------------------------------------------------
void notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned char d;

    switch_to_80Mhz();

    uart_init();
    hexstring(0x87654321);

    //1.02 PB5    LCD Power Control
    //1.05 PE4    LCD Enable
    //1.06 PE5    SPI CS
    //1.07 PB4    SPI CLK
    //2.02 PB2    Extern COM Inversion
    //2.06 PB7    SPI MOSI


    //1.07 PB4    SPI CLK
    //1.02 PB5    LCD Power Control
    //2.06 PB7    SPI MOSI

    //1.05 PE4    LCD Enable
    //1.06 PE5    SPI CS

    ra=GET32(RCGCGPIO);
    ra|=1<<1; //GPIOB
    ra|=1<<4; //GPIOE
    PUT32(RCGCGPIO,ra);
    while(1)
    {
        if(GET32(PRGPIO)&(1<<1)) break;
    }
    while(1)
    {
        if(GET32(PRGPIO)&(1<<4)) break;
    }

    ra=GET32(GPIOBAFSEL);
    ra&=~((1<<SCK)|(1<<LCDPC)|(1<<MOSI));
    PUT32(GPIOBAFSEL,ra);

    ra=GET32(GPIOBDIR);
    ra|=((1<<SCK)|(1<<LCDPC)|(1<<MOSI));
    PUT32(GPIOBDIR,ra);

    ra=GET32(GPIOBODR);
    ra&=~((1<<SCK)|(1<<LCDPC)|(1<<MOSI));
    PUT32(GPIOBODR,ra);

    ra=GET32(GPIOBDEN);
    ra|=((1<<SCK)|(1<<LCDPC)|(1<<MOSI));
    PUT32(GPIOBDEN,ra);


    ra=GET32(GPIOEAFSEL);
    ra&=~((1<<LCDE)|(1<<CS));
    PUT32(GPIOEAFSEL,ra);

    ra=GET32(GPIOEDIR);
    ra|=((1<<LCDE)|(1<<CS));
    PUT32(GPIOEDIR,ra);

    ra=GET32(GPIOEODR);
    ra&=~((1<<LCDE)|(1<<CS));
    PUT32(GPIOEODR,ra);

    ra=GET32(GPIOEDEN);
    ra|=((1<<LCDE)|(1<<CS));
    PUT32(GPIOEDEN,ra);


    PUT32(GPIOBDATA|( (1<<LCDPC) <<2),0x00);
    PUT32(GPIOEDATAX|( (1<<LCDE) <<2),0x00);
    ASMDELAY(10000000);
    PUT32(GPIOBDATA|( (1<<LCDPC) <<2),0xFF);
    PUT32(GPIOEDATAX|( (1<<LCDE) <<2),0xFF);
    ASMDELAY(10000000);
    PUT32(GPIOBDATA|( ((1<<SCK)|(1<<MOSI)) <<2),0x00);
    PUT32(GPIOEDATAX|( ((1<<CS)) <<2),0x00);

    clear_screen();

    for(d=0xF0;;d++)
    {
        for(ra=1;ra<=96;ra++)
        {
            spi_delay();
            SPI_CS_ON;
            spi_delay();
            spi_write_byte(0x80);
            spi_write_sharp_address(ra&0xFF);
            for(rb=0;rb<12;rb++)
            {
                spi_write_byte(d);
            }
            spi_write_byte(0x00);
            spi_write_byte(0x00);
            spi_delay();
            SPI_CS_OFF;
            spi_delay();
        }
    }
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
