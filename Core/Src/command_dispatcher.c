#include "command_dispatcher.h"
<<<<<<< HEAD

#include <string.h>

#include "relay_service.h"
#include "projector_service.h"
#include "ir_service.h"
#include "ota_service.h"

extern osMessageQueueId_t relayQueueHandle;
extern osMessageQueueId_t projectorQueueHandle;
extern osMessageQueueId_t irQueueHandle;
extern osMessageQueueId_t otaQueueHandle;

static uint32_t RelayCmd_Pack(uint8_t sourceCmd,
                              uint8_t type,
                              uint8_t deviceId,
                              uint8_t action)
{
    return (((uint32_t)sourceCmd << RELAY_CMD_SOURCE_SHIFT) |
            ((uint32_t)type      << RELAY_CMD_TYPE_SHIFT)   |
            ((uint32_t)deviceId  << RELAY_CMD_DEVICE_SHIFT) |
            ((uint32_t)action    << RELAY_CMD_ACTION_SHIFT));
}

DispatchResult_t Command_DispatchFrame(const SPS_Frame_t *frame)
{
    uint32_t relayCmd;

    if (frame == NULL)
    {
        return DISPATCH_RESULT_INVALID_PARAM;
    }

    switch (frame->cmd_id)
    {
        case SPS_CMD_LIGHT_CONTROL:
            if (frame->length != 2U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            relayCmd = RelayCmd_Pack(frame->cmd_id,
                                     RELAY_TYPE_LIGHT,
                                     frame->payload[0],
                                     frame->payload[1]);

            if (osMessageQueuePut(relayQueueHandle, &relayCmd, 0U, 0U) != osOK)
            {
                return DISPATCH_RESULT_QUEUE_FULL;
            }

            return DISPATCH_RESULT_OK;

        case SPS_CMD_RELAY_MOTOR:
            if (frame->length != 2U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            relayCmd = RelayCmd_Pack(frame->cmd_id,
                                     RELAY_TYPE_MOTOR,
                                     frame->payload[0],
                                     frame->payload[1]);

            if (osMessageQueuePut(relayQueueHandle, &relayCmd, 0U, 0U) != osOK)
            {
                return DISPATCH_RESULT_QUEUE_FULL;
            }

            return DISPATCH_RESULT_OK;

        case SPS_CMD_RELAY_STATUS:
            if (frame->length != 1U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            /*
             * Relay status được xử lý trực tiếp trong commandTask,
             * không đưa vào relayQueue.
             */
            return DISPATCH_RESULT_OK;

        case SPS_CMD_PROJECTOR_CONTROL:
        {
            ProjectorCommand_t projectorCmd;

            if (frame->length != 1U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            projectorCmd.sourceCmd = frame->cmd_id;
            projectorCmd.action = frame->payload[0];

            if (osMessageQueuePut(projectorQueueHandle, &projectorCmd, 0U, 0U) != osOK)
            {
                return DISPATCH_RESULT_QUEUE_FULL;
            }

            return DISPATCH_RESULT_OK;
        }

        case SPS_CMD_AC_CONTROL:
        {
            IrCommand_t irCmd;

            if (frame->length != 2U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            irCmd.sourceCmd = frame->cmd_id;
            irCmd.acId = frame->payload[0];
            irCmd.action = frame->payload[1];

            if (osMessageQueuePut(irQueueHandle, &irCmd, 0U, 0U) != osOK)
            {
                return DISPATCH_RESULT_QUEUE_FULL;
            }

            return DISPATCH_RESULT_OK;
        }

        case SPS_CMD_OTA_START:
        {
            OtaCommand_t otaCmd;

            if (frame->length != 4U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            otaCmd.sourceCmd = frame->cmd_id;
            otaCmd.type = OTA_COMMAND_START;
            otaCmd.firmwareSize = ((uint32_t)frame->payload[0]) |
                                  ((uint32_t)frame->payload[1] << 8) |
                                  ((uint32_t)frame->payload[2] << 16) |
                                  ((uint32_t)frame->payload[3] << 24);
            otaCmd.chunkIndex = 0U;
            otaCmd.dataLength = 0U;

            if (osMessageQueuePut(otaQueueHandle, &otaCmd, 0U, 0U) != osOK)
            {
                return DISPATCH_RESULT_QUEUE_FULL;
            }

            return DISPATCH_RESULT_OK;
        }

        case SPS_CMD_OTA_CHUNK:
        {
            OtaCommand_t otaCmd;
            uint8_t chunkLength;

            if (frame->length < 2U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            chunkLength = (uint8_t)(frame->length - 1U);

            if (chunkLength > OTA_COMMAND_MAX_DATA_SIZE)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            memset(&otaCmd, 0, sizeof(otaCmd));

            otaCmd.sourceCmd = frame->cmd_id;
            otaCmd.type = OTA_COMMAND_CHUNK;
            otaCmd.chunkIndex = frame->payload[0];
            otaCmd.dataLength = chunkLength;

            memcpy(otaCmd.data, &frame->payload[1], chunkLength);

            if (osMessageQueuePut(otaQueueHandle, &otaCmd, 0U, 0U) != osOK)
            {
                return DISPATCH_RESULT_QUEUE_FULL;
            }

            return DISPATCH_RESULT_OK;
        }

        case SPS_CMD_OTA_END:
        {
            OtaCommand_t otaCmd;

            if (frame->length != 0U)
            {
                return DISPATCH_RESULT_INVALID_PARAM;
            }

            memset(&otaCmd, 0, sizeof(otaCmd));

            otaCmd.sourceCmd = frame->cmd_id;
            otaCmd.type = OTA_COMMAND_END;

            if (osMessageQueuePut(otaQueueHandle, &otaCmd, 0U, 0U) != osOK)
            {
                return DISPATCH_RESULT_QUEUE_FULL;
            }

            return DISPATCH_RESULT_OK;
        }

        default:
            return DISPATCH_RESULT_UNKNOWN_CMD;
=======
#include "relay_service.h"
#include "cmsis_os.h"

extern osMessageQueueId_t relayQueueHandle;

static void Command_DispatchLight(const sps_frame_t *frame)
{
    uint8_t light_id;
    uint8_t state;
    uint32_t relay_cmd;
    osStatus_t queue_status;

    if (frame->length != 2U)
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_BAD_LENGTH);
        return;
    }

    light_id = frame->payload[0];
    state = frame->payload[1];

    /*
     * Tai phan cung hien tai chi co 1 relay den phong.
     * Chap nhan:
     *   0x01: den buc
     *   0x02: den lop
     *   0xFF: tat ca
     */
    if (!((light_id == 0x01U) || (light_id == 0x02U) || (light_id == 0xFFU)))
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_INVALID_PARAM);
        return;
    }

    if (!((state == 0x00U) || (state == 0x01U)))
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_INVALID_PARAM);
        return;
    }

    relay_cmd = RELAY_CMD_PACK(RELAY_DEV_LIGHT, state, 0U, frame->cmd_id);

    queue_status = osMessageQueuePut(relayQueueHandle, &relay_cmd, 0U, 0U);

    if (queue_status == osOK)
    {
        SPS_Protocol_SendAck(frame->cmd_id);
    }
    else
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_QUEUE_FULL);
    }
}

static void Command_DispatchRelayMotor(const sps_frame_t *frame)
{
    uint8_t device_id;
    uint8_t action;
    uint8_t relay_device;
    uint32_t relay_cmd;
    osStatus_t queue_status;

    if (frame->length != 2U)
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_BAD_LENGTH);
        return;
    }

    device_id = frame->payload[0];
    action = frame->payload[1];

    /*
     * Theo protocol:
     *   payload[0] = 0x01: Rem cua
     *   payload[0] = 0x02: Man chieu
     */
    if (device_id == 0x01U)
    {
        relay_device = RELAY_DEV_CURTAIN;
    }
    else if (device_id == 0x02U)
    {
        relay_device = RELAY_DEV_SCREEN;
    }
    else
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_INVALID_PARAM);
        return;
    }

    /*
     * Theo protocol:
     *   0x01: Mo / Len
     *   0x00: Dong / Xuong
     *   0x02: Dung
     */
    if (!((action == 0x00U) || (action == 0x01U) || (action == 0x02U)))
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_INVALID_PARAM);
        return;
    }

    relay_cmd = RELAY_CMD_PACK(relay_device, action, 0U, frame->cmd_id);

    queue_status = osMessageQueuePut(relayQueueHandle, &relay_cmd, 0U, 0U);

    if (queue_status == osOK)
    {
        SPS_Protocol_SendAck(frame->cmd_id);
    }
    else
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_QUEUE_FULL);
    }
}

