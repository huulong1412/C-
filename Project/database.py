import sqlite3

def init_db():
    conn = sqlite3.connect('thu_tien.db')
    cursor = conn.cursor()

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS giao_dich (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            ma_hoa_don TEXT,
            so_tien INTEGER,
            thoi_gian TEXT,
            ghi_chu TEXT
        )
    ''')

    cursor.execute('''
        CREATE TABLE IF NOT EXISTS lich_su (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            hanh_dong TEXT,
            ma_hoa_don TEXT,
            so_tien INTEGER,
            thoi_gian TEXT,
            ghi_chu TEXT
        )
    ''')

    conn.commit()
    conn.close()
