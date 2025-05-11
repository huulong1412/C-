# Đầu file: import
import tkinter as tk
from tkinter import messagebox
import sqlite3
from datetime import datetime
import csv
import os
import lgpio
import time
import threading
from database import init_db

DB_NAME = 'thu_tien.db'

# Khởi tạo GPIO
# Ngăn trên
IN1_A, IN2_A, IN3_A, IN4_A = 17, 18, 27, 22
LIMIT_SWITCH_PIN = 23

# Ngăn dưới
IN1_B, IN2_B, IN3_B, IN4_B = 5, 6, 13, 19
LIMIT_SWITCH_PIN_B = 24


h = lgpio.gpiochip_open(0)

for pin in (IN1_A, IN2_A, IN3_A, IN4_A):
    lgpio.gpio_claim_output(h, pin, 0)
lgpio.gpio_claim_input(h, LIMIT_SWITCH_PIN)

for pin in (IN1_B, IN2_B, IN3_B, IN4_B):
    lgpio.gpio_claim_output(h, pin, 0)
lgpio.gpio_claim_input(h, LIMIT_SWITCH_PIN_B)


# Chuỗi bước cho quay thuận (clockwise)
SEQ_CW = [
    [1, 0, 0, 1],
    [1, 0, 0, 0],
    [1, 1, 0, 0],
    [0, 1, 0, 0],
    [0, 1, 1, 0],
    [0, 0, 1, 0],
    [0, 0, 1, 1],
    [0, 0, 0, 1],
]

# Chuỗi bước cho quay ngược (counter-clockwise)
SEQ_CCW = list(reversed(SEQ_CW))

def step_motor(steps, direction=1, delay=0.002, motor='A'):
    if motor == 'A':
        pins = (IN1_A, IN2_A, IN3_A, IN4_A)
    else:
        pins = (IN1_B, IN2_B, IN3_B, IN4_B)

    seq = SEQ_CW if direction == 1 else SEQ_CCW
    seq_len = len(seq)

    for i in range(steps):
        step = seq[i % seq_len]
        for pin, val in zip(pins, step):
            lgpio.gpio_write(h, pin, val)
        time.sleep(delay)

    for pin in pins:
        lgpio.gpio_write(h, pin, 0)



def open_drawer_A():
    print("Đang mở tủ trên...")
    threading.Thread(target=lambda: step_motor(4096*5, direction=1, delay=0.0015, motor='A')).start()

def close_drawer_A():
    def run():
        print("Đang đóng tủ trên...")
        while True:
            if lgpio.gpio_read(h, LIMIT_SWITCH_PIN) == 1:
                print("Tủ trên đã đóng.")
                break
            step_motor(4096, direction=0, delay=0.002, motor='A')
            time.sleep(0.01)
        for pin in (IN1_A, IN2_A, IN3_A, IN4_A):
            lgpio.gpio_write(h, pin, 0)
    threading.Thread(target=run).start()

def open_drawer_B():
    print("Đang mở tủ dưới...")
    threading.Thread(target=lambda: step_motor(4096*5, direction=1, delay=0.0015, motor='B')).start()

def close_drawer_B():
    def run():
        print("Đang đóng tủ dưới...")
        while True:
            if lgpio.gpio_read(h, LIMIT_SWITCH_PIN_B) == 1:
                print("Tủ dưới đã đóng.")
                break
            step_motor(4096, direction=0, delay=0.002, motor='B')
            time.sleep(0.01)
        for pin in (IN1_B, IN2_B, IN3_B, IN4_B):
            lgpio.gpio_write(h, pin, 0)
    threading.Thread(target=run).start()




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
    if value.isdigit():
        formatted = "{:,}".format(int(value)).replace(",", ".")
        entry_so_tien.delete(0, tk.END)
        entry_so_tien.insert(0, formatted)

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
        tk.Label(scrollable_frame, text=h, font=('Arial', 10, 'bold')).grid(row=0, column=i, sticky="w")

    total = 0
    for i, row in enumerate(data):
        id_row, ma_hd, so_tien, thoi_gian, ghi_chu = row
        so_tien_fmt = "{:,}".format(so_tien).replace(",", ".")
        total += so_tien

        tk.Label(scrollable_frame, text=ma_hd).grid(row=i+1, column=0, sticky="w")
        tk.Label(scrollable_frame, text=so_tien_fmt).grid(row=i+1, column=1, sticky="w")
        tk.Label(scrollable_frame, text=thoi_gian).grid(row=i+1, column=2, sticky="w")
        tk.Label(scrollable_frame, text=ghi_chu).grid(row=i+1, column=3, sticky="w")
        tk.Button(scrollable_frame, text="X", command=lambda id=id_row: delete_row(id)).grid(row=i+1, column=4)

    total_label.config(text=f"Tổng tiền: {total:,} VND")
    conn.close()

