#include <gtk/gtk.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

sqlite3 *db;
GtkWidget *entry_ma_hd, *entry_so_tien, *entry_ghi_chu, *total_label;
GtkWidget *scrollable_frame;

#define DB_NAME "thu_tien.db"

// Hàm kết nối và khởi tạo cơ sở dữ liệu
int init_db() {
    int rc = sqlite3_open(DB_NAME, &db);
    if (rc) {
        g_print("Không thể mở cơ sở dữ liệu: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    const char *sql = 
        "CREATE TABLE IF NOT EXISTS giao_dich ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "ma_hoa_don TEXT, "
        "so_tien INTEGER, "
        "thoi_gian TEXT, "
        "ghi_chu TEXT); "
        "CREATE TABLE IF NOT EXISTS lich_su ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "hanh_dong TEXT, "
        "ma_hoa_don TEXT, "
        "so_tien INTEGER, "
        "thoi_gian TEXT, "
        "ghi_chu TEXT);";
    char *err_msg = 0;
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        g_print("Lỗi khi tạo bảng: %s\n", err_msg);
        sqlite3_free(err_msg);
        return rc;
    }
    return 0;
}

// Hàm chèn dữ liệu vào cơ sở dữ liệu
void insert_data(const char *ma_hd, int so_tien, const char *ghi_chu) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO giao_dich (ma_hoa_don, so_tien, thoi_gian, ghi_chu) VALUES (?, ?, datetime('now'), ?)";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, ma_hd, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, so_tien);
    sqlite3_bind_text(stmt, 3, ghi_chu, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Lưu vào lịch sử
    const char *sql_history = "INSERT INTO lich_su (hanh_dong, ma_hoa_don, so_tien, thoi_gian, ghi_chu) VALUES ('THÊM', ?, ?, datetime('now'), ?)";
    sqlite3_prepare_v2(db, sql_history, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, ma_hd, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, so_tien);
    sqlite3_bind_text(stmt, 3, ghi_chu, -1, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

// Hàm xóa dữ liệu
void delete_row(int id_row) {
    sqlite3_stmt *stmt;

    // Lấy thông tin mã hóa đơn và số tiền
    const char *sql_select = "SELECT ma_hoa_don, so_tien FROM giao_dich WHERE id = ?";
    sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id_row);
    sqlite3_step(stmt);
    const char *ma_hd = (const char *)sqlite3_column_text(stmt, 0);
    int so_tien = sqlite3_column_int(stmt, 1);
    sqlite3_finalize(stmt);

    // Lưu vào lịch sử
    const char *sql_history = "INSERT INTO lich_su (hanh_dong, ma_hoa_don, so_tien, thoi_gian, ghi_chu) VALUES ('XÓA', ?, ?, datetime('now'), '')";
    sqlite3_prepare_v2(db, sql_history, -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, ma_hd, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, so_tien);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Xóa giao dịch
    const char *sql_delete = "DELETE FROM giao_dich WHERE id = ?";
    sqlite3_prepare_v2(db, sql_delete, -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id_row);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

// Hàm làm mới bảng
void refresh_table() {
    // Xóa dữ liệu trong frame cũ
    GList *children = gtk_container_get_children(GTK_CONTAINER(scrollable_frame));
    for (GList *l = children; l != NULL; l = l->next) {
        gtk_widget_destroy(GTK_WIDGET(l->data));
    }
    g_list_free(children);

    const char *sql = "SELECT * FROM giao_dich";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    int row = 0;
    int total = 0;

    // Duyệt qua dữ liệu và hiển thị
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *ma_hd = (const char *)sqlite3_column_text(stmt, 1);
        int so_tien = sqlite3_column_int(stmt, 2);
        const char *thoi_gian = (const char *)sqlite3_column_text(stmt, 3);
        const char *ghi_chu = (const char *)sqlite3_column_text(stmt, 4);

        GtkWidget *label_ma_hd = gtk_label_new(ma_hd);
        GtkWidget *label_so_tien = gtk_label_new(g_strdup_printf("%d", so_tien));
        GtkWidget *label_thoi_gian = gtk_label_new(thoi_gian);
        GtkWidget *label_ghi_chu = gtk_label_new(ghi_chu);

        // Thêm vào table
        gtk_grid_attach(GTK_GRID(scrollable_frame), label_ma_hd, 0, row, 1, 1);
        gtk_grid_attach(GTK_GRID(scrollable_frame), label_so_tien, 1, row, 1, 1);
        gtk_grid_attach(GTK_GRID(scrollable_frame), label_thoi_gian, 2, row, 1, 1);
        gtk_grid_attach(GTK_GRID(scrollable_frame), label_ghi_chu, 3, row, 1, 1);

        // Cập nhật tổng tiền
        total += so_tien;
        row++;
    }

    sqlite3_finalize(stmt);
    gtk_label_set_text(GTK_LABEL(total_label), g_strdup_printf("Tổng tiền: %d VND", total));
}

// Hàm xử lý sự kiện khi người dùng nhập dữ liệu
void on_submit(GtkWidget *widget, gpointer data) {
    const char *ma_hd = gtk_entry_get_text(GTK_ENTRY(entry_ma_hd));
    const char *so_tien_str = gtk_entry_get_text(GTK_ENTRY(entry_so_tien));
    const char *ghi_chu = gtk_entry_get_text(GTK_ENTRY(entry_ghi_chu));

    int so_tien = atoi(so_tien_str);
    if (strlen(ma_hd) == 0 || so_tien == 0) {
        g_print("Vui lòng nhập đúng mã hóa đơn và số tiền.\n");
        return;
    }

    insert_data(ma_hd, so_tien, ghi_chu);
    gtk_entry_set_text(GTK_ENTRY(entry_ma_hd), "");
    gtk_entry_set_text(GTK_ENTRY(entry_so_tien), "");
    gtk_entry_set_text(GTK_ENTRY(entry_ghi_chu), "");
    refresh_table();
}

// Hàm xuất dữ liệu và reset
void export_and_reset(GtkWidget *widget, gpointer data) {
    const char *filename = "xuat_du_lieu.csv";
    FILE *f = fopen(filename, "w");

    if (!f) {
        g_print("Không thể tạo file xuất.\n");
        return;
    }

    fprintf(f, "ID,Mã hóa đơn,Số tiền,Thời gian,Ghi chú\n");

    const char *sql = "SELECT * FROM giao_dich";
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id_row = sqlite3_column_int(stmt, 0);
        const char *ma_hd = (const char *)sqlite3_column_text(stmt, 1);
        int so_tien = sqlite3_column_int(stmt, 2);
        const char *thoi_gian = (const char *)sqlite3_column_text(stmt, 3);
        const char *ghi_chu = (const char *)sqlite3_column_text(stmt, 4);
        fprintf(f, "%d,%s,%d,%s,%s\n", id_row, ma_hd, so_tien, thoi_gian, ghi_chu);
    }

    sqlite3_finalize(stmt);
    fclose(f);

    // Xóa bảng giao dịch
    const char *sql_delete = "DELETE FROM giao_dich";
    sqlite3_exec(db, sql_delete, 0, 0, 0);

    refresh_table();
    g_print("Dữ liệu đã được xuất vào file %s\n", filename);
}

// Hàm main
int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    init_db();

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Máy thu tiền");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    // Tạo form nhập liệu
    GtkWidget *grid = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 10);

    GtkWidget *label_ma_hd = gtk_label_new("Mã hóa đơn:");
    entry_ma_hd = gtk_entry_new();
    GtkWidget *label_so_tien = gtk_label_new("Số tiền:");
    entry_so_tien = gtk_entry_new();
    GtkWidget *label_ghi_chu = gtk_label_new("Ghi chú:");
    entry_ghi_chu = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), label_ma_hd, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_ma_hd, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_so_tien, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_so_tien, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), label_ghi_chu, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_ghi_chu, 1, 2, 1, 1);

    // Button nhập
    GtkWidget *button_submit = gtk_button_new_with_label("Nhập");
    g_signal_connect(button_submit, "clicked", G_CALLBACK(on_submit), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_submit, 0, 3, 2, 1);

    // Button xuất và reset
    GtkWidget *button_export_reset = gtk_button_new_with_label("Xuất dữ liệu và reset");
    g_signal_connect(button_export_reset, "clicked", G_CALLBACK(export_and_reset), NULL);
    gtk_grid_attach(GTK_GRID(grid), button_export_reset, 0, 4, 2, 1);

    // Label tổng tiền
    total_label = gtk_label_new("Tổng tiền: 0 VND");
    gtk_box_pack_start(GTK_BOX(vbox), total_label, FALSE, FALSE, 10);

    // Frame cho bảng dữ liệu
    scrollable_frame = gtk_grid_new();
    gtk_box_pack_start(GTK_BOX(vbox), scrollable_frame, TRUE, TRUE, 10);

    refresh_table();

    gtk_widget_show_all(window);
    gtk_main();

    sqlite3_close(db);
    return 0;
}
