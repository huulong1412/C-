/*
Tính S(n) = 𝑥^1 + x^2 + x^3 +...+ x^n
*/

#include<stdio.h>
#include<conio.h>

int main()
{
    int i, n;
    float x, T, S;
    i = 1;
    T = 1;
    S = 0;
    printf("\nNhap x: ");
    scanf("%f", &x);

    printf("\nNhap n: ");
    scanf("%d", &n);

    while(i <= n)
    {
        T = T * x;
        S = S + T;
        i++;
    }
    printf("\nTong la %f",S);

    getch();
    return 0;
}