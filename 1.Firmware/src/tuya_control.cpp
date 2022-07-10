/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-18 18:00:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-10 17:44:42
 */
#include <tuya_control.h>
#include <main.h>


void send_iot_control_msg(int status, char *msg)
{
    _iot_control_message *iot_control_message;
    iot_control_message = &IOT_CONTROL_MSG;
    iot_control_message->ucMessageID = status;
    memset(iot_control_message->ucData, 0x00, sizeof(iot_control_message->ucData));
    strcpy(iot_control_message->ucData, msg);
    xQueueSend(iot_control_Queue, &iot_control_message, (TickType_t)0);
}


void Task_module(void *pvParameters)
{
    for (;;)
    {
        _iot_control_message *iot_control_message;
        //阻塞等待消息队列
        if (xQueueReceive(iot_control_Queue, &(iot_control_message), portMAX_DELAY))
        {
            switch (iot_control_message->ucMessageID)
            {
            case 0:
                Serial.println(iot_control_message->ucData);
                Serial1.println(iot_control_message->ucData);
                break;
            default:
                break;
            }
        }
    }
}
