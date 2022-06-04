/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-15 17:02:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-04 15:30:05
 */
#pragma once

#include <Arduino.h>

//#define ENABLE_BLE_KEY_BOARD

struct _knod_message
{
    int ucMessageID;
    char ucData[100];
};
extern _knod_message LVGL_MSG;
extern _knod_message MOTOR_MSG;

extern QueueHandle_t motor_msg_Queue;  //lvgl 接收消息队列
extern QueueHandle_t motor_rcv_Queue;  //motor 接收消息队列

//void update_motor_status(motor_message *motor_status);

