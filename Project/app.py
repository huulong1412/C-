# file: app.py
import tkinter as tk
from tkinter import messagebox
import sqlite3
from datetime import datetime
from database import init_db
import csv
import os

DB_NAME = 'thu_tien.db'

def insert_data(ma_hd, so_tien, ghi_chu):
    conn = sqlite3.connect(DB_NAME)
    cursor = conn.cursor()
    thoi_gian = datetime.now().strftime('%Y-%m-%d %H:%M:%S')

    cursor.execute('INSERT INTO giao_dich (ma_hoa_don, so_tien, thoi_gian, ghi_chu) VALUES (?, ?, ?, ?)', 
                   (ma_hd, so_tien, thoi_gian, ghi_chu))
    cursor.execute('INSERT INTO lich_su (hanh_dong, ma_hoa_don, so_tien, thoi_gian, ghi_chu) VALUES (?, ?, ?, ?, ?)',
                   ('THÊM', ma_hd, so_tien, thoi_gian, ghi_chu))

    conn.commit()
    conn.close()
    refresh_table()

def dinh_dang_so_tien(event):
    value = entry_so_tien.get().replace(".", "")
    if not value.isdigit() and value != "":
        entry_so_tien.delete(0, tk.END)
        return

    try:
        number = int(value)
        formatted = "{:,}".format(number).replace(",", ".")
        entry_so_tien.delete(0, tk.END)
        entry_so_tien.insert(0, formatted)
    except ValueError:
        pass

def delete_row(id_row):
    conn = sqlite3.connect(DB_NAME)
    cursor = conn.cursor()

    cursor.execute('SELECT ma_hoa_don, so_tien FROM giao_dich WHERE id = ?', (id_row,))
    row = cursor.fetchone()

    if row:
        ma_hd, so_tien = row
        thoi_gian = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        cursor.execute('INSERT INTO lich_su (hanh_dong, ma_hoa_don, so_tien, thoi_gian, ghi_chu) VALUES (?, ?, ?, ?, ?)',
                       ('XÓA', ma_hd, so_tien, thoi_gian, ''))

    cursor.execute('DELETE FROM giao_dich WHERE id = ?', (id_row,))
    conn.commit()
    conn.close()
    refresh_table()

def refresh_table():
    for widget in scrollable_frame.winfo_children():
        widget.destroy()

    conn = sqlite3.connect(DB_NAME)
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM giao_dich')
    data = cursor.fetchall()

    headers = ['Mã HĐ', 'Số tiền', 'Thời gian', 'Ghi chú', 'Xóa']
    for i, h in enumerate(headers):
        tk.Label(scrollable_frame, text=h, font=('Arial', 10, 'bold'), width=18).grid(row=0, column=i, sticky="w")

    total = 0
    for i, row in enumerate(data):
        id_row, ma_hd, so_tien, thoi_gian, ghi_chu = row
        so_tien_fmt = "{:,}".format(so_tien).replace(",", ".")
        total += so_tien

        tk.Label(scrollable_frame, text=ma_hd, width=18).grid(row=i+1, column=0, sticky="w")
        tk.Label(scrollable_frame, text=so_tien_fmt, width=18).grid(row=i+1, column=1, sticky="w")
        tk.Label(scrollable_frame, text=thoi_gian, width=22).grid(row=i+1, column=2, sticky="w")
        tk.Label(scrollable_frame, text=ghi_chu, width=22).grid(row=i+1, column=3, sticky="w")
        tk.Button(scrollable_frame, text="X", command=lambda id=id_row: delete_row(id)).grid(row=i+1, column=4)

    total_label.config(text=f"Tổng tiền: {total:,} VND")
    conn.close()

def on_submit():
    ma_hd = entry_ma_hd.get().strip()
    so_tien_str = entry_so_tien.get().strip().replace(".", "")

    if not ma_hd or not so_tien_str.isdigit():
        messagebox.showerror("Lỗi", "Vui lòng nhập đúng mã hóa đơn và số tiền.")
        return

    ghi_chu = entry_ghi_chu.get().strip()
    insert_data(ma_hd, int(so_tien_str), ghi_chu)
    entry_ma_hd.delete(0, tk.END)
    entry_so_tien.delete(0, tk.END)
    entry_ghi_chu.delete(0, tk.END)

