# 04 - UART Test Guide

## 1. Mục tiêu

Test giao tiếp UART giữa laptop và STM32F401CCU6 Blackpill:

1. Laptop gửi frame UART xuống STM32.
2. STM32 parse frame đúng.
3. Test `PING` và ACK.
4. Test relay command bằng LED/module relay.
5. Sau UART + relay ổn mới test các task khác.

## 2. Thiết bị cần có

| Thiết bị | Ghi chú |
|---|---|
| STM32F401CCU6 Blackpill | Kit MCU |
| ST-LINK | Nạp firmware/debug |
| FT232RL USB To UART Type-C | Chuyển USB laptop sang UART TTL |
| Dây Dupont | TX/RX/GND |
| LED + điện trở 220Ω/330Ω | Test GPIO relay an toàn |
| Laptop | Gửi frame UART |

## 3. Nguyên tắc an toàn

- Chỉ dùng UART TTL 3.3V.
- Không đưa tín hiệu 5V vào chân UART STM32.
- Chưa đấu tải 220V khi mới test.
- Test relay bằng LED hoặc module relay rời trước.
- GND của FT232RL và STM32 phải nối chung.

## 4. Kết nối FT232RL với STM32

Firmware dùng USART1:

| STM32 | Chức năng |
|---|---|
| PA9 | USART1_TX |
| PA10 | USART1_RX |
| GND | Ground |

Đấu dây:

```text
FT232RL TXD → STM32 PA10 / USART1_RX
FT232RL RXD → STM32 PA9  / USART1_TX
FT232RL GND → STM32 GND
```

Nếu STM32 đã cấp nguồn bằng ST-LINK hoặc USB riêng thì không cần nối VCC từ FT232RL.

## 5. Cấu hình UART

| Thông số | Giá trị |
|---|---|
| Baudrate | 115200 |
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |
| Flow control | None |

## 6. Protocol frame

```text
AA 55 LENGTH CMD_ID PAYLOAD CRC_LOW CRC_HIGH
```

CRC16/CCITT, init `0xFFFF`, poly `0x1021`, tính trên `LENGTH + CMD_ID + PAYLOAD`, không tính SOF.

## 7. Test PING

Frame gửi:

```text
AA 55 00 10 3E 0F
```

Kết quả mong muốn:

```text
AA 55 01 11 10 DF D9
```

Ý nghĩa:

| Byte | Ý nghĩa |
|---|---|
| `AA 55` | SOF |
| `01` | Payload length = 1 |
| `11` | ACK command |
| `10` | ACK cho PING |
| `DF D9` | CRC low/high |

## 8. Gửi lệnh bằng Python

Cài thư viện:

```powershell
pip install pyserial
```

Script gửi PING:

```python
import serial
import time

SOF = bytes([0xAA, 0x55])

def crc16_ccitt(data: bytes) -> int:
    crc = 0xFFFF
    for b in data:
        crc ^= b << 8
        for _ in range(8):
            if crc & 0x8000:
                crc = ((crc << 1) ^ 0x1021) & 0xFFFF
            else:
                crc = (crc << 1) & 0xFFFF
    return crc

def build_frame(cmd_id: int, payload: bytes = b"") -> bytes:
    length = len(payload)
    body = bytes([length, cmd_id]) + payload
    crc = crc16_ccitt(body)
    return SOF + body + bytes([crc & 0xFF, (crc >> 8) & 0xFF])

ser = serial.Serial("COM5", 115200, timeout=1)
frame = build_frame(0x10)
print("TX:", frame.hex(" ").upper())
ser.write(frame)
time.sleep(0.2)
rx = ser.read(64)
print("RX:", rx.hex(" ").upper())
ser.close()
```

Đổi `COM5` thành COM thật của FT232RL.

## 9. Test relay bằng LED

Các pin relay:

| Chức năng | Pin |
|---|---|
| LIGHT_RELAY | PA15 |
| SCREEN_UP_RELAY | PB12 |
| SCREEN_DOWN_RELAY | PB13 |
| CURTAIN_OPEN_RELAY | PB14 |
| CURTAIN_CLOSE_RELAY | PB15 |

Test bằng LED + điện trở:

```text
STM32 GPIO pin → điện trở 220Ω/330Ω → LED → GND
```

## 10. Thứ tự test đề xuất

```text
1. Nạp firmware bằng ST-LINK.
2. Kết nối FT232RL với PA9/PA10/GND.
3. Gửi PING.
4. Kiểm tra ACK.
5. Gửi frame sai CRC để kiểm tra NACK CRC.
6. Gắn LED vào PA15.
7. Gửi LIGHT_CONTROL ON/OFF.
8. Kiểm tra LED đổi trạng thái.
9. Sau đó mới test các relay motor.
```

## 11. Lưu ý về USB trên Blackpill

Cổng USB trên Blackpill không tự động là UART1. Firmware hiện dùng USART1 PA9/PA10, không phải USB CDC. Vì vậy cần dùng FT232RL hoặc USB-to-UART tương đương.
