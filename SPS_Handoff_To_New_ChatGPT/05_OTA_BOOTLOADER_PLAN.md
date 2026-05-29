# 05 - OTA Bootloader Plan

## 1. Mục tiêu

Thiết kế OTA firmware update thật cho MCU STM32F401CCU6 trong SPS:

```text
Server/Web/SBC nhận firmware mới
→ SBC gửi firmware xuống MCU qua UART
→ MCU bootloader nhận firmware
→ ghi Flash
→ verify CRC/hash
→ reset/jump application mới
```

## 2. OTA skeleton hiện tại

Firmware hiện có `otaTask` và `ota_service` xử lý:

```text
OTA_START
OTA_CHUNK
OTA_END
sequence chunk
expectedSize
receivedSize
ACK/NACK
```

Nhưng chưa có:

- Ghi Flash thật.
- Bootloader.
- Verify image toàn bộ.
- Phân vùng firmware.
- Boot flag.
- Rollback.
- Jump application.

Vì vậy hiện tại chỉ nên báo cáo là: **đã hiện thực OTA protocol skeleton ở application layer**.

## 3. Vai trò của SBC

SBC/Raspberry Pi hoặc laptop đóng vai trò update host:

```text
1. Nhận file firmware từ server/web dashboard.
2. Lưu file .bin.
3. Kiểm tra version/kích thước.
4. Tính CRC32/hash.
5. Gửi OTA_START xuống MCU.
6. Chia firmware thành chunk.
7. Gửi OTA_CHUNK từng gói.
8. Chờ ACK/NACK.
9. Retry nếu lỗi.
10. Gửi OTA_END.
11. Chờ MCU verify và reboot.
```

## 4. Vai trò của MCU bootloader

Bootloader làm các việc:

```text
1. Chạy ngay sau reset.
2. Kiểm tra boot flag/update request.
3. Kiểm tra application hiện tại hợp lệ không.
4. Nhận firmware từ SBC qua UART.
5. Erase vùng application.
6. Ghi từng chunk vào Flash.
7. Verify CRC32/hash toàn image.
8. Set app valid flag.
9. Jump application mới.
10. Nếu lỗi, không chạy app hỏng.
```

## 5. Memory map đề xuất

STM32F401CCU6 có 256KB Flash. Đề xuất ban đầu:

```text
0x08000000 → Bootloader
0x08008000 → Main Application
```

| Vùng | Địa chỉ | Kích thước dự kiến | Ghi chú |
|---|---:|---:|---|
| Bootloader | `0x08000000` | 32KB | Nhận OTA, verify, jump app |
| Application | `0x08008000` | phần còn lại | Firmware chính |
| Metadata/Flag | cuối Flash hoặc sector riêng | nhỏ | app valid/update flag/version/CRC |

Cần kiểm tra lại flash sector layout thực tế trước khi chốt địa chỉ erase/write.

## 6. Application relocation

Khi application bắt đầu ở `0x08008000`, cần chỉnh linker script và vector table:

```c
SCB->VTOR = 0x08008000;
```

## 7. Giao thức OTA đề xuất

Command hiện có:

| CMD | Tên |
|---|---|
| `0x50` | OTA_START |
| `0x51` | OTA_CHUNK |
| `0x52` | OTA_END |

### OTA_START payload đề xuất

```text
firmware_size: 4 bytes little-endian
firmware_crc32: 4 bytes little-endian
version_major: 1 byte
version_minor: 1 byte
version_patch: 1 byte
reserved: 1 byte
```

### OTA_CHUNK payload đề xuất

```text
chunk_index: 2 bytes little-endian
chunk_length: 1 byte
chunk_data: N bytes
```

### OTA_END payload đề xuất

Có thể để rỗng hoặc gửi lại final CRC32.

## 8. FSM bootloader đề xuất

```text
BOOT_INIT
→ CHECK_UPDATE_FLAG
→ WAIT_OTA_START
→ ERASE_APP
→ RECEIVE_CHUNKS
→ VERIFY_IMAGE
→ SET_APP_VALID
→ JUMP_APP
→ ERROR_RECOVERY
```

## 9. Vì sao không nên ghi Flash từ application hiện tại?

Nếu application tự ghi đè chính nó mà chưa có bootloader:

- Mất điện giữa chừng có thể làm app hỏng.
- Nếu app hỏng thì MCU không còn logic để nhận bản sửa.
- Debug khó.
- Không có rollback.

Vì vậy bootloader nên là vùng cố định, nhỏ, ít thay đổi và chịu trách nhiệm cứu thiết bị.

## 10. Lộ trình triển khai

```text
1. Test UART PING thật trên kit.
2. Test relayTask thật bằng LED/module relay.
3. Test command ACK/NACK ổn định.
4. Test OTA skeleton qua UART.
5. Thiết kế memory map bootloader/application.
6. Tạo project bootloader riêng.
7. Tạo project application relocated.
8. Bootloader jump application.
9. Bootloader nhận OTA_START/CHUNK/END.
10. Bootloader ghi Flash.
11. Verify CRC32.
12. Reboot chạy app mới.
13. Test mất điện giữa chừng.
```

## 11. Kết luận

Mô hình OTA đúng cho SPS:

```text
SBC nhận firmware mới
→ SBC đẩy firmware qua UART cho MCU
→ MCU bootloader ghi firmware mới
```

`otaTask` hiện tại có giá trị để kiểm chứng protocol OTA, nhưng OTA thật cần bootloader riêng.
