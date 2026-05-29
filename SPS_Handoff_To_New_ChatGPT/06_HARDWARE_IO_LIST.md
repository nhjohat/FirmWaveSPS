# 06 - Hardware IO List

## 1. Mục tiêu

Ghi lại các chân STM32F401CCU6 Blackpill đang được firmware SPS sử dụng để đối chiếu firmware, schematic, PCB và test phần cứng.

## 2. MCU

| Hạng mục | Thông tin |
|---|---|
| MCU | STM32F401CCU6 Blackpill |
| Core | ARM Cortex-M4 |
| Framework | STM32 HAL + FreeRTOS CMSIS_V2 |
| SYSCLK | 84 MHz |

## 3. USART1 – SBC/laptop

| Tín hiệu | Chân STM32 | Chức năng |
|---|---|---|
| USART1_TX | PA9 | MCU gửi dữ liệu về SBC/laptop |
| USART1_RX | PA10 | MCU nhận dữ liệu từ SBC/laptop |

Cấu hình:

| Thông số | Giá trị |
|---|---|
| Baudrate | 115200 |
| Word length | 8 bits |
| Stop bits | 1 |
| Parity | None |
| DMA RX | DMA2 Stream2 Channel 4 |
| DMA Mode | Circular |
| DMA Priority | High |
| IRQ | USART1 IRQ enabled |

Kết nối test laptop:

```text
FT232RL TXD → STM32 PA10 / USART1_RX
FT232RL RXD → STM32 PA9  / USART1_TX
FT232RL GND → STM32 GND
```

## 4. USART2 – Projector RS232

| Tín hiệu | Chân STM32 | Chức năng |
|---|---|---|
| USART2_TX | PA2 | MCU gửi command projector qua MAX3232 |
| USART2_RX | PA3 | MCU nhận phản hồi projector qua MAX3232 |

Cấu hình:

| Thông số | Giá trị |
|---|---|
| Baudrate | 9600 |
| Word length | 8 bits |
| Stop bits | 1 |
| Parity | None |

Ghi chú:

- USART2 là TTL UART từ MCU.
- Cần IC MAX3232 để chuyển TTL 3.3V sang RS232 cho projector.
- Projector service hiện mới là skeleton.

## 5. Relay GPIO

| Chức năng | Pin STM32 | Tên firmware | Ghi chú |
|---|---|---|---|
| Đèn | PA15 | `LIGHT_RELAY` | Relay active-high giả định |
| Màn chiếu lên | PB12 | `SCREEN_UP_RELAY` | Motor direction/interlock |
| Màn chiếu xuống | PB13 | `SCREEN_DOWN_RELAY` | Motor direction/interlock |
| Rèm mở | PB14 | `CURTAIN_OPEN_RELAY` | Motor direction/interlock |
| Rèm đóng | PB15 | `CURTAIN_CLOSE_RELAY` | Motor direction/interlock |

Cấu hình GPIO hiện tại:

| Thuộc tính | Giá trị |
|---|---|
| Mode | Output Push Pull |
| Pull | No Pull |
| Speed | Low Speed |
| Default | LOW |
| Active level | Active-high giả định |

Lưu ý:

- Test bằng LED/module relay trước.
- Chưa đấu 220V trong giai đoạn đầu.
- Nếu module relay active-low thì cần chỉnh logic trong `relay_service`.

## 6. IR PWM

| Tín hiệu | Chân STM32 | Chức năng |
|---|---|---|
| TIM1_CH1 | PA8 | Phát carrier IR 37–38kHz |

Cấu hình:

| Thông số | Giá trị |
|---|---|
| Timer | TIM1 |
| Channel | CH1 |
| Prescaler | 83 |
| ARR/Period | 26 |
| Pulse | 13 |
| Tần số | khoảng 37–38kHz |

Ghi chú:

- `ir_service` hiện mới phát demo burst 100ms.
- Chưa có IR pattern máy lạnh thật.

## 7. BOOT0 / OTA note

Theo hướng OTA thật, cần xem xét BOOT0 hoặc boot flag. Nếu dùng bootloader tự viết ở `0x08000000`, BOOT0 không nhất thiết phải dùng cho mọi luồng OTA. Bootloader có thể chạy sau reset, kiểm tra flag và nhận firmware qua UART.

## 8. Chức năng chưa chốt

| Chức năng | Tình trạng |
|---|---|
| RFID RC522 | Theo thiết kế có thể nối Pi/SBC qua SPI, không thuộc firmware MCU hiện tại |
| Màn hình HMI | Thuộc SBC/Raspberry Pi |
| Máy tính/PC power | Chưa rõ MCU có cần điều khiển riêng không |
| Cảm biến môi trường | Chưa thấy cấu hình firmware hiện tại |
| Flash/NVM OTA | Chưa có driver hiện tại |

## 9. Checklist test phần cứng

### UART

- [ ] FT232RL nhận COM port trên laptop.
- [ ] Kết nối TX/RX/GND đúng.
- [ ] Gửi frame PING.
- [ ] Nhận ACK đúng.

### Relay

- [ ] Gắn LED + điện trở vào PA15.
- [ ] Gửi LIGHT ON.
- [ ] LED sáng.
- [ ] Gửi LIGHT OFF.
- [ ] LED tắt.
- [ ] Test PB12/PB13 bằng LED trước khi nối motor/relay.
- [ ] Test PB14/PB15 bằng LED trước khi nối motor/relay.

### Projector

- [ ] Test USART2 TX bằng logic analyzer/USB UART trước.
- [ ] Kết nối MAX3232.
- [ ] Xác nhận command RS232 theo model projector.
- [ ] Test Power ON/OFF.

### IR

- [ ] Dùng oscilloscope/logic analyzer đo PA8.
- [ ] Xác nhận carrier 37–38kHz.
- [ ] Sau đó mới thêm pattern IR thật.

### OTA

- [ ] Test OTA skeleton qua UART.
- [ ] Thiết kế bootloader.
- [ ] Test jump application.
- [ ] Test Flash write.
- [ ] Test verify CRC.
