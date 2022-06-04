/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-14 23:55:57
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-04 15:30:25
 */
// https://docs.simplefoc.com/bldcmotor

#include <main.h>
#include <display.h>
#include <motor.h>
#ifdef ENABLE_BLE_KEY_BOARD
#include <ble_keyboard.h>
#endif 


TaskHandle_t Task_foc_Handle;  //foc 任务
TaskHandle_t Task_lvgl_Handle; //lvgl 任务

QueueHandle_t motor_msg_Queue;  //lvgl 接收消息队列
QueueHandle_t motor_rcv_Queue;  //motor 接收消息队列
_knod_message LVGL_MSG;
_knod_message MOTOR_MSG;


void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    #if ENABLE_BLE_KEY_BOARD
    ble_keyboard_init();
    #endif
    motor_msg_Queue = xQueueCreate(10, sizeof(struct _knod_message *));
    motor_rcv_Queue = xQueueCreate(10, sizeof(struct _knod_message *));

    xTaskCreatePinnedToCore(
        Task_foc, "Task_foc", 4096, NULL, 2, &Task_foc_Handle, ESP32_RUNNING_CORE);
    xTaskCreatePinnedToCore(
        Task_lvgl, "Task_lvgl", 4096, NULL, 3, &Task_lvgl_Handle, LVGL_RUNNING_CORE);
}

void loop()
{
    //vTaskDelay(1000);
}



