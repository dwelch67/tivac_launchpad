
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

//00EFFF34 0000D3D6
//00EF2CF8 0000D23C
//00EF127A 00001A7E
//00EEC414 00004E66
//00EEA984 00001A90
//00EE5B30 00004E54
//00EE407C 00001AB4
//00EDF24C 00004E30
//00EDD786 00001AC6



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
                    if(rd>0xF000) break;
                    continue;
                }
                if(rd>0xF000) break;
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


//00000040
//00000000 003D5000
//00000001 003D1B11 000034EF
//00000002 003CE660 000034B1
//00000003 003CDFC7 00000699
//00000004 003CCC28 0000139F
//00000005 003CC5A1 00000687
//00000006 003CB202 0000139F
//00000007 003CAB57 000006AB
//00000008 003C97B8 0000139F
//00000009 003C910D 000006AB
//0000000A 003C8A94 00000679
//0000000B 003C83FB 00000699
//0000000C 003C7D82 00000679
//0000000D 003C76D7 000006AB
//0000000E 003C705E 00000679
//0000000F 003C69C5 00000699
//00000010 003C634C 00000679
//00000011 003C5CB3 00000699
//00000012 003C5618 0000069B
//00000013 003C4F7F 00000699
//00000014 003C3BE0 0000139F
//00000015 003C3535 000006AB
//00000016 003C21B8 0000137D
//00000017 003C1B1F 00000699
//00000018 003C0780 0000139F
//00000019 003C00D5 000006AB
//0000001A 003BFA5C 00000679
//0000001B 003BF3C3 00000699
//0000001C 003BED28 0000069B
//0000001D 003BE68F 00000699
//0000001E 003BE016 00000679
//0000001F 003BD97D 00000699
//00000020 003BD304 00000679
//00000021 003BCC47 000006BD
//00000022 003BC5CE 00000679
//00000023 003BBF35 00000699
//00000024 003BB8BC 00000679
//00000025 003BB223 00000699
//00000026 003B9E84 0000139F
//00000027 003B97EB 00000699
//00000028 003B9172 00000679
//00000029 003B8AB5 000006BD
//0000002A 003B843C 00000679
//0000002B 003B7DA3 00000699
//0000002C 003B772A 00000679
//0000002D 003B7091 00000699
//0000002E 003B6A18 00000679
//0000002F 003B635B 000006BD
//00000030 003B5CE2 00000679
//00000031 003B5649 00000699
//00000032 003B4FD0 00000679
//00000033 003B4937 00000699
//00000034 003B3598 0000139F
//00000035 003B2EFF 00000699
//00000036 003B2886 00000679
//00000037 003B21C9 000006BD
//00000038 003B0E2A 0000139F
//00000039 003B07A3 00000687
//0000003A 003AF404 0000139F
//0000003B 003AED59 000006AB
//0000003C 003AD9BA 0000139F
//0000003D 003AD321 00000699
//0000003E 003ABF82 0000139F
//0000003F 003AB8D7 000006AB



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
            rd=(rb-rc)&SYST_MASK; //sync burst
            if(rd<0x2000) continue;
            if(rd>max) max=rd;
            if(rd<min) min=rd;
            hexstrings(min);
            hexstring(max);
        }
    }
    //000034D8 0000359E sync burst

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
            rd=(rb-rc)&SYST_MASK; //sync burst
            if(rd>0x3700) continue;
            if(rd<0x3300) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync pause

            //if(rd<0x2000) continue;
            if(rd>max) max=rd;
            if(rd<min) min=rd;
            hexstrings(min);
            hexstring(max);
        }
    }
    //0000341E 000034F6

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
            rd=(rb-rc)&SYST_MASK; //sync burst
            if(rd>0x3700) continue;
            if(rd<0x3300) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync pause

            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync burst

            if(rd>max) max=rd;
            if(rd<min) min=rd;
            hexstrings(min);
            hexstring(max);
        }
    }
    //00000694 00000712

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
            rd=(rb-rc)&SYST_MASK; //sync burst
            if(rd>0x3700) continue;
            if(rd<0x3300) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync pause

            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //burst

            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //pause

            if(rd<0x800) continue;

            if(rd>max) max=rd;
            if(rd<min) min=rd;
            hexstrings(min);
            hexstring(max);
        }
    }
    //0000132A 000013F0

    //basic decoder.
    if(0)
    {
        unsigned int code;
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync burst
            if(rd>0x3700) continue;
            if(rd<0x3300) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync pause
            if(rd>0x3700) continue;
            if(rd<0x3300) continue;
            code=0;
            for(ra=0;;ra++)
            {
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //burst
                if(rd>0x900) break;
                if(rd<0x500) break;


                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //pause
                if(rd>0x900)
                {
                    if(rd>0x1400) break;
                    if(rd<0x1200) break;
                    code<<=1;
                    code|=1;
                }
                else
                {
                    if(rd<0x500) break;
                    code<<=1;
                    code|=0;
                }
            }
            hexstrings(ra);
            hexstring(code);
        }
    }
    //00000020 E0E016E9
    //looks like there are 32 data bits but the second half is not
    //the inverse of the first half like an RCA
    // this is a sharp remote, hdtv lcd generation.


    //repeats almost every press
    if(1)
    {
        unsigned int code;
        unsigned int lastcode;
        lastcode=0;
        while(1)
        {
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            while((GET8(PAIN_L)&0x40)==0x00) continue;
            rc=GET32(SYST_CVR);
            rd=(rb-rc)&SYST_MASK; //sync burst
            if(rd>0x3700) continue;
            if(rd<0x3300) continue;
            while((GET8(PAIN_L)&0x40)==0x40) continue;
            rb=GET32(SYST_CVR);
            rd=(rc-rb)&SYST_MASK; //sync pause
            if(rd>0x3700) continue;
            if(rd<0x3300) continue;
            code=0;
            for(ra=0;ra<32;ra++)
            {
                while((GET8(PAIN_L)&0x40)==0x00) continue;
                rc=GET32(SYST_CVR);
                rd=(rb-rc)&SYST_MASK; //burst
                if(rd>0x900) break;
                if(rd<0x500) break;


                while((GET8(PAIN_L)&0x40)==0x40) continue;
                rb=GET32(SYST_CVR);
                rd=(rc-rb)&SYST_MASK; //pause
                if(rd>0x900)
                {
                    if(rd>0x1400) break;
                    if(rd<0x1200) break;
                    code<<=1;
                    code|=1;
                }
                else
                {
                    if(rd<0x500) break;
                    code<<=1;
                    code|=0;
                }
            }
            if(code!=lastcode)
            hexstring(code);
            lastcode=code;
        }
    }
    //00000020 E0E016E9
    //looks like there are 32 data bits but the second half is not
    //the inverse of the first half like an RCA
    // this is a sharp remote, hdtv lcd generation.

