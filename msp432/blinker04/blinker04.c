
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );
void CPSID ( void );
void CPSIE ( void );
void WFI ( void );


#define PORT_BASE 0x40004C00

#define PAOUT PAOUT_L
#define PAOUT_L (PORT_BASE+0x02)
#define PAOUT_H (PORT_BASE+0x03)
#define PADIR PADIR_L
#define PADIR_L (PORT_BASE+0x04)
#define PADIR_H (PORT_BASE+0x05)

#define TIMER32_BASE    0x4000C000
#define T32LOAD1        (TIMER32_BASE+0x00)
#define T32VALUE1       (TIMER32_BASE+0x04)
#define T32CONTROL1     (TIMER32_BASE+0x08)
#define T32INTCLR1      (TIMER32_BASE+0x0C)
#define T32RIS1         (TIMER32_BASE+0x10)
#define T32MIS1         (TIMER32_BASE+0x14)
#define T32BGLOAD1      (TIMER32_BASE+0x18)

volatile unsigned char pastate;

void interrupt_25_handler ( void )
{
    pastate^=1;
    PUT8(PAOUT_L,pastate);
    PUT32(T32INTCLR1,0);
    PUT32(0xE000E280,1<<25);
}


int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;

    PUT8(PADIR_L,GET8(PADIR_L)|0x01);


    ra=GET8(PAOUT_L);
    ra|=1;

    PUT32(T32CONTROL1,0x00000002);
    PUT32(T32CONTROL1,0x00000082);
    for(rb=0;rb<2;rb++)
    {
        ra^=1;
        PUT8(PAOUT_L,ra);
        while(1) if((GET32(T32VALUE1)&0x08000000)!=0) break;
        ra^=1;
        PUT8(PAOUT_L,ra);
        while(1) if((GET32(T32VALUE1)&0x08000000)==0) break;
    }


    PUT32(T32CONTROL1,0x00000042);
    PUT32(T32LOAD1,48000000-1);
    PUT32(T32VALUE1,48000000-1);
    PUT32(T32INTCLR1,0);
    CPSID();
    PUT32(T32CONTROL1,0x000000C2);
    for(rb=0;rb<3;rb++)
    {
        ra^=1;
        PUT8(PAOUT_L,ra);
        while(1) if((GET32(T32RIS1)&0x00000001)!=0) break;
        PUT32(T32INTCLR1,0);
        ra^=1;
        PUT8(PAOUT_L,ra);
        while(1) if((GET32(T32RIS1)&0x00000001)!=0) break;
        PUT32(T32INTCLR1,0);
    }



    PUT32(T32CONTROL1,0x00000062);
    PUT32(T32LOAD1,36000000-1);
    PUT32(T32VALUE1,36000000-1);
    PUT32(T32INTCLR1,0);
    PUT32(0xE000E180,0xFFFFFFFF);
    PUT32(0xE000E280,0xFFFFFFFF);
    PUT32(0xE000E100,1<<25);
    CPSID();
    PUT32(T32CONTROL1,0x000000E2);
    for(rb=0;rb<3;rb++)
    {
        ra^=1;
        PUT8(PAOUT_L,ra);
        while(1) if((GET32(0xE000E200)&(1<<25))!=0) break;
        PUT32(T32INTCLR1,0);
        PUT32(0xE000E280,1<<25);
        ra^=1;
        PUT8(PAOUT_L,ra);
        while(1) if((GET32(0xE000E200)&(1<<25))!=0) break;
        PUT32(T32INTCLR1,0);
        PUT32(0xE000E280,1<<25);
    }


    PUT32(T32CONTROL1,0x00000062);
    PUT32(T32LOAD1,24000000-1);
    PUT32(T32VALUE1,24000000-1);
    PUT32(T32INTCLR1,0);
    PUT32(0xE000ED08,0x01000100);
    PUT32(0xE000E180,0xFFFFFFFF);
    PUT32(0xE000E280,0xFFFFFFFF);
    PUT32(0xE000E100,1<<25);
    pastate=ra;
    CPSIE();
    PUT32(T32CONTROL1,0x000000E2);
    WFI();
    while(1) continue;
    
    return(0);
}
