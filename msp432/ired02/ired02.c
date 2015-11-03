
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
    unsigned int code;
    unsigned int rx[64];

    PUT16(WDTCTL,0x5A84); //stop WDT
    uart_init();
    hexstring(0x12345678);

    //1.6 ir module input

    PUT8(PADIR_L,GET8(PADIR_L)|0x01);
    PUT8(PADIR_L,GET8(PADIR_L)&0xBF);
    PUT8(PAREN_L,GET8(PAREN_L)|0x40); //enable pull up/down
    PUT8(PAOUT_L,GET8(PAOUT_L)|0x40); //pull up

    // 1) determine the idle state
    // in my case the idle state was high
    if(0)
    {
        ra=0xFF;
        while(1)
        {
            rb=GET8(PAIN_L)&0x40;
            if(rb!=ra)
            {
                ra=rb;
                hexstring(ra);
            }
        }
    }

    // 2) understand the timer
    // notice it is a 24 bit timer (as documented) not 32
    // this is important to get the rollover math right.
    // notice it counts down
    PUT32(SYST_CSR,4);
    PUT32(SYST_RVR,0xFFFFFFFF);
    PUT32(SYST_CVR,0xFFFFFFFF);
    PUT32(SYST_CSR,5);
    if(0)
    {
        while(1)
        {
            ra=GET32(SYST_CVR);
            hexstring(ra);
        }
    }

    // 3) log some state changes
    if(0)
    {
        for(ra=0;ra<5;ra++)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rx[(ra<<1)+0]=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rx[(ra<<1)+1]=GET32(SYST_CVR);
        }
        for(ra=0;ra<10;ra++)
        {
            if(ra)
            {
                rb=(rx[ra-1]-rx[ra])&SYST_MASK;
                hexstrings(rx[ra]);
                hexstring(rb);
            }
            else
            {
                hexstring(rx[ra]);
            }
        }
    }
    //0020CC17
    //0020AFA1 00001C76
    //0020A8AB 000006F6
    //0020A171 0000073A
    //00209A7B 000006F6
    //00208C6F 00000E0C
    //00208579 000006F6
    //0020775B 00000E1E
    //00207077 000006E4
    //0020692B 0000074C
    //Nice I like this flavor of protocol that has a long/noticeable
    //start pulse, easier to detect/find and know where you are
    //might want to try your different remotes to find one like this
    //where the first pulse is longer than others and sometimes the
    //second number is half the first.  This should also help us
    //get a rough idea how to detect return to idle state.
    //also notice this
    //0020AFA1 00001C76  <-- low sync pulse
    //0020A8AB 000006F6  high
    //0020A171 0000073A  low
    //00209A7B 000006F6  high
    //00208C6F 00000E0C  low
    //00208579 000006F6  high
    //0020775B 00000E1E  low
    //00207077 000006E4  high
    //0020692B 0000074C  low
    //again this is one of my preferred protocols to decode, easier to
    //decode not that others are hard but this is a good first one.
    //the high parts are about the same length, the low parts after
    //the sync appear to be either long or short.  it is really
    //arbitrary which we call a 1 or a 0 but we make one a 1 and the
    //other a zero.  if we get a high much longer than what we see
    //above we call it the end of the pattern.

    // 4) log more state changes
    if(0)
    {
        while(1)
        {
            for(ra=0;ra<64;)
            {
                rd=0;
                rb=GET32(SYST_CVR);
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rc=GET32(SYST_CVR);
                    rd=(rb-rc)&SYST_MASK;
                    if(rd>0x5000) break;
                    continue;
                }
                if(rd>0x5000) break;
                rx[ra++]=GET32(SYST_CVR);
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rx[ra++]=GET32(SYST_CVR);
            }
            if(ra==0) continue;
            rd=ra;
            hexstring(rd);
            for(ra=0;ra<rd;ra++)
            {
                hexstrings(ra);
                if(ra)
                {
                    rb=(rx[ra-1]-rx[ra])&SYST_MASK;
                    hexstrings(rx[ra]);
                    hexstring(rb);
                }
                else
                {
                    hexstring(rx[ra]);
                }
            }
        }
    }
    //00000000 009E4860
    //00000001 009E2C11 00001C4F
    //00000002 009E24FA 00000717
    //00000003 009E1715 00000DE5
    //00000004 009E0FFE 00000717
    //00000005 009E08C7 00000737
    //00000006 009E01D2 000006F5
    //00000007 009DF3C9 00000E09
    //00000008 009DED18 000006B1
    //00000009 009DE587 00000791
    //0000000A 009DDE92 000006F5
    //0000000B 009DD749 00000749
    //0000000C 009DD054 000006F5
    //0000000D 009DC90B 00000749
    //0000000E 009DC216 000006F5
    //0000000F 009DB40D 00000E09
    //00000010 009DACF6 00000717
    //00000011 009DA5BF 00000737
    //00000012 009D9ECA 000006F5
    //00000013 009D9793 00000737
    //00000014 009D907C 00000717
    //00000015 009D8297 00000DE5
    //00000016 009D7BC4 000006D3
    //00000017 009D7445 0000077F
    //00000018 009D6D50 000006F5
    //00000019 009D65F5 0000075B
    //0000001A 009D5F00 000006F5
    //0000001B 009D57C9 00000737
    //0000001C 009D50D4 000006F5
    //0000001D 009D4979 0000075B
    //0000001E 009D4284 000006F5
    //0000001F 009D348D 00000DF7
    //5) okay getting protocol specific now.  see where this goes
    //next important thing to learn is that this is not all purely
    //digital.  what is going on is that the sending led is blinking
    //at a frequency of about 40Khz or a little slower, ideally you
    //want to get matched receivers for the specific freqency (use a
    //38Khz receiver instead of a 40 for example if the remote is
    //that frequency.  being close enough works but you might see the
    //effects in this next step, even with matching frequencies.
    //so the pulses are ideally square waves but there is some
    //physics there, you cant be perfect on that.  next the above
    //pulses we see are not these 40Khz, it is more like am radio
    //the non-idle signal is a blob of pulses of some length.  kind of
    //like a hair comb with some teeth, then some missing then some
    //there.  we dont see the individual teeth but if you squint and
    //make a group of teeth look like a combined blob, we see the length
    //of the blob.
    //to cut this short due to the analog nature of this being closer
    //or futher or perhaps strength of the battery in the remote, etc
    //you will see variations in these time measurements.
    //getting protocol specific so lets look for just long low periods
    //greater than 0x1000 counts.
    if(0)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK;
            if(rd>0x1000) hexstring(rd);
        }
    }
    //fortunately with this remote and receiver I am seeing
    //fairly consistent 0x1Czz where the zz part varies but the
    //0x1C00 time is consistent.
    //with this remote and there is no hard and fast rule
    //i am seeing about or maybe exactly 4 start patterns for each
    //quick button press.  dont want one volume up or channel up to
    //be interpreted as four so you need to watch for this.

    //lets capture idles less than 800
    if(0)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK;
            if(rd<0x800) hexstring(rd);
        }
    }
    //hmmm I see some 0x400s I didnt see before, maybe it is the way
    //this test was coded
    if(0)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK;
            if(rd>0x800) continue;
            if(rd<0x500) continue;
            hexstring(rd);
        }
    }
    //that demonstrates the point for this specific remote
    //up close I get at/over 0x700 counts
    //further away I get down into the lower 0x600s, also remember
    //not to just walk across the room but also point the remote at
    //all the walls to gain some distance and see what the reflection
    //does.  allowing for some slop allows you to still be able to
    //detect a code.



    //just go for it

    if(1)
    {
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            //sync
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK;
            if(rd>0x1000) ; else continue;
            
            code=0;
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rb=GET32(SYST_CVR);
                    rd=(rc-rb)&SYST_MASK;
                    if(rd>0x1000) break;
                }
                if(rd>0x1000)
                {
                    hexstring(code);
                    break;
                }
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK;
                code<<=1;
                if(rd>0x800) code|=1;
            }
        }
    }

































    hexstring(0x12345678);

    return(0);
}

