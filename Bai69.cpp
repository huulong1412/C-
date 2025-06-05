/*
  Tính S(x, n) = 𝑥 − 𝑥^3 + 𝑥^5 + ⋯ + (−1)^n x 𝑥^(2𝑛+1)
*/

#include<stdio.h>
#include<conio.h>
#include<math.h>
int main()
{
    int i, n;
    float x, T, S;
    i = 0;
    T = 1;
    S = 0;
    printf("\nNhap x: ");
    scanf("%f", &x);
    //x = -x;
    printf("\nNhap n: ");
    scanf("%d", &n);

    while(i < n)
    {
        T = pow(x, (2 * i + 1)) * pow(-1, (float)i);
        S = S + T;
        i++;
    }
    printf("\nTong la %f", S);

    getch();
    return 0;
}