/*
  Cho n là số nguyên dương. Hãy tìm giá trị nguyên dương k lớn nhất sao
cho S(k) < n. Trong đó chuỗi S(k) được định nghĩa như sau : S(k) = 1 +
2 + 3 + … + k
  S(k) là tổng của k số nguyên dương đầu tiên.
  n là số nguyên dương
  S là tổng
  i là biến đếm
  n là biến nhập
  S = S + i
*/

#include<stdio.h>
#include<conio.h>

int main()
{
    int i, n;
    float S;
    i = 0;
    S = 0;
    do
    {
    printf("\nNhap n: ");
    scanf("%d", &n);
    }while(n < 0 && printf("Error: (n >= 0)\n"));
    while(S + i < n)
    {
        i++;    
        S = S + i;  
    }
        printf("\nGia tri k lon nhat de S(k) = 1 + 2 + ... + k < n la %d", i);
    getch();
    return 0;
}