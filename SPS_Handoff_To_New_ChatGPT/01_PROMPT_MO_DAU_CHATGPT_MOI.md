# 01 - Prompt mở đầu cho ChatGPT mới

Tôi đang làm đồ án **Smart Podium System – SPS**, thiết bị **Podium Control Device – PCD**. Tôi phụ trách firmware MCU cho **STM32F401CCU6 Blackpill**, dùng **STM32 HAL + FreeRTOS CMSIS_V2**.

Hãy tiếp tục hỗ trợ tôi theo nguyên tắc:

- Hướng dẫn từng bước nhỏ.
- Mỗi lần chỉ đưa 1 bước.
- Tôi build/test pass rồi mới đưa bước tiếp theo.
- Nếu sửa code, chỉ rõ file, vị trí cần tìm, đoạn cần thay, đoạn cần thêm.
- Ưu tiên giữ project build pass sau từng bước.
- Không nhảy sang bootloader OTA khi UART/relay chưa test chắc trên kit thật.

## Trạng thái hiện tại

- Firmware đã clean build pass: `0 errors, 0 warnings`.
- GitHub repo sử dụng từ đây trở đi: `https://github.com/nhjohat/FirmWaveSPS`
- Branch/commit mới nhất: cần kiểm tra bằng `git status` và `git log --oneline -1` sau khi clone/push repo này.
- Checkpoint kỹ thuật hiện tại: đã hoàn thành task-based handling cho Projector / IR / OTA và clean build pass.
- Code firmware nằm trong folder firmware của repo mới hoặc folder firmware hiện tại sau khi đồng bộ.

## Kiến trúc firmware hiện tại

```text
UART1 DMA ReceiveToIdle
→ uartQueue
→ commandTask
→ SPS protocol parser
→ command_dispatcher
→ relayQueue / projectorQueue / irQueue / otaQueue
→ relayTask / projectorTask / irTask / otaTask
→ task xử lý xong mới gửi ACK/NACK về SBC/laptop
```

## Các task hiện có

| Task | Vai trò |
|---|---|
| `uartRxTask` | Khởi động UART ReceiveToIdle DMA |
| `commandTask` | Parse frame, kiểm tra CRC, dispatch command |
| `relayTask` | Điều khiển relay GPIO, ACK/NACK sau khi thực thi |
| `projectorTask` | Xử lý projector command từ `projectorQueue` |
| `irTask` | Xử lý AC IR command từ `irQueue` |
| `otaTask` | Xử lý OTA skeleton từ `otaQueue` |

## Cấu hình phần cứng chính

- MCU: STM32F401CCU6 Blackpill.
- USART1: SBC/laptop UART, PA9 TX, PA10 RX, 115200 8N1, DMA RX circular.
- USART2: projector RS232, PA2 TX, PA3 RX, 9600 8N1.
- Relay:
  - PA15 = LIGHT_RELAY
  - PB12 = SCREEN_UP_RELAY
  - PB13 = SCREEN_DOWN_RELAY
  - PB14 = CURTAIN_OPEN_RELAY
  - PB15 = CURTAIN_CLOSE_RELAY
- IR PWM: TIM1 CH1, PA8, khoảng 37–38kHz.

## Protocol UART

```text
AA 55 LENGTH CMD_ID PAYLOAD CRC_LOW CRC_HIGH
```

CRC16/CCITT, init `0xFFFF`, poly `0x1021`, tính trên `LENGTH + CMD_ID + PAYLOAD`, không tính SOF `AA 55`, CRC gửi low byte trước.

## Command hiện tại

| CMD | Tên |
|---|---|
| `0x10` | PING |
| `0x11` | ACK |
| `0x12` | NACK |
| `0x21` | LIGHT_CONTROL |
| `0x22` | RELAY_MOTOR |
| `0x23` | PROJECTOR_CONTROL |
| `0x24` | AC_CONTROL |
| `0x32` | RELAY_STATUS |
| `0x50` | OTA_START |
| `0x51` | OTA_CHUNK |
| `0x52` | OTA_END |

## Đánh giá hiện tại

- UART/parser/dispatcher/task-based control đã đúng thiết kế ban đầu.
- Projector service mới là skeleton, chưa có command RS232 thật.
- IR service mới phát demo carrier PWM 100ms, chưa có IR pattern máy lạnh thật.
- OTA hiện tại mới là skeleton ở application layer, chưa phải OTA thật.
- OTA thật cần bootloader riêng nhận firmware từ SBC qua UART, ghi Flash, verify CRC, set boot flag và reboot/jump app.

## Ưu tiên tiếp theo

Tôi đã có kit STM32. Ưu tiên hiện tại:

1. Test UART bằng laptop + FT232RL USB to UART TTL.
2. Test PING ACK.
3. Test `relayTask` bằng LED hoặc module relay, chưa đấu 220V.
4. Sau UART + relay ổn mới test `projectorTask`, `irTask`, `otaTask skeleton`.
5. Sau khi chức năng cơ bản ổn mới thiết kế bootloader OTA thật.
