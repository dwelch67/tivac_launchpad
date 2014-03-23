
//------------------------------------------------------------------------
//------------------------------------------------------------------------

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

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
#define GPIODEN   (GPIOA_BASE+0x51C)

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

//------------------------------------------------------------------------
void uart_init ( void )
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
    ra=GET32(GPIODEN);
    ra|=3;
    PUT32(GPIODEN,ra);

    PUT32(UARTCTL,0x00000000);
    PUT32(UARTIBRD, 8);
    PUT32(UARTFBRD, 44);
    PUT32(UARTLCRH,0x00000060);
    PUT32(UARTCC,0x00000005); //PIOSC
    PUT32(UARTFR,0);
    PUT32(UARTCTL,0x00000301);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET32(UARTFR)&0x80) break;
    }
    PUT32(UARTDR,x);
}
//------------------------------------------------------------------------
unsigned int uart_recv ( void )
{
    while(1)
    {
        if(GET32(UARTFR)&0x40) break;
    }
    return(GET32(UARTDR)&0xFF);;
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
void switch_to_80Mhz ( void )
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
//------------------------------------------------------------------------
void dowait ( void )
{
    unsigned int rb;

    rb=0;
    while(1)
    {
        if(GET32(STCTRL)&0x10000)
        {
            rb++;
            if(rb>=160) break;
        }
    }
}
//------------------------------------------------------------------------
void notmain ( void )
{
    unsigned int ra;

    switch_to_80Mhz();

    uart_init();
    hexstring(0x87654321);
    hexstring(0x12345678);

    //Cortex-M4 systick timer init
    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,1000000-1);
    PUT32(STCURRENT,0); //value is a dont care
    PUT32(STCTRL,0x00000005);

    while(1)
    {
        uart_send(0x55);
        dowait();
        uart_send(0x56);
        dowait();
    }

}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
