

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



unsigned char CRCTable[256];

unsigned char data[16];
unsigned char CRCPoly = 0x89;  // the value of our CRC-7 polynomial

void GenerateCRCTable()
{
    int i, j;

    // generate a table value for all 256 possible byte values
    for (i = 0; i < 256; i++)
    {
        CRCTable[i] = (i & 0x80) ? i ^ CRCPoly : i;
        for (j = 1; j < 8; j++)
        {
            CRCTable[i] <<= 1;
            if (CRCTable[i] & 0x80)
                CRCTable[i] ^= CRCPoly;
        }
    }
}


// adds a message byte to the current CRC-7 to get a the new CRC-7
unsigned char CRCAdd(unsigned char CRC, unsigned char message_byte)
{
    return CRCTable[(CRC << 1) ^ message_byte];
}


// returns the CRC-7 for a message of "length" bytes
unsigned char getCRC(unsigned char message[], int length)
{
    int i;
    unsigned char CRC = 0;

    for (i = 0; i < length; i++)
        CRC = CRCAdd(CRC, message[i]);

    return CRC;
}

int main ( void )
{
    unsigned int ra;
    unsigned int rb;
    unsigned char crc;

    GenerateCRCTable();

    rb=0;
    data[rb++]=0x00+(1<<6);
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;

    crc=0;
    for(ra=0;ra<rb;ra++)
    {
        crc=(crc << 1) ^ data[ra];
        crc=CRCTable[crc];
    }
    crc<<=1;
    crc|=1;
    data[rb++]=crc;
    for(ra=0;ra<rb;ra++) printf("0x%02X ",data[ra]);  printf("\n");


    rb=0;
    data[rb++]=0x08+(1<<6);
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x01;
    data[rb++]=0xAA;

    crc=0;
    for(ra=0;ra<rb;ra++)
    {
        crc=(crc << 1) ^ data[ra];
        crc=CRCTable[crc];
    }
    crc<<=1;
    crc|=1;
    data[rb++]=crc;
    for(ra=0;ra<rb;ra++) printf("0x%02X ",data[ra]);  printf("\n");



    rb=0;
    data[rb++]=58+(1<<6);
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;

    crc=0;
    for(ra=0;ra<rb;ra++)
    {
        crc=(crc << 1) ^ data[ra];
        crc=CRCTable[crc];
    }
    crc<<=1;
    crc|=1;
    data[rb++]=crc;
    for(ra=0;ra<rb;ra++) printf("0x%02X ",data[ra]);  printf("\n");

    rb=0;
    data[rb++]=55+(1<<6);
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;

    crc=0;
    for(ra=0;ra<rb;ra++)
    {
        crc=(crc << 1) ^ data[ra];
        crc=CRCTable[crc];
    }
    crc<<=1;
    crc|=1;
    data[rb++]=crc;
    for(ra=0;ra<rb;ra++) printf("0x%02X ",data[ra]);  printf("\n");

    rb=0;
    data[rb++]=41+(1<<6);
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;
    data[rb++]=0x00;

    crc=0;
    for(ra=0;ra<rb;ra++)
    {
        crc=(crc << 1) ^ data[ra];
        crc=CRCTable[crc];
    }
    crc<<=1;
    crc|=1;
    data[rb++]=crc;
    for(ra=0;ra<rb;ra++) printf("0x%02X ",data[ra]);  printf("\n");

    return(0);
}
