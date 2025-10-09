/* Bài 88: Hãy sử dụng vòng lặp for để xuất tất cả các ký tự từ A đến Z */

#include<stdio.h>
#include<conio.h>

int main()
{
    char kytu;
    for(kytu = 'A'; kytu <= 'Z'; kytu++)
    {
        printf("%4c", kytu);
    }
    getch();
    return 0;
}							
						