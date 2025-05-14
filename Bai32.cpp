/*
  Cho số nguyên dương n. Kiểm tra số nguyên dương n có phải là số
chính phương hay không?
Số chính phương là số mà kết quả khai căn bậc 2 là 1 số nguyên*/

#include<stdio.h>
#include<conio.h>
#include<math.h>
int main()
{
    int n;
    do
    {
    printf("\nNhap n: ");
    scanf("%d", &n);

       if(n <= 0)
           printf("\nn phai > 0. Xin nhap lai !");

    }while(n <= 0);
    
    if (sqrt((float)n) == (int)sqrt((float)n))  // So sánh 2 số khi chưa ép về kiểu nguyên và số đã ép về kiểu nguyên
    {
        printf("\n%d La so chinh phuong", n);
    }
    else
    {
        printf("\n%d Khong la so chinh phuong", n);
    }

    getch();
    return 0;
}