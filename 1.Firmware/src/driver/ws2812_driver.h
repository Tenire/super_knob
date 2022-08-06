/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-07-06 23:00:05
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-09 11:57:55
 */
#pragma once
#include <Arduino.h>


typedef enum
{
    WS2812_NORMAL,
    WS2812_RAINBOWCYCLE,  
    WS2812_RAINBOW,
    WS2812_METEOR_OVERTURN,
    WS2812_ROLL,
}WS2812_RUNNING_MODE_E;




void WS2812_init(void);
void ws2812_meteor(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait);
uint32_t ws2812_wheel(byte WheelPos);
void ws2812_rainbow(uint8_t wait);

void Task_ws2812(void *pvParameters);
void update_ws2812_status(WS2812_RUNNING_MODE_E status, int time);



