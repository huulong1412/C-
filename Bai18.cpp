/* 
Tính S(n) = 1 + x^2/2! + x^4/4! + ... + x^(2n)/(2n)!
*/

#include<stdio.h>
#include<conio.h>
#include<math.h>
int main()
{
    int i, n;
    float x, S, T;
    long M, N;
    printf("\nNhap x: ");
    scanf("%f", &x);
    do
    {
    printf("\nNhap n(n >= 0) : ");
    scanf("%d", &n);
      if(n < 0)
      {
          printf("\n N phai >= 0. Xin nhap lai !");
      }

    }while(n < 0);

    S = 1;
    N = 1;
    i = 1;

    while(i <= n)
    {
        T = pow(x, (2 * i ));
        M = i * 2;
        N = N * M * (M - 1);
        S = S + T/N;
        i++;
    }
    printf("\nTong la %f", S);
    getch();
    return 0;
}