
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

unsigned int data[2048];

int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;

    if(argc<3)
    {
        printf("bintoc input output\n");
        return(1);
    }
    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    rb=fread(data,1,sizeof(data),fp);
    fclose(fp);
    printf("%u bytes read\n",rb);
    if(rb>4096)
    {
        printf("Error more than a sector\n");
        return(1);
    }
    fp=fopen(argv[2],"wt");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[2]);
        return(1);
    }
    
    rb+=3;
    rb>>=1;
    fprintf(fp,"\n");
    fprintf(fp,"const unsigned int flash_data_words=0x%08X;\n",rb);
    fprintf(fp,"const unsigned int flash_data[]=\n");
    fprintf(fp,"{\n");
    for(ra=0;ra<rb;ra++)
    {
        fprintf(fp,"0x%08X, // 0x%04X\n",data[ra],ra<<2);
    }
    fprintf(fp,"};\n");
    fprintf(fp,"\n");
    fclose(fp);

    return(0);
}

