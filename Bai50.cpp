/*
  Hãy tìm chữ số đảo ngược của số nguyên dương n
*/

#include<stdio.h>
#include<conio.h>

int main()
{
    int n;
    do
    {
        printf("\nNhap n: ");
        scanf("%d", &n);
    }while(n < 0 && printf("\nLoi: (n >= 0)"));

    printf("\nSo dao nguoc cua %d la: ", n);
    do
    {
        printf("%d", n % 10);
    }while(n /= 10); // n = n / 10; n != 0;


    getch();
    return 0;
}