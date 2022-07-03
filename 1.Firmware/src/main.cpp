/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-14 23:55:57
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-04 00:12:36
 */
// https://docs.simplefoc.com/bldcmotor

#include <main.h>
#include <display.h>
#include <motor.h>
#include <tuya_control.h>
#ifdef ENABLE_BLE_KEY_BOARD
#include <ble_keyboard.h>
#endif 
#include <Adafruit_NeoPixel.h>//关键是这个库


TaskHandle_t Task_foc_Handle;  //foc 任务
TaskHandle_t Task_lvgl_Handle; //lvgl 任务
TaskHandle_t Task_module_Handle; //子模块通讯任务

QueueHandle_t motor_msg_Queue;  //lvgl 接收消息队列
QueueHandle_t motor_rcv_Queue;  //motor 接收消息队列
_knob_message LVGL_MSG;
_knob_message MOTOR_MSG;

#define WS_PIN  11	//定义选择引脚
#define WS_NUM  4	//定义led个数，决定数组长度
Adafruit_NeoPixel pixels(WS_NUM, WS_PIN, NEO_GRB + NEO_KHZ800);


void setup()
{
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    Serial.begin(115200);
    Serial1.begin(115200);
    #ifdef ENABLE_BLE_KEY_BOARD
    ble_keyboard_init();
    #endif
    motor_msg_Queue = xQueueCreate(10, sizeof(struct _knob_message *));
    motor_rcv_Queue = xQueueCreate(10, sizeof(struct _knob_message *));

    /***WS2812******/
    pixels.begin();//初始化灯带
    pixels.clear();//清空灯带数组
    pixels.setPixelColor(0,pixels.Color(100, 0, 0));//注意是从0开始，第一个led对应0
    pixels.setPixelColor(1,pixels.Color(0, 100, 0));//三色对应(R,G,B)
    pixels.setPixelColor(2,pixels.Color(0, 0, 100));
    pixels.setPixelColor(3,pixels.Color(0, 0, 100));
    pixels.show();//刷新
    /***WS2812******/


    xTaskCreatePinnedToCore(
        Task_foc, "Task_foc", 4096, NULL, 2, &Task_foc_Handle, ESP32_RUNNING_CORE);
    xTaskCreatePinnedToCore(
        Task_lvgl, "Task_lvgl", 4096, NULL, 3, &Task_lvgl_Handle, LVGL_RUNNING_CORE);
    xTaskCreatePinnedToCore(
        Task_module, "Task_knob", 2048, NULL, 4, &Task_module_Handle, ESP32_RUNNING_CORE);
}

void loop()
{
    //vTaskDelay(1000);
}



