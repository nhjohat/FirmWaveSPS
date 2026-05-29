#include "sps_protocol.h"
<<<<<<< HEAD

#include <string.h>

typedef enum
{
    PARSER_WAIT_SOF_1 = 0,
    PARSER_WAIT_SOF_2,
    PARSER_WAIT_LENGTH,
    PARSER_WAIT_CMD,
    PARSER_WAIT_PAYLOAD,
    PARSER_WAIT_CRC_LOW,
    PARSER_WAIT_CRC_HIGH
} ParserState_t;

static ParserState_t parserState;
static SPS_Frame_t currentFrame;
static uint16_t payloadIndex;
static uint16_t receivedCrc;

static uint16_t SPS_Protocol_CalcFrameCrc(const SPS_Frame_t *frame);

void SPS_Protocol_Init(void)
{
    parserState = PARSER_WAIT_SOF_1;
    payloadIndex = 0U;
    receivedCrc = 0U;
    memset(&currentFrame, 0, sizeof(currentFrame));
}

uint16_t SPS_Protocol_Crc16Ccitt(const uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFFU;

    if ((data == NULL) && (length > 0U))
    {
        return 0U;
    }

    for (uint16_t i = 0U; i < length; i++)
    {
        crc ^= ((uint16_t)data[i] << 8);

        for (uint8_t bit = 0U; bit < 8U; bit++)
        {
            if ((crc & 0x8000U) != 0U)
            {
                crc = (uint16_t)((crc << 1) ^ 0x1021U);
            }
            else
            {
                crc = (uint16_t)(crc << 1);
            }
=======
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx.h"

#define CRC16_CCITT_INIT_VALUE      0xFFFFU
#define CRC16_CCITT_POLY            0x1021U

static uint16_t SPS_Crc16UpdateByte(uint16_t crc, uint8_t data)
{
    crc ^= ((uint16_t)data << 8);

    for (uint8_t i = 0; i < 8U; i++)
    {
        if ((crc & 0x8000U) != 0U)
        {
            crc = (uint16_t)((crc << 1) ^ CRC16_CCITT_POLY);
        }
        else
        {
            crc = (uint16_t)(crc << 1);
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
        }
    }

    return crc;
}

<<<<<<< HEAD
static uint16_t SPS_Protocol_CalcFrameCrc(const SPS_Frame_t *frame)
{
    uint8_t crcBuffer[2U + SPS_MAX_PAYLOAD_SIZE];

    if (frame == NULL)
    {
        return 0U;
    }

    crcBuffer[0] = frame->length;
    crcBuffer[1] = frame->cmd_id;

    if (frame->length > 0U)
    {
        memcpy(&crcBuffer[2], frame->payload, frame->length);
    }

    return SPS_Protocol_Crc16Ccitt(crcBuffer, (uint16_t)(2U + frame->length));
}

SPS_ParseResult_t SPS_Protocol_ParseByte(uint8_t byte, SPS_Frame_t *outFrame)
{
    switch (parserState)
    {
        case PARSER_WAIT_SOF_1:
            if (byte == SPS_SOF_1)
            {
                parserState = PARSER_WAIT_SOF_2;
            }
            break;

        case PARSER_WAIT_SOF_2:
            if (byte == SPS_SOF_2)
            {
                parserState = PARSER_WAIT_LENGTH;
                memset(&currentFrame, 0, sizeof(currentFrame));
                payloadIndex = 0U;
                receivedCrc = 0U;
            }
            else if (byte == SPS_SOF_1)
            {
                /*
                 * Vẫn giữ trạng thái chờ SOF_2.
                 * Trường hợp stream có AA AA 55 thì byte AA thứ hai
                 * được xem như SOF_1 mới.
                 */
                parserState = PARSER_WAIT_SOF_2;
            }
            else
            {
                parserState = PARSER_WAIT_SOF_1;
            }
            break;

        case PARSER_WAIT_LENGTH:
            currentFrame.length = byte;
            parserState = PARSER_WAIT_CMD;
            break;

        case PARSER_WAIT_CMD:
            currentFrame.cmd_id = byte;

            if (currentFrame.length == 0U)
            {
                parserState = PARSER_WAIT_CRC_LOW;
            }
            else
            {
                parserState = PARSER_WAIT_PAYLOAD;
            }
            break;

        case PARSER_WAIT_PAYLOAD:
            currentFrame.payload[payloadIndex] = byte;
            payloadIndex++;

            if (payloadIndex >= currentFrame.length)
            {
                parserState = PARSER_WAIT_CRC_LOW;
            }
            break;

        case PARSER_WAIT_CRC_LOW:
            receivedCrc = byte;
            parserState = PARSER_WAIT_CRC_HIGH;
            break;

        case PARSER_WAIT_CRC_HIGH:
        {
            uint16_t calculatedCrc;

            receivedCrc |= ((uint16_t)byte << 8);
            calculatedCrc = SPS_Protocol_CalcFrameCrc(&currentFrame);

            if (receivedCrc == calculatedCrc)
            {
                if (outFrame != NULL)
                {
                    memcpy(outFrame, &currentFrame, sizeof(SPS_Frame_t));
                }

                parserState = PARSER_WAIT_SOF_1;
                payloadIndex = 0U;
                receivedCrc = 0U;

                return SPS_PARSE_FRAME_READY;
            }

            parserState = PARSER_WAIT_SOF_1;
            payloadIndex = 0U;
            receivedCrc = 0U;

            return SPS_PARSE_ERROR;
        }

        default:
            parserState = PARSER_WAIT_SOF_1;
            payloadIndex = 0U;
            receivedCrc = 0U;
            break;
    }

    return SPS_PARSE_IN_PROGRESS;
}

uint16_t SPS_Protocol_BuildFrame(uint8_t cmd_id,
                                 const uint8_t *payload,
                                 uint8_t length,
                                 uint8_t *outBuffer,
                                 uint16_t outBufferSize)
{
    uint16_t index = 0U;
    uint16_t crc;
    uint8_t crcBuffer[2U + SPS_MAX_PAYLOAD_SIZE];

    if (outBuffer == NULL)
    {
        return 0U;
    }

    if ((payload == NULL) && (length > 0U))
    {
        return 0U;
    }

    if (outBufferSize < (uint16_t)(2U + 1U + 1U + length + 2U))
    {
        return 0U;
    }

    outBuffer[index++] = SPS_SOF_1;
    outBuffer[index++] = SPS_SOF_2;
    outBuffer[index++] = length;
    outBuffer[index++] = cmd_id;

    if (length > 0U)
    {
        memcpy(&outBuffer[index], payload, length);
        index = (uint16_t)(index + length);
    }

    crcBuffer[0] = length;
    crcBuffer[1] = cmd_id;

    if (length > 0U)
    {
        memcpy(&crcBuffer[2], payload, length);
    }

    crc = SPS_Protocol_Crc16Ccitt(crcBuffer, (uint16_t)(2U + length));

    outBuffer[index++] = (uint8_t)(crc & 0xFFU);
    outBuffer[index++] = (uint8_t)((crc >> 8) & 0xFFU);

    return index;
=======
static uint16_t SPS_Crc16Frame(uint8_t length, uint8_t cmd_id, const uint8_t *payload)
{
    uint16_t crc = CRC16_CCITT_INIT_VALUE;

    crc = SPS_Crc16UpdateByte(crc, length);
    crc = SPS_Crc16UpdateByte(crc, cmd_id);

    for (uint8_t i = 0; i < length; i++)
    {
        crc = SPS_Crc16UpdateByte(crc, payload[i]);
    }

    return crc;
}

void SPS_Protocol_ParserInit(sps_parser_t *parser)
{
    parser->state = SPS_PARSE_WAIT_SOF1;
    parser->length = 0U;
    parser->cmd_id = 0U;
    parser->payload_index = 0U;
    parser->crc_calc = CRC16_CCITT_INIT_VALUE;
    parser->crc_rx = 0U;
}

int32_t SPS_Protocol_ParserPushByte(sps_parser_t *parser, uint8_t byte, sps_frame_t *out_frame)
{
    switch (parser->state)
    {
        case SPS_PARSE_WAIT_SOF1:
        {
            if (byte == SPS_SOF_1)
            {
                parser->state = SPS_PARSE_WAIT_SOF2;
            }
            break;
        }

        case SPS_PARSE_WAIT_SOF2:
        {
            if (byte == SPS_SOF_2)
            {
                parser->state = SPS_PARSE_LENGTH;
            }
            else
            {
                parser->state = SPS_PARSE_WAIT_SOF1;
            }
            break;
        }

        case SPS_PARSE_LENGTH:
        {
            parser->length = byte;
            parser->payload_index = 0U;
            parser->crc_calc = CRC16_CCITT_INIT_VALUE;
            parser->crc_calc = SPS_Crc16UpdateByte(parser->crc_calc, byte);
            parser->state = SPS_PARSE_CMD;
            break;
        }

        case SPS_PARSE_CMD:
        {
            parser->cmd_id = byte;
            parser->crc_calc = SPS_Crc16UpdateByte(parser->crc_calc, byte);

            if (parser->length == 0U)
            {
                parser->state = SPS_PARSE_CRC_HIGH;
            }
            else
            {
                parser->state = SPS_PARSE_PAYLOAD;
            }
            break;
        }

        case SPS_PARSE_PAYLOAD:
        {
            parser->payload[parser->payload_index] = byte;
            parser->payload_index++;

            parser->crc_calc = SPS_Crc16UpdateByte(parser->crc_calc, byte);

            if (parser->payload_index >= parser->length)
            {
                parser->state = SPS_PARSE_CRC_HIGH;
            }
            break;
        }

        case SPS_PARSE_CRC_HIGH:
        {
            parser->crc_rx = ((uint16_t)byte << 8);
            parser->state = SPS_PARSE_CRC_LOW;
            break;
        }

        case SPS_PARSE_CRC_LOW:
        {
            parser->crc_rx |= byte;

            if (parser->crc_rx == parser->crc_calc)
            {
                out_frame->length = parser->length;
                out_frame->cmd_id = parser->cmd_id;

                for (uint8_t i = 0; i < parser->length; i++)
                {
                    out_frame->payload[i] = parser->payload[i];
                }

                SPS_Protocol_ParserInit(parser);
                return 1;
            }

            SPS_Protocol_ParserInit(parser);
            return -1;
        }

        default:
        {
            SPS_Protocol_ParserInit(parser);
            break;
        }
    }

    return 0;
}

static void SPS_Protocol_SendByte(uint8_t byte)
{
    while (LL_USART_IsActiveFlag_TXE(USART1) == 0U)
    {
    }

    LL_USART_TransmitData8(USART1, byte);
}

void SPS_Protocol_SendFrame(uint8_t cmd_id, const uint8_t *payload, uint8_t length)
{
    uint16_t crc = SPS_Crc16Frame(length, cmd_id, payload);

    SPS_Protocol_SendByte(SPS_SOF_1);
    SPS_Protocol_SendByte(SPS_SOF_2);
    SPS_Protocol_SendByte(length);
    SPS_Protocol_SendByte(cmd_id);

    for (uint8_t i = 0; i < length; i++)
    {
        SPS_Protocol_SendByte(payload[i]);
    }

    SPS_Protocol_SendByte((uint8_t)((crc >> 8) & 0xFFU));
    SPS_Protocol_SendByte((uint8_t)(crc & 0xFFU));

    while (LL_USART_IsActiveFlag_TC(USART1) == 0U)
    {
    }
}

void SPS_Protocol_SendAck(uint8_t ack_cmd_id)
{
    uint8_t payload[1];

    payload[0] = ack_cmd_id;

    SPS_Protocol_SendFrame(SPS_CMD_ACK, payload, 1U);
}

void SPS_Protocol_SendNack(uint8_t failed_cmd_id, uint8_t error_code)
{
    uint8_t payload[2];

    payload[0] = failed_cmd_id;
    payload[1] = error_code;

    SPS_Protocol_SendFrame(SPS_CMD_NACK, payload, 2U);
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
}
