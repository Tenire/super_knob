/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-07-06 22:59:49
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-08-06 15:29:47
 */
#include "ws2812_driver.h"
#include <main.h>
#ifdef ENABLE_WS2812_LED
#include <Adafruit_NeoPixel.h>
#endif

Adafruit_NeoPixel strip = Adafruit_NeoPixel(WS2812_NUM, WS2812_PIN, NEO_GRB + NEO_KHZ800);

SemaphoreHandle_t ws2812_timeout_Semaphore;
TimerHandle_t ws2812_delay_timer;
TaskHandle_t Task_rainbowCycle_Handle; //ws2812 呼吸灯 任务

void WS2812_init(void)
{
    strip.begin();
    strip.setBrightness(100);
    strip.show(); 
}

void ws_delay_timeout(TimerHandle_t pxTimer)
{
    int32_t lArrayIndex;
    configASSERT(pxTimer);
    static BaseType_t xHigherPriorityTaskWoken;
    // 读取超时定时器的ID
    // lArrayIndex = (int32_t)pvTimerGetTimerID(pxTimer);
    // Serial.print("[E]ws_delay_timeout----");
    // Serial.println(lArrayIndex);
    xSemaphoreGiveFromISR( ws2812_timeout_Semaphore, &xHigherPriorityTaskWoken);

    //xTimerStop(pxTimer, 0);
}

void ws2812_smart_delay(uint32_t time_t)
{
    if(xTimerIsTimerActive(ws2812_delay_timer))
    {
        xTimerStop(ws2812_delay_timer, 0);//首先停止当前的定时器
        xTimerChangePeriod(ws2812_delay_timer, time_t, 0);//修改定时器周期
        xTimerReset(ws2812_delay_timer, 0);
    }else{
        xTimerChangePeriod(ws2812_delay_timer, time_t, 0);
        xTimerStart(ws2812_delay_timer, 0);
    }


    //阻塞等待超时信号量
    xSemaphoreTake(ws2812_timeout_Semaphore, portMAX_DELAY);
}

void ws2812_colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

uint32_t ws2812_wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
//整体渐变
void ws2812_rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, ws2812_wheel((i + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// 呼吸灯效果(rainbow进阶版本)
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, ws2812_wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
    for (int j = 0; j < 10; j++)
    { // do 10 cycles of chasing
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, c); // turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0); // turn every third pixel off
            }
        }
    }
}

// Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
    for (int j = 0; j < 256; j++)
    { // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, ws2812_wheel((i + j) % 255)); // turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0); // turn every third pixel off
            }
        }
    }
}

void ws2812_clear()
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, 0);
        delay(10);
    }
}

//流星
void ws2812_meteor(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait)
{
    const uint8_t num = 15;
    uint8_t max_color = red;
    if (green > max_color)
        max_color = green;
    if (blue > max_color)
        max_color = blue;
    uint8_t instance = (max_color - 200) / num;
    for (uint16_t i = 0; i < strip.numPixels() + num; i++)
    {
        for (uint8_t j = 0; j < num; j++)
        {
            if (i - j >= 0 && i - j < strip.numPixels())
            {
                int red_after = red - (instance * j);
                int green_after = green - (instance * j);
                int blue_after = blue - (instance * j);

                if (j >= 1)
                {
                    red_after -= 200;
                    green_after -= 200;
                    blue_after -= 200;
                }
                strip.setPixelColor(i - j, strip.Color(red_after >= 0 ? red_after : 0, green_after >= 0 ? green_after : 0, blue_after >= 0 ? blue_after : 0));
            }
        }
        if (i - num >= 0 && i - num < strip.numPixels())
            strip.setPixelColor(i - num, 0);

        strip.show();
        delay(wait);
    }
    ws2812_clear();
}

