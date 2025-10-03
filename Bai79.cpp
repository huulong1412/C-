/* Bài 79: Hãy đếm số lượng chữ số của số nguyên dương n */


#include<stdio.h>
#include<conio.h>

int main()
{
    long int n, t;
    int dem;
    printf("\nNhap n: ");
    scanf("%d", &n);
    dem = 0;
    t = n;
    while(t != 0)
    {
        dem++;
        t /= 10;
    }
    printf("\n%d", dem);
    getch();
    return 0;
}							