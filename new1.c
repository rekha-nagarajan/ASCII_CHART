#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include<string.h>
int main()
{
char data[1000];
int i;
FILE *fPtr;
FILE *fPtr1;
FILE *fPtr2;

fPtr=fopen("fil.text","w");
if(fPtr==NULL)
    {
     printf("Unable to create file.\n");
     exit(1);
    }
else{

    char hex[24];
    long decimal,place;
    int val, len,bin,j=0;
    int binary=0;
    decimal = 0;
    place = 1;

    printf("Enter any hexadecimal number: ");
    gets(hex);
    len = strlen(hex);
    len--;

    for(int i=0; hex[i]!='\0'; i++)
    {

        if(hex[i]>='0' && hex[i]<='9')
        {
            val = hex[i] - 48;
        }
        else if(hex[i]>='a' && hex[i]<='f')
        {
            val = hex[i] - 97 + 10;
        }
        else if(hex[i]>='A' && hex[i]<='F')
        {
            val = hex[i] - 65 + 10;
        }

        decimal += val * pow(16, len);
        len--;
    }
    int n,c,k;
    for(c=16;c>=0;c--)
    {
        k=decimal>>c;
        binary=k;
    if(binary & 1)
            {
            fprintf(fPtr,"1");
         printf("1");
         }
        else{
            fprintf(fPtr,"0");
             printf("0");}
    }




fclose(fPtr);


return 0;
}
}


/* read.c 8*/

#include "stdio.h"


int main() {

	FILE *fptr = fopen("fil.text","r");
    FILE *fptr1 = fopen("fi.text","w");

	int i =1;
	char single,bin;

	while ( (single = fgetc(fptr) ) != EOF) {




            if(single=='0'){
                   fprintf(fptr1,"\t %c B1b%d is not set\n",single,i);
                   printf("\t %c B1b%d is not set\n",single,i);
		      }
            else{
                   fprintf(fptr1,"\t %c B1b%d is  set\n",single,i);
                   printf("\t %c B1b%d is  set\n",single,i);
        }
i++;
}
	fclose(fptr);
	fclose(fptr1);
    return 0;
	}






