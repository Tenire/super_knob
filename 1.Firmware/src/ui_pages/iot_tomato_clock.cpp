/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 14:15:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-18 22:56:28
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knod.h"
#include <motor.h>
#include <display.h>


LV_IMG_DECLARE(music_close_img);       //图片初始化



void setup_scr_screen_tomato_clock(lv_ui *ui)
{
    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_opa(&style_text, 255);
    lv_style_set_text_color(&style_text, lv_color_black());

    ui->screen_iot_tomato_clock = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_iot_tomato_clock, 240, 240);
    lv_obj_center(ui->screen_iot_tomato_clock);

    lv_obj_t* label = lv_label_create(ui->screen_iot_tomato_clock);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    // LV_FONT_DECLARE(lv_font_lcd_font_60); //加载字体
    // lv_obj_set_style_text_font(label, &lv_font_lcd_font_60, 0);
    // lv_label_set_text(label, "1234");

    LV_FONT_DECLARE(lv_font_super_knob_30); //加载字体
    lv_obj_set_style_text_font(label, &lv_font_super_knob_30, 0);
    lv_label_set_text(label, "Super Knob!");
    lv_obj_add_style(label, &style_text, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);



    //刷新页面调度器
    set_super_knod_page_status(IOT_COMPUTER_PAGE);
}