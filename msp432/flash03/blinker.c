
void PUT16 ( unsigned int, unsigned int );

#define WDTCTL   0x4000480C

int notmain ( void )
{
    PUT16(WDTCTL,0x5A84); //stop WDT
    return(0);
}