static void Command_DispatchRelayStatus(const sps_frame_t *frame)
{
    uint8_t query_id;
    uint8_t relay_device;
    uint8_t status_payload[2];

    if (frame->length != 1U)
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_BAD_LENGTH);
        return;
    }

    query_id = frame->payload[0];

    /*
     * Ho tro ca 2 cach goi:
     *   0x01 hoac 0x21: den
     *   0x02: rem
     *   0x03: man chieu
     */
    if ((query_id == 0x01U) || (query_id == SPS_CMD_LIGHT_CONTROL))
    {
        relay_device = RELAY_DEV_LIGHT;
    }
    else if (query_id == 0x02U)
    {
        relay_device = RELAY_DEV_CURTAIN;
    }
    else if (query_id == 0x03U)
    {
        relay_device = RELAY_DEV_SCREEN;
    }
    else
    {
        SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_INVALID_PARAM);
        return;
    }

    status_payload[0] = query_id;
    status_payload[1] = Relay_Service_GetStatus(relay_device);

    SPS_Protocol_SendFrame(SPS_CMD_RELAY_STATUS, status_payload, 2U);
}

void Command_DispatchFrame(const sps_frame_t *frame)
{
    switch (frame->cmd_id)
    {
        case SPS_CMD_PING:
        {
            if (frame->length == 0U)
            {
                SPS_Protocol_SendAck(SPS_CMD_PING);
            }
            else
            {
                SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_BAD_LENGTH);
            }
            break;
        }

        case SPS_CMD_LIGHT_CONTROL:
        {
            Command_DispatchLight(frame);
            break;
        }

        case SPS_CMD_RELAY_CONTROL:
        {
            Command_DispatchRelayMotor(frame);
            break;
        }

        case SPS_CMD_RELAY_STATUS:
        {
            Command_DispatchRelayStatus(frame);
            break;
        }

        default:
        {
            SPS_Protocol_SendNack(frame->cmd_id, SPS_ERR_UNKNOWN_CMD);
            break;
        }
>>>>>>> 01ccb4f4c884c2ffb501141d05a78716c179a428
    }
}
