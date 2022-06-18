/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-18 23:26:59
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knob.h"




static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_end_value(super_knob_ui.screen_alarm_clock, (lv_meter_indicator_t *)indic, v);
}

/**
 * A clock from a ui->screen_alarm_clock
 */
void setup_scr_screen_alarm_clock(lv_ui *ui)
{
    ui->screen_alarm_clock = lv_meter_create(NULL);

    lv_obj_set_size(ui->screen_alarm_clock, 220, 220);
    lv_obj_center(ui->screen_alarm_clock);

    /*为分钟创建刻度*/
    /*61 ticks in a 360 degrees range (the last and the first line overlaps)*/
    lv_meter_scale_t * scale_min = lv_meter_add_scale(ui->screen_alarm_clock);
    lv_meter_set_scale_ticks(ui->screen_alarm_clock, scale_min, 61, 1, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_range(ui->screen_alarm_clock, scale_min, 0, 60, 360, 270);

    /*创建小时的刻度*/
    lv_meter_scale_t * scale_hour = lv_meter_add_scale(ui->screen_alarm_clock);
    lv_meter_set_scale_ticks(ui->screen_alarm_clock, scale_hour, 12, 0, 0, lv_palette_main(LV_PALETTE_GREY));               /*12 ticks*/
    lv_meter_set_scale_major_ticks(ui->screen_alarm_clock, scale_hour, 1, 2, 20, lv_color_black(), 10);    /*Every tick is major*/
    lv_meter_set_scale_range(ui->screen_alarm_clock, scale_hour, 1, 12, 330, 300);       /*[1..12] values in an almost full circle*/

    LV_IMG_DECLARE(clock_img_hand)

    /*Add a the hands from images*/
    lv_meter_indicator_t * indic_min = lv_meter_add_needle_img(ui->screen_alarm_clock, scale_min, &clock_img_hand, 5, 5);
    lv_meter_indicator_t * indic_hour = lv_meter_add_needle_img(ui->screen_alarm_clock, scale_min, &clock_img_hand, 5, 5);

    //lv_meter_set_indicator_value(super_knob_ui.screen_alarm_clock, indic_min, 25); 主动设置数值

    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_values(&a, 0, 60); //设置动画的开始和结束值
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); //动画重复
    lv_anim_set_time(&a, 2000);     /*2 sec for 1 turn of the minute hand (1 hour) 设置动画持续时间*/
    lv_anim_set_var(&a, indic_min);
    lv_anim_start(&a);

    lv_anim_set_var(&a, indic_hour);
    lv_anim_set_time(&a, 24000);    /*24 sec for 1 turn of the hour hand 设置动画持续时间  */
    lv_anim_set_values(&a, 0, 60);
    lv_anim_start(&a);
}