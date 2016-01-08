
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

#define SYST_CSR 0xE000E010
#define SYST_RVR 0xE000E014
#define SYST_CVR 0xE000E018
#define SYST_CALIB 0xE000E01C

#define CS_BASE     0x40010400
#define CSKEY       (CS_BASE+0x00)
#define CSCTL0      (CS_BASE+0x04)
#define CSCTL1      (CS_BASE+0x08)
#define CSCTL2      (CS_BASE+0x0C)
#define CSCTL3      (CS_BASE+0x10)
#define CSCLKEN     (CS_BASE+0x30)
#define CSSTAT      (CS_BASE+0x34)
#define CSIFG       (CS_BASE+0x48)
#define CSCLRIFG    (CS_BASE+0x50)

#define WDTCTL   0x4000480C


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
int notmain ( void )
{
    unsigned int ra;

    PUT16(WDTCTL,0x5A84); //stop WDT

    PUT32(CSKEY,0x695A);
    PUT32(CSCTL0,0);
    PUT32(CSCTL0,3<<16);
    PUT32(CSCTL1,0x00000033);
    PUT32(CSKEY,0);
   
    PUT8(PADIR_L,GET8(PADIR_L)|0x01);

    PUT32(SYST_CSR,4);
    PUT32(SYST_RVR,12000000-1);
    PUT32(SYST_CVR,12000000-1);
    PUT32(SYST_CSR,5);

    ra=GET8(PAOUT_L);
    ra&=~1;
    while(1)
    {
        ra^=1;
        PUT8(PAOUT_L,ra);
        delay();
        ra^=1;
        PUT8(PAOUT_L,ra);
        delay();
    }
    return(0);
}
