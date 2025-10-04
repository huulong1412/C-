/* Bài 82: Viết chương trình tìm số lớn nhất trong 3 số thực a, b, c  */ 

#include<stdio.h>
#include<conio.h>

int main()
{
    float a, b, c, max;
    printf("\nNhap a: ");
    scanf("%f", &a);

    printf("\nNhap b: ");
    scanf("%f", &b);

    printf("\nNhap c: ");
    scanf("%f", &c);

    max = a;
    if(max < b)
        max = b;
    if(max < c)
        max = c;
    printf("\nSo lon nhat trong 3 so %10.3f, %10.3f, %10.3f, la: %10.3f", a, b, c, max);


    getch();
    return 0;

}			