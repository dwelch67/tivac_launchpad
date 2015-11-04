
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );
void PUT8 ( unsigned int, unsigned int );
unsigned int GET8 ( unsigned int );
void PUT16 ( unsigned int, unsigned int );
unsigned int GET16 ( unsigned int );
void dummy ( unsigned int );
void ASMDELAY ( unsigned int );

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

#define PBOUT PBOUT_L
#define PBOUT_L (PORT_BASE+0x22)
#define PBOUT_H (PORT_BASE+0x23)
#define PBDIR PBDIR_L
#define PBDIR_L (PORT_BASE+0x24)
#define PBDIR_H (PORT_BASE+0x25)

#define P4IN  (PORT_BASE+0x21)
#define P4OUT (PORT_BASE+0x23)
#define P4DIR (PORT_BASE+0x25)


#define SYST_CSR 0xE000E010
#define SYST_RVR 0xE000E014
#define SYST_CVR 0xE000E018
#define SYST_CALIB 0xE000E01C
#define SYST_MASK 0x00FFFFFF

#define UART0_BASE 0x40001000
#define UCA0CTLW0   (UART0_BASE+0x00)
#define UCA0BRW     (UART0_BASE+0x06)
#define UCA0MCTLW   (UART0_BASE+0x08)
#define UCA0TXBUF   (UART0_BASE+0x0E)
#define UCA0IE      (UART0_BASE+0x1A)
#define UCA0IFG     (UART0_BASE+0x1C)

#define PORT_BASE 0x40004C00
#define PASEL0_L (PORT_BASE+0x0A)
#define PASEL1 PASEL1_L
#define PASEL1_L (PORT_BASE+0x0C)

#define WDTCTL   0x4000480C

//------------------------------------------------------------------------

//------------------------------------------------------------------------
void uart_init ( void )
{
    PUT8(PASEL1_L,GET8(PASEL1_L)&0xF3);
    PUT8(PASEL0_L,GET8(PASEL0_L)|0x0C);

    PUT16(UCA0CTLW0,0x0081);
    //PUT16(UCA0BRW,104); //12000000/115200 = 104
    PUT16(UCA0BRW,26); //3000000/115200 = 26
    PUT16(UCA0MCTLW,0x0000);
    PUT16(UCA0IE,0);
    PUT16(UCA0CTLW0,0x0081);
    PUT16(UCA0CTLW0,0x0080);
}
//------------------------------------------------------------------------
void uart_send ( unsigned int x )
{
    while(1)
    {
        if(GET16(UCA0IFG)&2) break;
    }
    PUT16(UCA0TXBUF,x);
}

//-------------------------------------------------------------------------
void hexstrings ( unsigned int d )
{
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
//-------------------------------------------------------------------------
void hexstring ( unsigned int d )
{
    hexstrings(d);
    uart_send(0x0D);
    uart_send(0x0A);
}
//-------------------------------------------------------------------------

int notmain ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;
    unsigned int rd;
    unsigned int rx[64];

    PUT16(WDTCTL,0x5A84); //stop WDT
    uart_init();
    hexstring(0x12345678);

    //1.6 ir module input

    PUT8(PADIR_L,GET8(PADIR_L)|0x01);
    PUT8(PADIR_L,GET8(PADIR_L)&0xBF);
    PUT8(PAREN_L,GET8(PAREN_L)|0x40); //enable pull up/down
    PUT8(PAOUT_L,GET8(PAOUT_L)|0x40); //pull up

    PUT32(SYST_CSR,4);
    PUT32(SYST_RVR,0xFFFFFFFF);
    PUT32(SYST_CVR,0xFFFFFFFF);
    PUT32(SYST_CSR,5);

    if(0)
    {
        rd=0;
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rc=(rb-rc)&SYST_MASK;
            if(rc>rd)
            {
                rd=rc;
                hexstring(rd);
            }
        }
    }
    //000002A4
    //000003C4
    //000004F6
    //0000052C
    //0000053E
    //00006AC6
    //00006AD8
    //00006AEA
    if(0)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK;
            if(rd<0x6000) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK;
            hexstring(rd);
        }
    }
    if(0)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync
            if(rd<0x6000) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //return after sync
            if(rd<0x3000) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //next signal
            hexstring(rd);
        }
    }
    if(0)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync
            if(rd<0x6000) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync return
            if(rd<0x3000) continue;
            for(ra=0;ra<10;)
            {
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //next signal
                rx[ra++]=rd;
                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //sync return
                rx[ra++]=rd;
            }
            for(rd=0;rd<ra;rd++)
            {
                if(rd)
                {
                    hexstrings(rd);
                    hexstrings(rx[rd]);
                    rb=(rx[rd-1]-rx[rd])&SYST_MASK;
                    hexstring(rb);
                }
                else
                {
                    hexstrings(rd);
                    hexstring(rx[rd]);
                }
            }
        }
    }
    if(0)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync
            if(rd<0x6000) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync return
            if(rd<0x3000) continue;
            ra=0;
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //next signal
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rb=GET32(SYST_CVR);
                    rd=(rc-rb)&SYST_MASK; //sync return
                    if(rd>0x6000) break;
                }
                if(rd>0x6000) break;
                ra++;
            }
            hexstring(ra);
        }
    }
    if(0)
    {
        unsigned int min,max;
        min=0; min--;
        max=0;
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync
            if(rd<0x6000) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync return
            if(rd<0x3000) continue;
            ra=0;
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //next signal
                if(rd<min) min=rd; //680
                if(rd>max) max=rd; //866
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rb=GET32(SYST_CVR);
                    rd=(rc-rb)&SYST_MASK; //sync return
                    if(rd>0x6000) break;
                }
                if(rd>0x6000) break;
                ra++;
            }
            hexstrings(ra);
            hexstrings(min);
            hexstring(max);

        }
    }

    //here is a quick and dirty decoder for the NEC code
    if(0)
    {
        unsigned int code;
        code = 0;
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync
            if(rd<0x6000) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync return
            if(rd<0x3000)
            {
                if(rd>0x1000)
                {
                    if(code) hexstring(~code);
                }
                continue;
            }
            code=0;
            for(ra=0;ra<32;ra++)
            {
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //next signal
                while((GET8(PAIN_L)&0x40)==0x40)
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //sync return
                code<<=1;
                if(rd>0x1000) code|=1;
            }
            rb=(code>>8);
            rb&=0x00FF00FF;
            rc=~code;
            rc&=0x00FF00FF;
            if(rb!=rc)
            {
                code=0;
                continue;
            }
            hexstring(code);
        }
    }


    //added some abort checks.
    if(1)
    {
        unsigned int code;
        code = 0;
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync
            if(rd<0x6000) continue;
            if(rd>0x7000) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync return
            if(rd<0x3000)
            {
                if(rd>0x1000)
                {
                    //if(code) hexstring(~code);
                }
                continue;
            }
            if(rd>0x4000) continue; //abort decode
            code=0;
            for(ra=0;ra<32;ra++)
            {
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //next signal
                while((GET8(PAIN_L)&0x40)==0x40)
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //sync return
                code<<=1;
                if(rd>0x1000) code|=1;
                if(rd>0x1800) break;
            }
            if(ra<32) continue; //abort decode
            rb=(code>>8);
            rb&=0x00FF00FF;
            rc=~code;
            rc&=0x00FF00FF;
            if(rb!=rc)
            {
                code=0;
                continue;
            }
            hexstring(code);
        }
    }



    hexstring(0x12345678);

    return(0);
}

