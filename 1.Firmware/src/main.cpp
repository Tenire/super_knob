/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-14 23:55:57
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-30 00:50:14
 */
// https://docs.simplefoc.com/bldcmotor

#include <main.h>
#include <display.h>
#include <motor.h>

TaskHandle_t Task_foc_Handle;  //foc 任务
TaskHandle_t Task_lvgl_Handle; //lvgl 任务

QueueHandle_t motor_msg_Queue;  //lvgl 接收消息队列
QueueHandle_t foc_rcv_Queue;  //foc 接收消息队列
_motor_message MOTOR_MSG;


void setup()
{
    Serial.begin(115200);

    motor_msg_Queue = xQueueCreate(10, sizeof(struct motor_message *));

    xTaskCreatePinnedToCore(
        Task_foc, "Task_foc", 4096, NULL, 2, &Task_foc_Handle, ESP32_RUNNING_CORE);
    xTaskCreatePinnedToCore(
        Task_lvgl, "Task_lvgl", 4096, NULL, 3, &Task_lvgl_Handle, LVGL_RUNNING_CORE);
}

void loop()
{
    //vTaskDelay(1000);
}



