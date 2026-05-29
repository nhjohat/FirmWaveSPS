# 02 - Firmware Status Handoff

## 1. Thông tin dự án

| Hạng mục | Thông tin |
|---|---|
| Project | Smart Podium System – SPS |
| Thiết bị | Podium Control Device – PCD |
| MCU | STM32F401CCU6 Blackpill |
| Framework | STM32 HAL + FreeRTOS CMSIS_V2 |
| IDE | STM32CubeIDE |
| Repository | `https://github.com/nhjohat/FirmWaveSPS` |
| Branch | Cần xác nhận sau khi clone repo mới bằng `git branch --show-current` |
| Firmware folder | `firmware/` |
| Commit checkpoint | Cần xác nhận sau khi push/clone repo mới bằng `git log --oneline -1`; checkpoint kỹ thuật hiện tại là task-based Projector/IR/OTA handling đã clean build pass |
| Build status | Clean build pass, `0 errors, 0 warnings` |

## 2. Luồng xử lý chính

```text
USART1 DMA ReceiveToIdle
→ uartQueue
→ commandTask
→ SPS_Protocol_ParseByte()
→ Command_DispatchFrame()
→ relayQueue / projectorQueue / irQueue / otaQueue
→ relayTask / projectorTask / irTask / otaTask
→ ACK/NACK về SBC/laptop qua USART1
```

## 3. Task hiện tại

| Task | Priority | Stack | Vai trò |
|---|---:|---:|---|
| `uartRxTask` | High | 256 words | Khởi động UART1 ReceiveToIdle DMA |
| `commandTask` | AboveNormal | 256 words | Nhận byte từ `uartQueue`, parse frame, dispatch command |
| `relayTask` | Normal | 256 words | Điều khiển relay GPIO và ACK/NACK sau xử lý |
| `projectorTask` | Normal | 256 words | Xử lý projector command |
| `irTask` | Normal | 256 words | Xử lý IR command |
| `otaTask` | Normal | 512 words | Xử lý OTA skeleton |

## 4. Queue/mutex hiện tại

| Thành phần | Size | Item size | Vai trò |
|---|---:|---:|---|
| `uartQueue` | 256 | `sizeof(uint8_t)` | Byte UART từ DMA callback |
| `relayQueue` | 10 | `sizeof(uint32_t)` | Relay command packed |
| `projectorQueue` | 5 | `sizeof(ProjectorCommand_t)` | Projector command |
| `irQueue` | 5 | `sizeof(IrCommand_t)` | IR command |
| `otaQueue` | 5 | `sizeof(OtaCommand_t)` | OTA command |
| `uartTxMutex` | - | - | Bảo vệ UART TX khi nhiều task gửi ACK/NACK |

## 5. Cấu hình ngoại vi

### USART1 – SBC/laptop

- PA9 = TX
- PA10 = RX
- Baudrate: 115200 8N1
- DMA RX circular, ReceiveToIdle

### USART2 – Projector RS232

- PA2 = TX
- PA3 = RX
- Baudrate: 9600 8N1

### Relay GPIO

| Chức năng | Pin |
|---|---|
| Đèn | PA15 |
| Màn chiếu lên | PB12 |
| Màn chiếu xuống | PB13 |
| Rèm mở | PB14 |
| Rèm đóng | PB15 |

### IR PWM

- TIM1 CH1
- PA8
- 37–38kHz
- Hiện chỉ phát demo burst 100ms.

## 6. Command hiện tại

| CMD | Tên | Xử lý hiện tại |
|---|---|---|
| `0x10` | PING | ACK trực tiếp trong `commandTask` |
| `0x21` | LIGHT_CONTROL | `relayQueue` → `relayTask` |
| `0x22` | RELAY_MOTOR | `relayQueue` → `relayTask` |
| `0x23` | PROJECTOR_CONTROL | `projectorQueue` → `projectorTask` |
| `0x24` | AC_CONTROL | `irQueue` → `irTask` |
| `0x32` | RELAY_STATUS | Xử lý trực tiếp trong `commandTask` |
| `0x50` | OTA_START | `otaQueue` → `otaTask` |
| `0x51` | OTA_CHUNK | `otaQueue` → `otaTask` |
| `0x52` | OTA_END | `otaQueue` → `otaTask` |

## 7. File đã thay đổi ở checkpoint mới nhất

```text
firmware/.gitignore
firmware/Core/Inc/ir_service.h
firmware/Core/Inc/ota_service.h
firmware/Core/Inc/projector_service.h
firmware/Core/Src/command_dispatcher.c
firmware/Core/Src/freertos.c
firmware/Core/Src/ir_service.c
firmware/Core/Src/ota_service.c
firmware/Core/Src/projector_service.c
```

## 8. Chưa hoàn thành

- Chưa test UART thật bằng laptop/SBC.
- Chưa test relay thật bằng LED/module relay.
- Projector service chưa có command RS232 thật.
- IR service chưa có IR pattern thật.
- OTA chưa ghi Flash thật.
- Chưa có bootloader OTA thật.
- Chưa có telemetry/status service riêng.

## 9. Ưu tiên tiếp theo

1. Test UART PING bằng laptop + FT232RL.
2. Test relayTask bằng LED hoặc module relay.
3. Test projectorTask, irTask, otaTask skeleton.
4. Sau khi phần cứng cơ bản ổn, thiết kế bootloader OTA thật.
