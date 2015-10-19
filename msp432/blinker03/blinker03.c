
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE 0x40004C00

#define PAIN PAIN_L
#define PAIN_L (PORT_BASE+0x00)
#define PAIN_H (PORT_BASE+0x01)
#define PAOUT PAOUT_L
#define PAOUT_L (PORT_BASE+0x02)
#define PAOUT_H (PORT_BASE+0x03)
#define PADIR PADIR_L
#define PADIR_L (PORT_BASE+0x04)
#define PADIR_H (PORT_BASE+0x05)
#define PAREN PAREN_L
#define PAREN_L (PORT_BASE+0x06)
#define PAREN_H (PORT_BASE+0x07)

#define WDTCTL   0x4000480C

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    PUT16(WDTCTL,0x5A84); //stop WDT

    PUT8(PADIR_L,GET8(PADIR_L)|0x01);
    PUT8(PADIR_L,GET8(PADIR_L)&0xED);
    PUT8(PAREN_L,GET8(PAREN_L)|0x12); //enable pull up/down
    PUT8(PAOUT_L,GET8(PAOUT_L)|0x12); //pull up

    ra=GET8(PAOUT_L);
    while(1)
    {
        rb=GET8(PAIN_L);
        if((rb&0x12)!=0x12) ra|=0x01;
        else                ra&=0xFE;
        PUT8(PAOUT_L,ra);
    }
    return(0);
}
