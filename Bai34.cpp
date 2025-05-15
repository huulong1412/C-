/*
  Tính S(n) = √𝑛 + √𝑛 − 1 + √𝑛 − 2 + ⋯ √2 + √1 có n dấu căn.
*/

#include<stdio.h>
#include<conio.h>
#include<math.h>
int main()
{
    int i, n;
    float S;
    do
    {
        printf("\nNhap n(n >= 1): ");
        scanf("%d", &n);

        if(n < 1)
        {
            printf("\nn phai >= 1. Xin nhap lai !");
        }
    }while(n < 1);

    i = 1;
    S = 0;

    while(i <= n)
    {
        S = sqrt(i + S);
        i++;
    }
    printf("\nTong S = %f", S);

    getch();
    return 0;
}