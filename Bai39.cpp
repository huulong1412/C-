/*
Tinh S = ((1! + 2!)^(1/2) + (3! + 4!)^(1/3) + ... + (n! + (n+1)!))^(1/(n+1))
        n >= 1
        n la so nguyen duong
        S la tong
        M la giai thua
        i la bien dem
        n la bien nhap
        S = ((M + S)^(1/(i + 1)))
*/

#include<stdio.h>
#include<conio.h>
#include<math.h>
int main()
{
    int i, n;
    float S, M;
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
    M = 1;
    while(i <= n)
    {
        M = M * i;
        S = pow((M + S) * 1.0, 1.0/(i + 1));
        i++;
    }
    printf("\nTong la %f", S);

    getch();
    return 0;
}