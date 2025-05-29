/*
  Hãy kiểm tra số nguyên dương n có toàn chữ số lẻ hay không
*/

#include<stdio.h>
#include<conio.h>

int main()
{

    int n;
    do
    {
        printf("\nNhap n(n >= 0): ");
        scanf("%d", &n);
        if(n < 0)
        {
            printf("\nN phai >= 0. Xin nhap lai !"); 
        }
    }while(n < 0);

    
    printf("\nSo %d gom toan cac chu so le hay khong ?\n", n);
    bool Check = true;
    while(n /= 10)
    {
        if((n % 10) % 2 == 0)
        {
            Check = false;
            break;
        }
    }
    if(Check == true)
    {
        printf("Dung");
    }
    else
    {
        printf("Khong");
    }
    getch();
    return 0;
}