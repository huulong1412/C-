/*
  Tính T(x, n) = x+ x^2 + x^3 + x^4 + ... + x^n
*/

#include<stdio.h>
#include<conio.h>


double Power_n(double x, long n)
{
    // n >= 0
    double result = 1;
    while(n--)
    {
        result = result * x;
    }
    return result;
}
double qPower_n(double x, long n)
{
    // n >= 0
    double result = 1;
    while(n)
    {
        if(n % 2 == 1)
        {
            result = result * x;
        }
        x = x * x;
        n = n / 2;
    }
    return result;
}
int main()
{
    double x = 3;
    long n = 2;
    double z;
    z = qPower_n(x, n);
    printf("z = %f", z);

    getch();
    return 0;
}