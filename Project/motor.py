import lgpio
import time

# GPIO chân (BCM)
pins = [17, 18, 27, 22]

# Half-step sequence cho 28BYJ-48
half_step_sequence = [
    [1, 0, 0, 0],
    [1, 1, 0, 0],
    [0, 1, 0, 0],
    [0, 1, 1, 0],
    [0, 0, 1, 0],
    [0, 0, 1, 1],
    [0, 0, 0, 1],
    [1, 0, 0, 1]
]

def rotate_motor(direction='forward', delay=0.002):
    h = lgpio.gpiochip_open(0)

    try:
        # Setup output
        for pin in pins:
            lgpio.gpio_claim_output(h, pin, 0)

        # Xác định thứ tự bước
        seq = half_step_sequence if direction == 'forward' else list(reversed(half_step_sequence))

        print(f"Đang quay {'thuận' if direction == 'forward' else 'ngược'}... Nhấn Ctrl+C để dừng.")

        while True:
            for step in seq:
                for i in range(4):
                    lgpio.gpio_write(h, pins[i], step[i])
                time.sleep(delay)

    except KeyboardInterrupt:
        print("\nDừng động cơ.")
    finally:
        # Tắt chân và đóng
        for pin in pins:
            lgpio.gpio_write(h, pin, 0)
        lgpio.gpiochip_close(h)

# Gọi hàm: chọn 'forward' hoặc 'backward'
rotate_motor(direction='forward')
