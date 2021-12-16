#include <stdio.h>
#include <math.h>
#include <string.h>

int main()
{
    char hex[24];
    long decimal,place;
    int val, len,bin,j=0,binary=0;
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





    while(decimal > 0)
    {
        int rem = decimal % 2;

        binary = (rem * place) + binary;

        decimal /= 2;

        place *= 10;
    }



printf("Hexadecimal number = %s\n", hex);

printf("Binary number = %d\n", binary);

while(j=8){
while(binary>0){

        bin=binary%10;
        binary=binary/10;

        if(bin==0){
            printf("B1b%d  is not set\n",j);
        }
        else{
            printf("B1b%d is set\n",j);
        }

 j--;
}


}



return 0;
}

