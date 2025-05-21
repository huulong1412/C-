/*
 Tính S = 1 + 1/(1 + 1/(1 + ... + 1))
         n >= 1
         n là số nguyên dương
         S là tổng
         i là biến đếm
         n là biến nhập
         S = 1 + 1/S
*/

#include<stdio.h>
#include<conio.h>

int main()
{
    int i, n;
    float S;
    i = 0;
    S = 1;

    printf("\nNhap n: ");
    scanf("%d", &n);

    while(i <= n)
    {
      S = 1 + 1.0/S;
      i++;
    }
    printf("\nTong la %f", S);
    getch();
    return 0;
}