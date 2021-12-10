#include<stdio.h>
#include<conio.h>
char const* character[]={"NULL","SOH","STX", "ETX", "EOT", "ENQ", "ACK",
                         "BEL","BS","TAB","LF","VT","FF","CR","SO", "SI",
                         "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN",
                         "ETB","CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US","SPA"};
int main()
{
    char c;
    int row;
    printf("ASCII CHART\n==========================\n");
    for(int i=0; i<33; i++)
        {
        row=i;
        while(row<=127){
            if(row<33)
                printf("\t%d=%s\t",row,character[i]);

            else if(row>=33 && row<127){
                    c=row;
                    printf("\t%d=%c\t",row,c);
            }

            else
                printf("\t%d=DEL\t",row);
            row=row+33;
        }
        printf("\n");
}
}
