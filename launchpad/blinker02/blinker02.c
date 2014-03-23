
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

#define RCGCGPIO 0x400FE608
#define PRGPIO   0x400FEA08

#define GPIOF_BASE 0x40025000
#define GPIOFDATA   (GPIOF_BASE+0x000)
#define GPIOFDIR    (GPIOF_BASE+0x400)
#define GPIOFAFSEL  (GPIOF_BASE+0x420)
#define GPIOFODR    (GPIOF_BASE+0x50C)
#define GPIOFPUR    (GPIOF_BASE+0x510)
#define GPIOFPDR    (GPIOF_BASE+0x514)
#define GPIOFDEN    (GPIOF_BASE+0x51C)
#define GPIOFOCR    (GPIOF_BASE+0x524)

#define CM4_BASE 0xE000E000
#define STCTRL      (CM4_BASE+0x010)
#define STRELOAD    (CM4_BASE+0x014)
#define STCURRENT   (CM4_BASE+0x018)

void dowait ( void )
{
    unsigned int ra;

    for(ra=0x200;ra;ra--)
    {
        dummy(ra);
    }
}

void notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int lasttime,nowtime;

    //GPIO (led) init
    ra=GET32(RCGCGPIO);
    ra|=1<<5; //GPIOF
    PUT32(RCGCGPIO,ra);

    while(1)
    {
        if(GET32(PRGPIO)&(1<<5)) break;
    }

    ra=GET32(GPIOFDIR);
    ra|=(1<<3)|(1<<2)|(1<<1);
    PUT32(GPIOFDIR,ra);

    ra=GET32(GPIOFAFSEL);
    ra&=~((1<<3)|(1<<2)|(1<<1));
    PUT32(GPIOFAFSEL,ra);

    ra=GET32(GPIOFODR);
    ra&=~((1<<3)|(1<<2)|(1<<1));
    PUT32(GPIOFODR,ra);

    ra=GET32(GPIOFDEN);
    ra|=(1<<3)|(1<<2)|(1<<1);
    PUT32(GPIOFDEN,ra);

    PUT32(GPIOFDATA|(((1<<3)|(1<<2)|(1<<1))<<2),0x00); //led off

    //Cortex-M4 systick timer init
    PUT32(STCTRL,0x00000004);
    PUT32(STRELOAD,0xFFFFFFFF); //really only 24 bits
    PUT32(STCURRENT,0); //value is a dont care
    PUT32(STCTRL,0x00000005);


    lasttime=GET32(STCURRENT);
    for(ra=0;ra<10;ra++)
    //while(1)
    {
        rb=0;
        while(1)
        {
            nowtime=GET32(STCURRENT);
            rc=(lasttime-nowtime)&0x00FFFFFF;
            if(rc<1000000) continue;
            lasttime=nowtime;
            rb++;
            if(rb>=160) break;
        }
        //just blink it, to painful on the eyes to leave it on
        PUT32(GPIOFDATA|(((1<<3)|(1<<2)|(1<<1))<<2),0xFF);
        dowait();
        PUT32(GPIOFDATA|(((1<<3)|(1<<2)|(1<<1))<<2),0x00);
    }
}

