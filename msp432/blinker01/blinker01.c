
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );


int notmain ( void )
{
    unsigned int ra;
    for(ra=0;;ra++)
    {
        PUT32(0x20001000,ra);
    }
    return(0);
}
