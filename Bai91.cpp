/* Bài 91: In tất cả các số nguyên dương lẻ nhỏ hơn 100 */

#include<stdio.h>
#include<conio.h>

int main()
{
    int i = 1;
    while(i < 100)
    {
        printf("%5d", i);
        i = i + 2;
    }
    getch();
    return 0;
}					