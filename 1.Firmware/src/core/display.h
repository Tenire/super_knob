#pragma once
#include <Arduino.h>

/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-22 00:19:58
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-07-31 00:12:05
 */

typedef enum
{
    CHECKOUT_PAGE,
    BUTTON_CLICK

} DISPLAY_STATUS_E;



void update_page_status(int page_status);
void Task_lvgl(void *pvParameters);
void set_encoder_read_ignore(bool status);