
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

void dowait ( void )
{
    unsigned int ra;

    for(ra=0x20000;ra;ra--)
    {
        dummy(ra);
    }
}

void notmain ( void )
{
    unsigned int ra;

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

    for(ra=0;ra<10;ra++)
    //while(1)
    {
        PUT32(GPIOFDATA|(((1<<3)|(1<<2)|(1<<1))<<2),0xFF);
        dowait();
        PUT32(GPIOFDATA|(((1<<3)|(1<<2)|(1<<1))<<2),0x00);
        dowait();
    }

}

