/*
  Giải phương trình 𝑎𝑥 + 𝑏 = 0
  
*/

#include <stdio.h>
#include <conio.h>  
#include <math.h>
int main() {
    float a, b;

    do {
        printf("\nNhap vao so thuc a = ");
        scanf("%f", &a);
        if (a == 0) {
            printf("\nGia tri nhap vao khong hop le. Xin kiem tra lai !");
        }
    } while (a == 0);

    printf("\nNhap vao so thuc b = ");
    scanf("%f", &b);

    // Tính nghiệm
    float x = -b / a;

    printf("\nNghiem cua phuong trinh %.2fx + %.2f = 0 la: x = %.2f", a, b, x);

    getch();
    return 0;
}