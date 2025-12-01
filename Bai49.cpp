/*
  Cho số nguyên dương n. Hãy tìm chữ số đầu tiên của n
*/

#include<stdio.h>
#include<conio.h>

int main()
{
    int themang, n, i;

    do
    {
        printf("\nNhap n: ");
        scanf("%d", &n);
    }while(n < 0 && printf("\nLoi: (n >= 0)"));

    themang = n;
    do
    {
      i = themang % 10;
    }while(themang /= 10);
    
    printf("\nChu so dau tien la %d", i);


    getch();
    return 0;
}