//E0E040BF power
//E0E0D827 TV
//E0E020DF 1
//E0E0A05F 2
//E0E0609F 3
//E0E010EF 4
//E0E0906F 5
//E0E050AF 6
//E0E030CF 7
//E0E0B04F 8
//E0E0708F 9
//E0E0C43B -
//E0E08877 0
//E0E0C837 PRE-CH
//E0E0D629 CH LIST
//E0E058A7 MENU
//E0E022DD FAV CH

//E0E0D22D TOOLS
//E0E01AE5 RETURN
//E0E0B44B EXIT
//E0E0C936 INTERNET
//E0E006F9 UP
//E0E046B9 RIGHT
//E0E08679 DOWN
//E0E0A659 LEFT
//E0E016E9 SELECT/MIDDLE CURSOR

//E0E036C9 RED
//E0E028D7 GREEN
//E0E0A857 YELLOW
//E0E06897 BLUE
//E0E0E01F VOL +
//E0E0D02F VOL -
//E0E048B7 CH +
//E0E008F7 CH -
//E0E0807F SOURCE
//E0E0F00F MUTE


//E0E031CE MEDIA.P
//E0E09E61 CONTENT
//E0E07C83 P.SIZE
//E0E0F807 INFO
//E0E000FF MTS
//E0E0A45B CC
//E0E0A25D <<
//E0E052AD ||
//E0E012ED >>
//E0E0926D REC
//E0E0E21D PLAY
//E0E0629D STOP

    hexstring(0x12345678);

    return(0);
}

