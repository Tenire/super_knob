/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-14 23:55:57
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-08-13 09:27:53
 */
// https://docs.simplefoc.com/bldcmotor

#include <main.h>
#include <core/display.h>
#include <core/motor.h>
#include <driver/tuya_control.h>
#ifdef ENABLE_BLE_KEY_BOARD
#include <driver/ble_keyboard.h>
#endif 
#include <driver/ws2812_driver.h>
#include "driver/driver_sht30.h"

TaskHandle_t Task_foc_Handle;  //foc 任务
TaskHandle_t Task_lvgl_Handle; //lvgl 任务
TaskHandle_t Task_module_Handle; //子模块通讯任务
TaskHandle_t Task_ws2812_Handle; //ws2812 任务

QueueHandle_t motor_msg_Queue;  //lvgl 接收消息队列
QueueHandle_t motor_rcv_Queue;  //motor 接收消息队列
QueueHandle_t ws2812_rcv_Queue;  //ws2812 接收消息队列
QueueHandle_t iot_control_Queue;  //ws2812 接收消息队列

_knob_message LVGL_MSG;
_knob_message MOTOR_MSG;
_ws2812_message WS2812_MSG;
_iot_control_message IOT_CONTROL_MSG;

void super_knob_print_logo(void);
void setup()
{
    Serial.begin(115200);
    super_knob_print_logo();
    Serial1.begin(115200);
    #ifdef ENABLE_BLE_KEY_BOARD
    ble_keyboard_init();
    #endif
    motor_msg_Queue = xQueueCreate(10, sizeof(_knob_message *));
    motor_rcv_Queue = xQueueCreate(10, sizeof(_knob_message *));
    ws2812_rcv_Queue = xQueueCreate(10, sizeof(_ws2812_message *));
    iot_control_Queue = xQueueCreate(10, sizeof(_ws2812_message *));

    WS2812_init();
    driver_sht30_init();
    
    #ifdef ESP32C3_ENABLE
        xTaskCreate(Task_foc, "Task_foc", 4096, NULL, 2, &Task_foc_Handle);
        xTaskCreate(Task_lvgl, "Task_lvgl", 4096, NULL, 3, &Task_lvgl_Handle);
        xTaskCreate(Task_module, "Task_knob", 2048, NULL, 4, &Task_module_Handle);
        xTaskCreate(Task_ws2812, "Task_ws2812", 1024, NULL, 4, &Task_ws2812_Handle);
    #else
        xTaskCreatePinnedToCore(
            Task_foc, "Task_foc", 4096, NULL, 2, &Task_foc_Handle, ESP32_RUNNING_CORE);
        xTaskCreatePinnedToCore(
            Task_lvgl, "Task_lvgl", 4096, NULL, 3, &Task_lvgl_Handle, LVGL_RUNNING_CORE);
        xTaskCreatePinnedToCore(
            Task_module, "Task_knob", 2048, NULL, 4, &Task_module_Handle, ESP32_RUNNING_CORE);
        xTaskCreatePinnedToCore(
            Task_ws2812, "Task_ws2812", 1024, NULL, 4, &Task_ws2812_Handle, ESP32_RUNNING_CORE);
    #endif
}

void loop()
{
    delay(1000);
}


void super_knob_print_logo(void)
{
    Serial.println("\r\n\r\n");
    Serial.println("         _          _ _                                     _                _       ");
    Serial.println("        | |__   ___| | | ___    ___ _   _ _ __   ___ _ __  | | ___ __   ___ | |__    ");
    Serial.println("        | '_ \\ / _ \\ | |/ _ \\  / __| | | | '_ \\ / _ \\ '__| | |/ / '_ \\ / _ \\| '_ \\");    
    Serial.println("        | | | |  __/ | | (_) | \\__ \\ |_| | |_) |  __/ |    |   <| | | | (_) | |_) |   ");
    Serial.println("        |_| |_|\\___|_|_|\\___/  |___/\\__,_| .__/ \\___|_|    |_|\\_\\_| |_|\\___/|_.__/    ");
    Serial.println("                                         |_|                                          ");                   

}

