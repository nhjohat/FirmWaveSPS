#ifndef SPS_PROTOCOL_H
#define SPS_PROTOCOL_H

#include <stdint.h>
<<<<<<< HEAD
#include <stddef.h>

/*
 * SPS UART Protocol v0.1.0
 *
 * Frame format:
 * [0]      SOF_1   = 0xAA
 * [1]      SOF_2   = 0x55
 * [2]      LENGTH  = payload length only
 * [3]      CMD_ID
 * [4..N]   PAYLOAD
 * [N+1..]  CRC16-CCITT, little-endian: CRC_LOW first, CRC_HIGH second
 *
 * CRC is calculated over:
 * LENGTH + CMD_ID + PAYLOAD
 * Header is not included in CRC.
 */

#define SPS_SOF_1               0xAAU
#define SPS_SOF_2               0x55U

#define SPS_MAX_PAYLOAD_SIZE    255U
#define SPS_MAX_FRAME_SIZE      (2U + 1U + 1U + SPS_MAX_PAYLOAD_SIZE + 2U)

typedef enum
{
    SPS_PARSE_IN_PROGRESS = 0,
    SPS_PARSE_FRAME_READY,
    SPS_PARSE_ERROR
} SPS_ParseResult_t;

typedef enum
{
    SPS_CMD_PING              = 0x10,
    SPS_CMD_ACK               = 0x11,
    SPS_CMD_NACK              = 0x12,

    SPS_CMD_LIGHT_CONTROL     = 0x21,
    SPS_CMD_RELAY_MOTOR       = 0x22,
    SPS_CMD_PROJECTOR_CONTROL = 0x23,
    SPS_CMD_AC_CONTROL        = 0x24,

    SPS_CMD_RELAY_STATUS      = 0x32,

    SPS_CMD_PRESENCE_EVENT    = 0x41,

    SPS_CMD_OTA_START         = 0x50,
    SPS_CMD_OTA_CHUNK         = 0x51,
    SPS_CMD_OTA_END           = 0x52
} SPS_CommandId_t;

typedef enum
{
    SPS_ERR_CRC           = 0x01,
    SPS_ERR_INVALID_PARAM = 0x02,
    SPS_ERR_INVALID_LEN   = 0x03,
    SPS_ERR_UNKNOWN_CMD   = 0x04,
    SPS_ERR_QUEUE_FULL    = 0x05,
    SPS_ERR_TIMEOUT       = 0x06,
    SPS_ERR_OTA           = 0x07
} SPS_ErrorCode_t;
=======

#define SPS_SOF_1                  0xAAU
#define SPS_SOF_2                  0x55U
#define SPS_MAX_PAYLOAD_SIZE       255U

#define SPS_CMD_PING               0x10U
#define SPS_CMD_ACK                0x11U
#define SPS_CMD_NACK               0x12U

#define SPS_CMD_LIGHT_CONTROL      0x21U
#define SPS_CMD_RELAY_CONTROL      0x22U
#define SPS_CMD_RELAY_STATUS       0x32U

#define SPS_ERR_CRC                0x01U
#define SPS_ERR_INVALID_PARAM      0x02U
#define SPS_ERR_UNKNOWN_CMD        0x03U
#define SPS_ERR_QUEUE_FULL         0x04U
#define SPS_ERR_BAD_LENGTH         0x05U
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428

typedef struct
{
    uint8_t length;
    uint8_t cmd_id;
    uint8_t payload[SPS_MAX_PAYLOAD_SIZE];
<<<<<<< HEAD
} SPS_Frame_t;

void SPS_Protocol_Init(void);

SPS_ParseResult_t SPS_Protocol_ParseByte(uint8_t byte, SPS_Frame_t *outFrame);

uint16_t SPS_Protocol_Crc16Ccitt(const uint8_t *data, uint16_t length);

uint16_t SPS_Protocol_BuildFrame(uint8_t cmd_id,
                                 const uint8_t *payload,
                                 uint8_t length,
                                 uint8_t *outBuffer,
                                 uint16_t outBufferSize);

#endif /* SPS_PROTOCOL_H */
=======
} sps_frame_t;

typedef enum
{
    SPS_PARSE_WAIT_SOF1 = 0,
    SPS_PARSE_WAIT_SOF2,
    SPS_PARSE_LENGTH,
    SPS_PARSE_CMD,
    SPS_PARSE_PAYLOAD,
    SPS_PARSE_CRC_HIGH,
    SPS_PARSE_CRC_LOW
} sps_parse_state_t;

typedef struct
{
    sps_parse_state_t state;

    uint8_t length;
    uint8_t cmd_id;
    uint8_t payload[SPS_MAX_PAYLOAD_SIZE];
    uint8_t payload_index;

    uint16_t crc_calc;
    uint16_t crc_rx;
} sps_parser_t;

void SPS_Protocol_ParserInit(sps_parser_t *parser);
int32_t SPS_Protocol_ParserPushByte(sps_parser_t *parser, uint8_t byte, sps_frame_t *out_frame);

void SPS_Protocol_SendFrame(uint8_t cmd_id, const uint8_t *payload, uint8_t length);
void SPS_Protocol_SendAck(uint8_t ack_cmd_id);
void SPS_Protocol_SendNack(uint8_t failed_cmd_id, uint8_t error_code);

#endif
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
