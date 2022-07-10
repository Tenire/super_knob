/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-15 17:02:59
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-10 18:44:42
 */
#pragma once

#include <Arduino.h>

#define ENABLE_BLE_KEY_BOARD
#define ENABLE_WS2812_LED

#define LVGL_RUNNING_CORE   0            //内核选择   LVGL用核心0  其他用核心1
#define ESP32_RUNNING_CORE  1            //内核选择   LVGL用核心0  其他用核心1
#define ESP32_TOUCH_PIN1    33           //
#define ESP32_TOUCH_PIN2    32           //
#define ESP32_TOUCH_PIN3    12           //
#define WS2812_PIN          11           //定义选择引脚
#define WS2812_NUM          4            //定义led个数，决定数组长度

struct _knob_message
{
    int ucMessageID;
    char ucData[100];
};

struct _ws2812_message
{
    int ws2812_status;
    int ws2812_time;
};
struct _iot_control_message
{
    int ucMessageID;
    char ucData[100];
};

extern _knob_message LVGL_MSG;
extern _knob_message MOTOR_MSG;
extern _ws2812_message WS2812_MSG;
extern _iot_control_message IOT_CONTROL_MSG;

extern QueueHandle_t motor_msg_Queue;  //lvgl 接收消息队列
extern QueueHandle_t motor_rcv_Queue;  //motor 接收消息队列
extern QueueHandle_t ws2812_rcv_Queue;  //ws2812 接收消息队列
extern QueueHandle_t iot_control_Queue;  //ws2812 接收消息队列