import random  # thêm ở đầu file

def on_submit():
    # Kiểm tra trạng thái công tắc hành trình
    dang_mo_A = lgpio.gpio_read(h, LIMIT_SWITCH_PIN) == 0  # tủ trên chưa nhấn => đang mở
    dang_mo_B = lgpio.gpio_read(h, LIMIT_SWITCH_PIN_B) == 0  # tủ dưới chưa nhấn => đang mở

    def mo_tu_con_lai():
        if random.choice([True, False]):
            open_drawer_A()
        else:
            open_drawer_B()

    # Nếu một trong hai tủ đang mở, đóng nó rồi mới mở tủ còn lại
    if dang_mo_A:
        close_drawer_A()
        threading.Timer(3, open_drawer_B).start()  # chờ 3 giây rồi mở tủ dưới
    elif dang_mo_B:
        close_drawer_B()
        threading.Timer(3, open_drawer_A).start()  # chờ 3 giây rồi mở tủ trên
    else:
        mo_tu_con_lai()  # không tủ nào mở => mở ngẫu nhiên

    # Xử lý nhập liệu
    ma_hd = entry_ma_hd.get().strip()
    so_tien_str = entry_so_tien.get().strip().replace(".", "")

    if not ma_hd or not so_tien_str.isdigit():
        messagebox.showerror("Lỗi", "Vui lòng nhập đúng mã hóa đơn và số tiền.")
        return

    ghi_chu = entry_ghi_chu.get().strip()
    insert_data(ma_hd, int(so_tien_str), ghi_chu)

    # Xóa trường nhập sau khi lưu
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



# --- Giao diện ---
init_db()
root = tk.Tk()
root.title("Máy thu tiền")

top_frame = tk.Frame(root)
top_frame.pack(pady=10)

table_frame = tk.Frame(root)
table_frame.pack(pady=10)

canvas = tk.Canvas(table_frame, height=200, width=800)
scrollbar = tk.Scrollbar(table_frame, orient="vertical", command=canvas.yview)
scrollable_frame = tk.Frame(canvas)

scrollable_frame.bind("<Configure>", lambda e: canvas.configure(scrollregion=canvas.bbox("all")))
canvas.create_window((0, 0), window=scrollable_frame, anchor="nw")
canvas.configure(yscrollcommand=scrollbar.set)

canvas.pack(side="left", fill="both", expand=True)
scrollbar.pack(side="right", fill="y")
def _on_mousewheel(event):
    canvas.yview_scroll(int(-1*(event.delta/120)), "units")

canvas.bind_all("<MouseWheel>", _on_mousewheel)
canvas.bind_all("<Button-4>", lambda e: canvas.yview_scroll(-1, "units"))   
canvas.bind_all("<Button-5>", lambda e: canvas.yview_scroll(1, "units"))

canvas.bind_all("<MouseWheel>", lambda e: canvas.yview_scroll(-1 * int(e.delta / 120), "units"))

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
tk.Button(top_frame, text="Mở tủ trên", command=open_drawer_A).grid(row=4, column=0, pady=5)
tk.Button(top_frame, text="Đóng tủ trên", command=close_drawer_A).grid(row=4, column=1, pady=5)
tk.Button(top_frame, text="Mở tủ dưới", command=open_drawer_B).grid(row=5, column=0, pady=5)
tk.Button(top_frame, text="Đóng tủ dưới", command=close_drawer_B).grid(row=5, column=1, pady=5)

bottom_frame = tk.Frame(root)
bottom_frame.pack(pady=10)

total_label = tk.Label(bottom_frame, text="Tổng tiền: 0 VND", font=('Arial', 12, 'bold'))
total_label.pack()

refresh_table()
root.mainloop()