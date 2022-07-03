/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-18 18:00:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-02 17:45:47
 */
#include "tuya_control.h"

void Task_module(void *pvParameters)
{
    for (;;)
    {
        Serial1.println("test----------------------->>>>");
        vTaskDelay(1000);
    }
}