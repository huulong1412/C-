/* 
    Hãy đêm số lượng chữ số đầu tiên của số nguyên dương n 
*/
#include<stdio.h>
#include<conio.h>   
int main()
{
    int i, n;
    int first, count;
    count = 0;
    do
    {
        printf("\nNhap n: ");
        scanf("%d", &n);
    } while(n < 0 && printf("\nLoi: n >= 0 !"));
    
    first = n % 10;  // khởi tạo first
    if(n == 0)
        first = 0;

    do
    {
        i = n % 10;
        if(i == first)
            count++;
        if(i > first)
            first = i;
    } while(n /= 10);

    printf("\nChu so dau tien la %d", first);
    printf("\nSo luong chu so dau tien la %d", count);

    getch();
    return 0;
}