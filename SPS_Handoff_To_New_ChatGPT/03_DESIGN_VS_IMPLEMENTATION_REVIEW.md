# 03 - Design vs Implementation Review

## 1. Kết luận tổng quan

Firmware hiện tại **đúng với thiết kế ban đầu ở phần lõi điều khiển thời gian thực**:

```text
UART
→ Buffer/Queue
→ Parser
→ Dispatcher
→ Relay / Projector / IR / OTA task
→ ACK/NACK
```

Mức độ khớp hiện tại:

| Nhóm thiết kế | Mức độ khớp | Ghi chú |
|---|---:|---|
| Kiến trúc MCU realtime | Cao | Đã dùng FreeRTOS task + queue |
| UART protocol/parser | Cao | Có frame, parser, CRC16 |
| Relay control | Cao | Đã có relayTask và service |
| Projector control | Trung bình | Có task/service, chưa có command RS232 thật |
| IR control | Trung bình | Có task/service, mới phát PWM demo |
| OTA/update manager | Thấp-Trung bình | Mới skeleton |
| Bootloader/A-B OTA | Chưa có | Cần thiết kế riêng |
| Telemetry manager riêng | Chưa có | ACK/NACK còn nằm trong `freertos.c` |

## 2. Đối chiếu với kiến trúc tổng thể SPS

Thiết kế SPS phân chia:

```text
SBC/Raspberry Pi:
- Giao diện HMI
- Logic nghiệp vụ cấp cao
- Kết nối server/web dashboard
- Nhận firmware update từ server hoặc người dùng

MCU/STM32:
- Điều khiển phần cứng thời gian thực
- Relay
- Projector RS232
- IR PWM
- Nhận lệnh từ SBC qua UART
- Trả ACK/NACK/status về SBC
```

Firmware hiện tại đi đúng phân chia này.

## 3. Đối chiếu flow xử lý realtime

| Thiết kế ban đầu | Firmware hiện tại | Đánh giá |
|---|---|---|
| UART Driver | HAL UART + ReceiveToIdle DMA | Đúng |
| Buffer Management | `uartRxDmaBuffer` + `uartQueue` | Đúng chức năng |
| UART Frame Parser | `sps_protocol.h/.c` | Đúng |
| Validate checksum | CRC16/CCITT | Đúng |
| Command Dispatcher | `command_dispatcher.c` | Đúng |
| Projector Control Service | `projector_service.c` + `projectorTask` | Đúng khung |
| Relay Control Service | `relay_service.c` + `relayTask` | Đúng |
| IR Control Service | `ir_service.c` + `irTask` | Đúng khung |
| OTA/Update Manager | `ota_service.c` + `otaTask` | Mới skeleton |
| Telemetry & Status Manager | `App_SendAck/App_SendNack` trong `freertos.c` | Chưa tách module |

## 4. Điểm cải tiến so với thiết kế ban đầu

Dispatcher ban đầu có thể gọi service trực tiếp. Firmware hiện tại tốt hơn vì dùng queue/task:

```text
commandTask
→ dispatcher
→ queue riêng
→ task riêng xử lý
→ task gửi ACK/NACK
```

Ưu điểm:

- `commandTask` không bị block bởi tác vụ chậm.
- Relay, projector, IR, OTA có luồng riêng.
- Dễ mở rộng/debug.
- ACK/NACK sau khi xử lý thật, không ACK sớm.

## 5. Khoảng cách lớn nhất: OTA thật

Thiết kế OTA mong muốn:

```text
SBC nhận firmware
→ SBC gửi xuống MCU
→ Bootloader MCU nhận firmware
→ ghi Flash
→ verify CRC/hash
→ set boot flag
→ reset
→ chạy firmware mới
→ nếu lỗi thì rollback
```

Firmware hiện tại mới có:

```text
OTA_START → kiểm tra firmwareSize
OTA_CHUNK → kiểm tra sequence chunk, đếm receivedSize
OTA_END → kiểm tra receivedSize == expectedSize, ACK/NACK
```

Còn thiếu:

- Bootloader riêng.
- Memory map bootloader/application.
- Flash erase/write.
- Image metadata.
- CRC32/hash toàn image.
- Vector table relocation.
- Boot flag.
- Jump application.
- Rollback hoặc safe recovery.

## 6. Các điểm chưa đủ

### Telemetry & Status Manager

Hiện ACK/NACK nằm trong `freertos.c`:

```c
App_SendFrame()
App_SendAck()
App_SendNack()
```

Nên tách thành `telemetry_service.h/.c`, nhưng nên làm sau khi test UART/relay cơ bản pass.

### Projector

Cần bổ sung:

- Power ON/OFF command theo projector thật.
- Timeout/response check.
- Mapping theo model projector.

### IR

Cần bổ sung:

- IR protocol cụ thể.
- Mark/space timing.
- Encode mode/temp/fan.
- Pattern máy lạnh thật.

## 7. Thứ tự ưu tiên hiện tại

```text
1. Test UART bằng laptop + FT232RL USB to UART TTL.
2. Test PING ACK.
3. Test LIGHT_CONTROL bằng LED/module relay.
4. Test RELAY_MOTOR bằng LED/module relay, chưa đấu 220V.
5. Sau UART + relay ổn, test projectorTask/irTask/otaTask skeleton.
6. Sau đó mới refactor telemetry_service.
7. Sau cùng mới thiết kế bootloader OTA thật.
```

## 8. Câu báo cáo ngắn cho thầy

Firmware MCU hiện tại đã bám đúng kiến trúc thiết kế ban đầu ở phần xử lý lệnh thời gian thực. Nhóm đã hiện thực UART protocol, parser, dispatcher và tách các nhóm điều khiển thành task riêng trên FreeRTOS gồm `relayTask`, `projectorTask`, `irTask` và `otaTask`. Các lệnh được đưa qua queue riêng và ACK/NACK sau khi task xử lý xong, phù hợp với yêu cầu realtime. Tuy nhiên, phần OTA hiện tại mới là skeleton ở application layer; để đạt thiết kế OTA hoàn chỉnh như sequence diagram, nhóm cần triển khai thêm bootloader, phân vùng Flash, kiểm tra toàn vẹn firmware và cơ chế rollback.
