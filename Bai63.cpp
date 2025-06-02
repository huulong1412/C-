/*
  Cho hai số nguyên dương a và b. Hãy vẽ lưu đồ tìm bội chung nhỏ nhất
của hai giá trị này
*/

#include<stdio.h>
#include<conio.h>   
#include<math.h>
int main()
{
    int a, b;

    do {
        printf("\nNhap vao so nguyen duong a = ");
        scanf("%4d", &a);
        if (a < 0) {
            printf("\nGia tri nhap vao khong hop le. Xin kiem tra lai !");
        }
    } while (a < 0);

    do {
        printf("\nNhap vao so nguyen duong b = ");
        scanf("%4d", &b);
        if (b < 0) {
            printf("\nGia tri nhap vao khong hop le. Xin kiem tra lai !");
        }
    } while (b < 0);

    int Max = a > b ? a : b;
    int Min = a < b ? a : b;

    // Cách 1: Tính USCLN trước
    int USCLN = 1;
    for (int i = Min; i >= 1; i--) {
        if (Max % i == 0 && Min % i == 0) {
            USCLN = i;
            break;
        }
    }

    // Tính Bội chung nhỏ nhất
    int BCNN = (Max * Min) / USCLN;
    
    printf("\nBCNN(%d, %d) = %d", a, b, BCNN);

    getch();
    return 0;
}