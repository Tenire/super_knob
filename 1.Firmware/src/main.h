/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-15 17:02:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-22 16:06:52
 */
#pragma once

#include <Arduino.h>

struct _motor_message
{
    int ucMessageID;
    char ucData[100];
};
extern _motor_message MOTOR_MSG;

extern QueueHandle_t motor_msg_Queue;  //lvgl 接收消息队列

//void update_motor_status(motor_message *motor_status);

