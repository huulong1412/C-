/*
  Tính T(x, n) = 𝑥^n

*/

#include<stdio.h>
#include<conio.h>
#include<math.h>

/*
  Tính T(x, n) = 𝑥^n
  Sử dụng hàm pow() trong thư viện math.h
*/
double Power(double x, long n)
{
    return pow(x, n);
}

int main()
{
    double x = 3;
    long n = 2;
    double z;
    z = Power(x, n);
    printf("z = %f", z);

    getch();
    return 0;
}


/* #include<stdio.h>
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
} */