def export_and_reset():
    conn = sqlite3.connect(DB_NAME)
    cursor = conn.cursor()
    cursor.execute('SELECT * FROM giao_dich')
    data = cursor.fetchall()

    if not data:
        messagebox.showinfo("Thông báo", "Không có dữ liệu để xuất.")
        conn.close()
        return

    export_dir = "xuat_du_lieu"
    os.makedirs(export_dir, exist_ok=True)
    timestamp = datetime.now().strftime('%Y%m%d_%H%M%S')
    filename = f"{export_dir}/du_lieu_{timestamp}.csv"

    with open(filename, mode='w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(["ID", "Mã hóa đơn", "Số tiền", "Thời gian", "Ghi chú"])
        for row in data:
            id_row, ma_hd, so_tien, thoi_gian, ghi_chu = row
            so_tien_fmt = "{:,}".format(so_tien).replace(",", ".")
            writer.writerow([id_row, ma_hd, so_tien_fmt, thoi_gian, ghi_chu])

    thoi_gian = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    cursor.execute('INSERT INTO lich_su (hanh_dong, ma_hoa_don, so_tien, thoi_gian, ghi_chu) VALUES (?, ?, ?, ?, ?)',
                   ('XUẤT_DỮ_LIỆU', '', 0, thoi_gian, f"Xuất {len(data)} dòng vào {filename}"))

    cursor.execute('DELETE FROM giao_dich')

    conn.commit()
    conn.close()
    refresh_table()
    messagebox.showinfo("Xuất thành công", f"Đã lưu: {filename}")

# Khởi tạo DB
init_db()

# Giao diện
root = tk.Tk()
root.title("Máy thu tiền")

top_frame = tk.Frame(root)
top_frame.pack(pady=10)

table_frame = tk.Frame(root)
table_frame.pack(pady=10)

canvas = tk.Canvas(table_frame, height=200, width=900)
scrollbar = tk.Scrollbar(table_frame, orient="vertical", command=canvas.yview)
x_scrollbar = tk.Scrollbar(table_frame, orient="horizontal", command=canvas.xview)

scrollable_frame = tk.Frame(canvas)

scrollable_frame.bind(
    "<Configure>",
    lambda e: canvas.configure(
        scrollregion=canvas.bbox("all")
    )
)

def _on_mousewheel(event):
    canvas.yview_scroll(int(-1*(event.delta/120)), "units")

canvas.bind_all("<MouseWheel>", _on_mousewheel)
canvas.bind("<Button-4>", lambda event: canvas.yview_scroll(-1, "units"))
canvas.bind("<Button-5>", lambda event: canvas.yview_scroll(1, "units"))


canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
canvas.configure(yscrollcommand=scrollbar.set, xscrollcommand=x_scrollbar.set)

canvas.pack(side="left", fill="both", expand=True)
scrollbar.pack(side="right", fill="y")
x_scrollbar.pack(side="bottom", fill="x")

tk.Label(top_frame, text="Mã hóa đơn:").grid(row=0, column=0)
entry_ma_hd = tk.Entry(top_frame)
entry_ma_hd.grid(row=0, column=1)
entry_ma_hd.bind("<Return>", lambda event: on_submit())

tk.Label(top_frame, text="Số tiền:").grid(row=1, column=0)
entry_so_tien = tk.Entry(top_frame)
entry_so_tien.grid(row=1, column=1)
entry_so_tien.bind("<KeyRelease>", dinh_dang_so_tien)
entry_so_tien.bind("<Return>", lambda event: on_submit())

tk.Label(top_frame, text="Ghi chú:").grid(row=2, column=0)
entry_ghi_chu = tk.Entry(top_frame)
entry_ghi_chu.grid(row=2, column=1)
entry_ghi_chu.bind("<Return>", lambda event: on_submit())

tk.Button(top_frame, text="Nhập", command=on_submit).grid(row=3, column=0, pady=10)
tk.Button(top_frame, text="Xuất dữ liệu và reset", command=export_and_reset).grid(row=3, column=1, pady=10)

bottom_frame = tk.Frame(root)
bottom_frame.pack(pady=10)

total_label = tk.Label(bottom_frame, text="Tổng tiền: 0 VND", font=('Arial', 12, 'bold'))
total_label.pack()

refresh_table()
root.mainloop()
