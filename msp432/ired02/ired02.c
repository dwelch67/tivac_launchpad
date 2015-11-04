
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


    if(0)
    {
        unsigned int min,max;
        rd=0;
        min=0; min--;
        max=0;
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK;
            if(rd>max) max=rd;
            if(rd<min) min=rd;
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rb=GET32(SYST_CVR);
                    rd=(rb-rc)&SYST_MASK;
                    if(rd>0x6000) break;
                }
                if(rd>0x6000) break;
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
            }
            hexstrings(min);
            hexstring(max);




        }
    }


    if(0)
    {
        while(1)
        {
            for(ra=0;ra<64;)
            {
                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rx[ra++]=GET32(SYST_CVR);
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rx[ra++]=GET32(SYST_CVR);
            }
            rc=rx[ra-1];
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rb=GET32(SYST_CVR);
                    rd=(rc-rb)&SYST_MASK;
                    if(rd>3000000) break;
                }
                if(rd>3000000) break;
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
            }
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
    //Okay so this tells all
    //00000040
    //00000000 005121D2
    //00000001 00510570 00001C62 sync
    //00000002 0050FEC6 000006AA
    //00000003 0050F722 000007A4 zero
    //00000004 0050F030 000006F2
    //00000005 0050E226 00000E0A one
    //00000006 0050DB34 000006F2
    //00000007 0050CD2A 00000E0A one
    //00000008 0050C638 000006F2
    //00000009 0050BEDC 0000075C zero
    //0000000A 0050B7FC 000006E0
    //0000000B 0050B0B2 0000074A zero
    //0000000C 0050A9AE 00000704
    //0000000D 0050A264 0000074A zero
    //0000000E 00509B72 000006F2
    //0000000F 00508D68 00000E0A one
    //00000010 005086BE 000006AA
    //00000011 00507F1A 000007A4 zero
    //00000012 0050783A 000006E0
    //00000013 005070F0 0000074A zero
    //00000014 005069FE 000006F2
    //00000015 00505BF4 00000E0A one
    //00000016 0050554A 000006AA
    //00000017 00504DA6 000007A4 zero
    //00000018 005046B4 000006F2
    //00000019 00503F7C 00000738 zero
    //0000001A 00503878 00000704
    //0000001B 0050312E 0000074A zero
    //0000001C 00502A3C 000006F2
    //0000001D 005022E0 0000075C zero
    //0000001E 00501C00 000006E0
    //0000001F 00500DE4 00000E1C one
    //00000020 004F1230 0000FBB4  done
    //00000021 004EF5CE 00001C62 new code
    //00000022 004EEF12 000006BC
    //00000023 004EE780 00000792
    //00000024 004EE08E 000006F2
    //00000025 004ED284 00000E0A
    //00000026 004ECB92 000006F2
    //00000027 004EBD88 00000E0A
    //00000028 004EB6DE 000006AA
    //00000029 004EAF5E 00000780
    //0000002A 004EA85A 00000704
    //0000002B 004EA110 0000074A
    //0000002C 004E9A0C 00000704
    //0000002D 004E92C2 0000074A
    //0000002E 004E8BD0 000006F2
    //0000002F 004E7DC6 00000E0A
    //00000030 004E771C 000006AA
    //00000031 004E6F8A 00000792
    //00000032 004E6898 000006F2
    //00000033 004E613C 0000075C
    //00000034 004E5A5C 000006E0
    //00000035 004E4C40 00000E1C
    //00000036 004E45A8 00000698
    //00000037 004E3E16 00000792
    //00000038 004E3712 00000704
    //00000039 004E2FC8 0000074A
    //0000003A 004E28D6 000006F2
    //0000003B 004E217A 0000075C
    //0000003C 004E1A9A 000006E0
    //0000003D 004E1350 0000074A
    //0000003E 004E0C5E 000006F2
    //0000003F 004DFE54 00000E0A
    if(0)
    {
        while(1)
        {
            for(ra=0;ra<64;)
            {
                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rx[ra++]=(rb-rc)&SYST_MASK;
            }
            rc=rx[ra-1];
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rb=GET32(SYST_CVR);
                    rd=(rc-rb)&SYST_MASK;
                    if(rd>3000000) break;
                }
                if(rd>3000000) break;
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
            }
            rd=ra;
            hexstring(rd);
            for(ra=0;ra<rd;ra++)
            {
                hexstrings(ra);
                hexstring(rx[ra]);
            }
        }
    }
    //00000040
    //00000000 00001C72
    //00000001 00000790 0
    //00000002 00000E1A 1
    //00000003 00000E62 1
    //00000004 00000790 0
    //00000005 00000748 0
    //00000006 00000736 0
    //00000007 00000E1A 1
    //00000008 00000790 0
    //00000009 0000075A 0
    //0000000A 00000E08 1
    //0000000B 00000736 0
    //0000000C 00000748 0
    //0000000D 0000075A 0
    //0000000E 00000748 0
    //0000000F 00000DF6 1
    //00000010 00001C72
    //00000011 00000790
    //00000012 00000E08
    //00000013 00000E50
    //00000014 000007A2
    //00000015 00000748
    //00000016 00000748
    //00000017 00000E08
    //00000018 000007A2
    //00000019 0000075A
    //0000001A 00000E1A
    //0000001B 00000790
    //0000001C 0000075A
    //0000001D 00000748
    //0000001E 00000748
    //0000001F 00000E08
    //00000020 00001C60
    //00000021 00000790
    //00000022 00000DF6
    //00000023 00000E3E
    //00000024 00000790
    //00000025 0000075A
    //00000026 00000748
    //00000027 00000E08
    //00000028 00000790
    //00000029 00000748
    //0000002A 00000E08
    //0000002B 000007A2
    //0000002C 00000748
    //0000002D 00000748
    //0000002E 0000075A
    //0000002F 00000E1A

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
            if(rd<0x1B00) continue;
            if(rd>0x1D80) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync return
            if(rd>max) max=rd;
            if(rd<min) min=rd;
            hexstrings(min);
            hexstring(max);
        }
    }
    //00000644 000006C2



    //here is a quick and dirty decoder for the sony sirc code
    //3000000 * 0.002400 = 7200 = 0x1C20
    //3000000 * 0.001200 = 3600 = 0xE10
    //3000000 * 0.000600 = 1800 = 0x708
    //3000000 * 0.045000 = 135000 = 0x20F58
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
            rd=(rb-rc)&SYST_MASK; //sync burst
            //if(rd<0x1B00) continue;
            if(rd<0x1B80) continue;
            if(rd>0x1D00) continue;
            code=0;
            for(ra=0;ra<15;ra++)
            {
                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //pause
                if(rd>0x800) break;
                if(rd<0x600) break;
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //burst
                code<<=1;
                if(rd>0x900) code|=1;
                if(rd>0x1000) break;
            }
            hexstring(code);
        }
    }


    if(0)
    {
        unsigned int x;
        unsigned int code;
        code = 0;
        //while(1)
        {
            for(x=0;x<64;x++)
            {
                rc=GET32(SYST_CVR);
                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //sync burst
                if(rd>3000000) break;
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //sync burst
                //if(rd<0x1B00) continue;
                if(rd<0x1B80) continue;
                if(rd>0x1D00) continue;
                code=0;
                for(ra=0;ra<15;ra++)
                {
                    while((GET8(PAIN_L)&0x40)==0x40) continue;
                    rb=GET32(SYST_CVR);
                    rd=(rc-rb)&SYST_MASK; //pause
                    if(rd>0x800) break;
                    if(rd<0x600) break;
                    while((GET8(PAIN_L)&0x40)==0x00) continue;
                    rc=GET32(SYST_CVR);
                    rd=(rb-rc)&SYST_MASK; //burst
                    code<<=1;
                    if(rd>0x900) code|=1;
                    if(rd>0x1000) break;
                }
                rx[x]=code;
            }
            for(ra=0;ra<x;ra++)
            {
                hexstrings(ra);
                hexstring(rx[ra]);
            }
        }
    }


    if(0)
    {
        while(1)
        {
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //sync burst
                //if(rd<0x1B00) continue;
                if(rd<0x1B80) continue;
                if(rd>0x1D00) continue;
                break;
            }
            ra=0;
            while(1)
            {
                while((GET8(PAIN_L)&0x40)==0x40)
                {
                    rb=GET32(SYST_CVR);
                    rd=(rc-rb)&SYST_MASK;
                    if(rd>3000000) break;
                }
                if(rd>3000000) break;
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //sync burst
                //if(rd<0x1B00) continue;
                if(rd<0x1B80) continue;
                if(rd>0x1D00) continue;
                ra++;
            }
            hexstring(ra);
        }
    }


    if(1)
    {
        unsigned int code;
        unsigned int state;
        unsigned int codes;
        code = 0;
        state = 0;
        codes = 0;
        rc=GET32(SYST_CVR);
        while(1)
        {

            while((GET8(PAIN_L)&0x40)==0x40)
            {
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK;
                if(rd>3000000)
                {
                    if(state)
                    {
                        hexstring(codes);
                        for(ra=0;ra<codes;ra++)
                        {
                            hexstrings(ra);
                            hexstring(rx[ra]);
                        }
                        state=0;
                        codes=0;
                    }
                }
            }
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync burst
            //if(rd<0x1B00) continue;
            if(rd<0x1B80) continue;
            if(rd>0x1D00) continue;
            code=0;
            state=1;
            for(ra=0;ra<15;ra++)
            {
                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //pause
                if(rd>0x800) break;
                if(rd<0x600) break;
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //burst
                code<<=1;
                if(rd>0x900) code|=1;
                if(rd>0x1000) break;
            }
            //hexstring(code);
            if(codes<64) rx[codes++]=code;
        }
    }
    //and this is basically it single button presses on this
    //particular remote result in a burst of codes  some of them
    //interestingly have this 5121 pattern others do not.

    //12345678
    //00000006
    //00000000 00003121
    //00000001 00003121
    //00000002 00003121
    //00000003 00005121
    //00000004 00005121
    //00000005 00005121
    //00000006
    //00000000 00003121
    //00000001 00003121
    //00000002 00003121
    //00000003 00005121
    //00000004 00005121
    //00000005 00005121
    //00000006
    //00000000 00001621
    //00000001 00001621
    //00000002 00001621
    //00000003 00005121
    //00000004 00005121
    //00000005 00005121
    //00000007
    //00000000 00005621
    //00000001 00005621
    //00000002 00005621
    //00000003 00005621
    //00000004 00005121
    //00000005 00005121
    //00000006 00005121
    //00000007
    //00000000 00007121
    //00000001 00007121
    //00000002 00007121
    //00000003 00007121
    //00000004 00005121
    //00000005 00005121
    //00000006 00005121
    //00000006
    //00000000 00000A21
    //00000001 00000A21
    //00000002 00000A21
    //00000003 00005121
    //00000004 00005121
    //00000005 00005121
    //00000004
    //00000000 00006421
    //00000001 00006421
    //00000002 00006421
    //00000003 00006421
    //00000004
    //00000000 00002421
    //00000001 00002421
    //00000002 00002421
    //00000003 00002421
    //00000006
    //00000000 00001421
    //00000001 00001421
    //00000002 00001421
    //00000003 00005121
    //00000004 00005121
    //00000005 00005121
    //00000006
    //00000000 00005821
    //00000001 00005821
    //00000002 00005821
    //00000003 00005121
    //00000004 00005121
    //00000005 00005121



    hexstring(0x12345678);

    return(0);
}