void ws2812_meteor_overturn(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait)
{

    const uint8_t num = 15;
    uint8_t max_color = red;
    if (green > max_color)
        max_color = green;
    if (blue > max_color)
        max_color = blue;
    uint8_t instance = (max_color - 200) / num;
    for (int i = strip.numPixels() - 1; i >= -num; i--)
    {
        for (uint8_t j = 0; j < num; j++)
        {
            if (i + j >= 0 && i + j < strip.numPixels())
            {
                int red_after = red - instance * j;
                int green_after = green - instance * j;
                int blue_after = blue - instance * j;
                if (j >= 1)
                {
                    red_after -= 200;
                    green_after -= 200;
                    blue_after -= 200;
                }
                strip.setPixelColor(i + j, strip.Color(red_after >= 0 ? red_after : 0, green_after >= 0 ? green_after : 0, blue_after >= 0 ? blue_after : 0));
            }
        }
        if (i + num >= 0 && i + num < strip.numPixels())
            strip.setPixelColor(i + num, 0);

        strip.show();
        delay(wait);
    }
}

void update_ws2812_status(WS2812_RUNNING_MODE_E status, int time)
{
    _ws2812_message *send_message;
    send_message = &WS2812_MSG;
    send_message->ws2812_status = status;
    send_message->ws2812_time = time;
    xQueueSend(ws2812_rcv_Queue, &send_message, (TickType_t)0);
}

//呼吸灯任务
void Task_rainbowCycle(void *pvParameters)
{
    for (;;)
    {
        rainbowCycle(10);
    }
}



void Task_ws2812(void *pvParameters)
{
    int ws2812_status = WS2812_NORMAL;

    // ws2812_timeout_Semaphore = xSemaphoreCreateBinary();

    // ws2812_delay_timer = xTimerCreate("delay_Timer", (10), pdTRUE, (void *)0, ws_delay_timeout);

    xTaskCreatePinnedToCore(
        Task_rainbowCycle, "Task_rainbowCycle_Handle", 1024, NULL, 4, &Task_rainbowCycle_Handle, ESP32_RUNNING_CORE);



    for (;;)
    {
        _ws2812_message *ws2812_message;
        if (xQueueReceive(ws2812_rcv_Queue, &(ws2812_message), (TickType_t)0))
        {
            ws2812_status = ws2812_message->ws2812_status;
        }

        switch (ws2812_status)
        {
        case WS2812_NORMAL:
            vTaskDelay(10);
            break;
        case WS2812_RAINBOWCYCLE:
            rainbowCycle(11);
            break;    
        case WS2812_RAINBOW:
            vTaskSuspend(Task_rainbowCycle_Handle);
            ws2812_rainbow(10);
            ws2812_status = WS2812_NORMAL;
            break;
        case WS2812_METEOR_OVERTURN:
            vTaskSuspend(Task_rainbowCycle_Handle);
            ws2812_colorWipe(strip.Color(255, 0, 0), 20); // Red
            ws2812_colorWipe(strip.Color(0, 255, 0), 20); // Green
            ws2812_colorWipe(strip.Color(0, 0, 255), 20); // Blue
            vTaskResume(Task_rainbowCycle_Handle);
            ws2812_status = WS2812_NORMAL;
            break;
        case WS2812_ROLL:
            vTaskSuspend(Task_rainbowCycle_Handle);
            ws2812_meteor(3, 0xa9, 0xf4, 10); //#03a9f4
            vTaskResume(Task_rainbowCycle_Handle);
            ws2812_status = WS2812_NORMAL;
            break;    
        
        default:
            break;
        }

    }
}


//void loop()
//{
    // 递进渐变
    // Some example procedures showing how to display to the pixels:
    // colorWipe(strip.Color(255, 0, 0), 20); // Red
    // colorWipe(strip.Color(0, 255, 0), 20); // Green
    // colorWipe(strip.Color(0, 0, 255), 20); // Blue

    // 交叉突变
    // Send a theater pixel chase in...
    // theaterChase(strip.Color(127, 127, 127), 50); // White
    // theaterChase(strip.Color(127, 0, 0), 50); // Red
    // theaterChase(strip.Color(0, 0, 127), 50); // Blue

    // 整体渐变
    //rainbow(12);

    // 呼吸灯效果(rainbow进阶版本)
    // rainbowCycle(10);

    // 交叉突变 + 整体渐变(rainbow进阶版本)
    // theaterChaseRainbow(50);

    // 流星
    // clear();
    // ws2812_meteor(255, 0, 0, 10);
    // ws2812_meteor_overturn(255, 0, 0, 10);
//}