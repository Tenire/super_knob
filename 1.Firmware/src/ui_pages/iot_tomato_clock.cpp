/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 14:15:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-19 16:22:56
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knob.h"
#include <motor.h>
#include <display.h>

TimerHandle_t tomato_tmr;
LV_IMG_DECLARE(music_close_img);       //图片初始化

lv_obj_t* tomato_time_label;
int tomato_hour_time = 0;


void update_tomato_time(void)
{
    static int tomato_second_time = 60;
    static int tomato_minute_time = 24;
    tomato_second_time --;

    char * print_buff = (char *)malloc(6);
    memset(print_buff, 0x00, 6);

    if(tomato_second_time < 10 && tomato_minute_time < 10){
        sprintf(print_buff,"0%d:0%d", tomato_minute_time, tomato_second_time);
    }else if(tomato_second_time < 10){
        sprintf(print_buff,"%d:0%d", tomato_minute_time, tomato_second_time);
    }else if(tomato_minute_time < 10){
        sprintf(print_buff,"0%d:%d", tomato_minute_time, tomato_second_time);
    }else{
        sprintf(print_buff,"%d:%d", tomato_minute_time, tomato_second_time);
    }
    
    lv_label_set_text(tomato_time_label, print_buff);
    free(print_buff);


    if(tomato_second_time == 0){
        tomato_minute_time --;
        tomato_second_time = 60;
    }

    if(tomato_minute_time == -1){
        xTimerStop(tomato_tmr, 0);
    }
}


void tomato_timeout(TimerHandle_t pxTimer)
{
    int32_t lArrayIndex;
    configASSERT(pxTimer);
    // 读取超时定时器的ID
    //lArrayIndex = (int32_t)pvTimerGetTimerID(pxTimer);
    // Serial.print("[E]network_connect_timeout----");
    // Serial.println(lArrayIndex);

    update_tomato_time();
    
}


void setup_scr_screen_tomato_clock(lv_ui *ui)
{
    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_opa(&style_text, 255);
    lv_style_set_text_color(&style_text, lv_color_black());

    ui->screen_iot_tomato_clock = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_iot_tomato_clock, 240, 240);
    lv_obj_center(ui->screen_iot_tomato_clock);

    tomato_time_label = lv_label_create(ui->screen_iot_tomato_clock);
    lv_label_set_long_mode(tomato_time_label, LV_LABEL_LONG_CLIP);     /*Circular scroll*/
    LV_FONT_DECLARE(lv_font_lcd_font_60); //加载字体
    lv_obj_set_style_text_font(tomato_time_label, &lv_font_lcd_font_60, 0);
    lv_label_set_text(tomato_time_label, "25:00");
    lv_obj_align(tomato_time_label, LV_ALIGN_CENTER, 0, 0);

    //刷新页面调度器
    set_super_knob_page_status(IOT_COMPUTER_PAGE);
    tomato_tmr = xTimerCreate("tomato_Timer", (1000), pdTRUE, (void *)0, tomato_timeout);
    xTimerStart(tomato_tmr, 0); //开启倒计时定时器
}