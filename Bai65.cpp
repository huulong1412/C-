/* Giải phương trình 𝑎𝑥^2 + 𝑏𝑥 + 𝑐 = 0 */

#include <stdio.h>
#include <conio.h>
#include <math.h>

int main() {
    float a, b, c;

    do {
        printf("\nNhap vao so thuc a (a != 0) = ");
        scanf("%f", &a);
        if (a == 0) {
            printf("\nGia tri nhap vao khong hop le. Xin kiem tra lai !");
        }
    } while (a == 0);

    printf("\nNhap vao so thuc b = ");
    scanf("%f", &b);

    printf("\nNhap vao so thuc c = ");
    scanf("%f", &c);

    // Tính delta
    float delta = b * b - 4 * a * c;

    if (delta < 0) {
        printf("\nPhuong trinh vo nghiem.");
    } else if (delta == 0) {
        // Nghiệm kép
        float x = -b / (2 * a);
        printf("\nPhuong trinh co nghiem kep: x1 = x2 = %.2f", x);
    } else {
        // Hai nghiệm phân biệt
        float x1 = (-b + sqrt(delta)) / (2 * a);
        float x2 = (-b - sqrt(delta)) / (2 * a);
        printf("\nPhuong trinh co hai nghiem phan biet: x1 = %.2f, x2 = %.2f", x1, x2);
    }

    getch();
    return 0;
}