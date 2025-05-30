/*
  Sưu tầm bởi @nguyenvanhieuvn
  Thực hành nhiều bài tập hơn tại https://luyencode.net/
*/

#include<stdio.h>
#include<conio.h>

int main()
{
    int n;
    // Nhập n ( n > 0 )
    do
    {
        printf("\nNhap vao n ( n > 0 ): ");
        scanf("%d", &n);

        if (n <= 0)
        {
            printf("\nGia tri ban nhap vao khong hop le. Xin vui long nhap lai! ");
        }

    } while (n <= 0);

    bool CheckTangDan = true;
    int themang = n;
    int ChuSoCuoi = themang % 10;  // lấy ra chữ số cuối
    themang /= 10;

    printf("\nCac chu so cua %d co tang dan tu trai sang phai khong ?\n ", n);
    while(themang != 0)
    {
        int ChuSoKeCuoi = themang % 10;
        themang /= 10;
        if (ChuSoCuoi < ChuSoKeCuoi)
        {
            CheckTangDan = false;
            break;
        }
        else
        {
            ChuSoCuoi = ChuSoKeCuoi;  
        }
    }
    if(CheckTangDan)
        printf("Dung");
    else
        printf("Khong");
    

    getch();
    return 0;
}