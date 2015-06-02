
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );

#define PORT_BASE 0x40004C00

#define PAOUT PAOUT_L
#define PAOUT_L (PORT_BASE+0x02)
#define PAOUT_H (PORT_BASE+0x03)
#define PADIR PADIR_L
#define PADIR_L (PORT_BASE+0x04)
#define PADIR_H (PORT_BASE+0x05)

#define WDTCTL   0x4000480C

#define DELAY_COUNT 200000

void delay ( unsigned int x )
{
    while(x--) dummy(x);
}
int notmain ( void )
{
    unsigned int ra;

    PUT16(WDTCTL,0x5A84); //stop WDT

    PUT8(PADIR_L,GET8(PADIR_L)|0x01);
    ra=GET8(PAOUT_L);
    while(1)
    {
        ra^=1;
        PUT8(PAOUT_L,ra);
        delay(DELAY_COUNT);
        ra^=1;
        PUT8(PAOUT_L,ra);
        delay(DELAY_COUNT);
    }
    return(0);
}
