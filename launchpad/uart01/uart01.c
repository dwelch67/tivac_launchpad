
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
void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET32(UARTFR)&0x80) break;
    }
    PUT32(UARTDR,x);
}


void notmain ( void )
{
    unsigned int ra;

    uart_init();

    //Cortex-M4 systick timer init
    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,16000000-1);
    PUT32(STCURRENT,0); //value is a dont care
    PUT32(STCTRL,0x00000005);

    PUT32(UARTDR,0x56);
    for(ra=0;ra<5;ra++)
    {
        while(1)
        {
            if(GET32(STCTRL)&0x10000) break;
        }
        PUT32(UARTDR,0x55);
    }
    PUT32(UARTDR,0x0D);
    //for(ra=0;ra<100;ra++)
    for(ra=0;;ra++)
    {
        uart_send(0x30|(ra&7));
    }
}

