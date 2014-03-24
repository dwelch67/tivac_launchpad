
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define RCGCGPIO 0x400FE608
#define PRGPIO   0x400FEA08

#define GPION_BASE 0x40064000
#define GPIONDATA   (GPION_BASE+0x000)
#define GPIONDIR    (GPION_BASE+0x400)
#define GPIONAFSEL  (GPION_BASE+0x420)
#define GPIONODR    (GPION_BASE+0x50C)
#define GPIONPUR    (GPION_BASE+0x510)
#define GPIONPDR    (GPION_BASE+0x514)
#define GPIONDEN    (GPION_BASE+0x51C)
#define GPIONOCR    (GPION_BASE+0x524)

#define CM4_BASE 0xE000E000
#define STCTRL      (CM4_BASE+0x010)
#define STRELOAD    (CM4_BASE+0x014)
#define STCURRENT   (CM4_BASE+0x018)

void dowait ( void )
{
    unsigned int ra;

    for(ra=0x4000;ra;ra--)
    {
        dummy(ra);
    }
}

void notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    ra=GET32(RCGCGPIO);
    ra|=1<<12; //GPION
    PUT32(RCGCGPIO,ra);

    while(1)
    {
        if(GET32(PRGPIO)&(1<<12)) break;
    }

    //LEDs on PN0 and PN1

    ra=GET32(GPIONDIR);
    ra|=(1<<1)|(1<<0);
    PUT32(GPIONDIR,ra);

    //should already be 0
    ra=GET32(GPIONAFSEL);
    ra&=~((1<<1)|(1<<0));
    PUT32(GPIONAFSEL,ra);

    ra=GET32(GPIONODR);
    ra&=~((1<<1)|(1<<0));
    PUT32(GPIONODR,ra);

    ra=GET32(GPIONDEN);
    ra|=(1<<1)|(1<<0);
    PUT32(GPIONDEN,ra);

    PUT32(GPIONDATA|(((1<<1)|(1<<0))<<2),0x00); //led off

    //Cortex-M4 systick timer init
    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,1000000-1);
    PUT32(STCURRENT,0); //value is a dont care
    PUT32(STCTRL,0x00000005);

    for(ra=0;ra<10;ra++)
    //while(1)
    {
        rb=0;
        while(1)
        {
            if(GET32(STCTRL)&0x10000)
            {
                rb++;
                if(rb>=160) break;
            }
        }
        //just blink it
        PUT32(GPIONDATA|(((1<<1)|(1<<0))<<2),0xFF); //led on
        dowait();
        PUT32(GPIONDATA|(((1<<1)|(1<<0))<<2),0x00); //led off
    }
}

