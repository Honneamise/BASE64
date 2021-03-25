#include "stdlib.h"
#include "stdio.h"
#include "string.h"

//wikipedia's test : "any carnal pleasure." <==> "YW55IGNhcm5hbCBwbGVhc3VyZS4="
char *test_str = "any carnal pleasure.";

char usage[]="[USAGE]\nTo encode : .exe -e string\nTo decode : .exe -d string\nRun test  : .exe -t";

char table[64]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/**********/
void* Calloc(size_t count, size_t size)
{
	void* p = calloc(count, size);

	if (p == NULL) { printf("Allocation failed\n"); exit(EXIT_FAILURE); };

	return p;
}

/**********/
void Free(void* p)
{
	if (p != NULL) { free(p); };
}

/**********/
char *Encode(char *str)
{
    int src_len = strlen(str);
    char *src = str;

    int dst_len = src_len*8/6;
    dst_len += dst_len%4 == 0 ? 0 : 4 - dst_len%4 ;
    char *dst = Calloc(dst_len+1,sizeof(char));

    char l = 0;
    char r = 0;
    int index = 0;

    for(int i=0,j=0;i<dst_len;i+=4,j+=3)
    {
        //
        index = src[j] >> 2;
        dst[i] = table[index];
          
        //
        l = src[j] & 0x03;
        if(j+1<src_len)
        {
            r = src[j+1] >> 4;
            index = (l<<4) + r;
            dst[i+1] = table[index];
        }
        else
        {
            index = (l<<4);
            dst[i+1] = table[index];
            break;
        }
            
        //
        l = src[j+1] & 0x0F;
        if(j+2<src_len)
        {
            r = src[j+2] >> 6;
            index = (l<<2) + r;
            dst[i+2] = table[index];
        }
        else
        {
            index = (l<<2);
            dst[i+2] = table[index];
            break;
        }
            
        //
        index = src[j+2] & 0x3F;
        dst[i+3] = table[index];
         
    }

    //padding
    if(src_len%3==2) { dst[dst_len-1] = '='; }
    if(src_len%3==1) { dst[dst_len-1] = '='; dst[dst_len-2] = '='; }
    
    return dst;
}

/**********/
int DecodeSymbol(char c)
{
    if(c>=65 && c<=90 ){ return c-65; }//uppercase
    if(c>=97 && c<=122 ){ return c-71; }//lowercase
    if(c>=48 && c<=57 ){ return c+4; }//number
    if(c=='+'){ return 62; }
    if(c=='/'){ return 63; }
    
    return 0;
}

/**********/
char *Decode(char *str)
{
    char l = 0;
    char r = 0;

    char *src = str;
    int src_len = strlen(src);

    int dst_len = src_len * 6 / 8;
    dst_len -= src[src_len-1] == '=' ? 1 : 0 ;
    dst_len -= src[src_len-2] == '=' ? 1 : 0 ;

    char *dst = Calloc(dst_len+1, sizeof(char));

    for(int i=0,j=0;i<src_len;i+=4,j+=3)
    {

        //
        dst[j] = (DecodeSymbol(src[i]) << 2) + (DecodeSymbol(src[i+1]) >> 4);

        //
        l = (DecodeSymbol(src[i+1]) & 0x0F) << 4;
        r = (DecodeSymbol(src[i+2]) & 0xFC) >> 2;

        dst[j+1] =  l + r;

        //
        l = (DecodeSymbol(src[i+2]) & 0x03) << 6;
        r = (DecodeSymbol(src[i+3]) & 0x3F);
        dst[j+2] =  l + r;

    }

    return dst;
}

/**********/
int main(int argc, char **argv)
{ 
    if(argc==2)
    { 
        if(strcmp(argv[1],"-t")==0)
        {
            char *enc_str = Encode(test_str);
            char *dec_str = Decode(enc_str);

            printf("TEST : %s\n",test_str);
            printf("ENC  : %s\n",enc_str);
            printf("DEC  : %s\n",dec_str);

            Free(enc_str);
            Free(dec_str);
        } 
        else
        {
            printf("%s\n",usage);  
        }
    }
    else if(argc==3)
    {
        if(strcmp(argv[1],"-e")==0)
        {
            char *enc_str = Encode(argv[2]);

            printf("IN  : %s\n",argv[2]);

            printf("OUT : %s\n",enc_str);

            Free(enc_str);
        }
        else if(strcmp(argv[1],"-d")==0)
        {
            char *dec_str = Decode(argv[2]);

            printf("IN  : %s\n",argv[2]);

            printf("OUT : %s\n",dec_str);

            Free(dec_str);
        }
        else
        {
            printf("%s\n",usage);  
        }
    }
    else
    {
        printf("%s\n",usage);  
    }
    
    return 0;
}