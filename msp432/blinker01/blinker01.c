
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

#define DELAY_COUNT 100000

void delay ( unsigned int x )
{
    while(x--) dummy(x);
}
int notmain ( void )
{
    unsigned int ra;

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
    for(ra=0;;ra++)
    {
        PUT32(0x20001000,ra);
    }
    return(0);
}
