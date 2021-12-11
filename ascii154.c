#include<stdio.h>
#include<conio.h>
int main()
{
   unsigned char i;
   printf("ASCII CHART\n==========================\n");
   for( i=0; i<127; i++)
       {
        printf("\n%d\t=\t%c\n",i,i);
   }
  return 0;
}
