/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-07-31 00:12:41
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knob.h"
#include <motor.h>
#include <display.h>



static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_value(super_knob_ui.screen_iot_pointer, (lv_meter_indicator_t *)indic, v);
}

void indic_update_timer(lv_timer_t *timer)
{
    /*Use the user_data*/
    uint32_t *user_data = (uint32_t *)timer->user_data;
    //Serial.println(*user_data);
    lv_meter_set_indicator_value(super_knob_ui.screen_iot_pointer, super_knob_ui.screen_iot_pointer_indic, get_motor_position()); //主动设置数值

}

/**
 * A simple ui->screen_iot_pointer
 */
void setup_scr_screen_pointer(lv_ui *ui)
{
    ui->screen_iot_pointer =  lv_meter_create(NULL);

    lv_obj_center(ui->screen_iot_pointer);
    lv_obj_set_size(ui->screen_iot_pointer, 240, 240);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(ui->screen_iot_pointer);
    lv_meter_set_scale_ticks(ui->screen_iot_pointer, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(ui->screen_iot_pointer, scale, 8, 4, 15, lv_color_black(), 10);

    lv_meter_indicator_t * indic;

    /*Add a blue arc to the start*/
    indic = lv_meter_add_arc(ui->screen_iot_pointer, scale, 3, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_meter_set_indicator_start_value(ui->screen_iot_pointer, indic, 0);
    lv_meter_set_indicator_end_value(ui->screen_iot_pointer, indic, 20);

    /*Make the tick lines blue at the start of the scale*/
    indic = lv_meter_add_scale_lines(ui->screen_iot_pointer, scale, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_BLUE), false, 0);
    lv_meter_set_indicator_start_value(ui->screen_iot_pointer, indic, 0);
    lv_meter_set_indicator_end_value(ui->screen_iot_pointer, indic, 20);

    /*Add a red arc to the end*/
    indic = lv_meter_add_arc(ui->screen_iot_pointer, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(ui->screen_iot_pointer, indic, 80);
    lv_meter_set_indicator_end_value(ui->screen_iot_pointer, indic, 100);

    /*Make the tick lines red at the end of the scale*/
    indic = lv_meter_add_scale_lines(ui->screen_iot_pointer, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(ui->screen_iot_pointer, indic, 80);
    lv_meter_set_indicator_end_value(ui->screen_iot_pointer, indic, 100);

    /*Add a needle line indicator*/
    ui->screen_iot_pointer_indic = lv_meter_add_needle_line(ui->screen_iot_pointer, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

    //开启定时器。定时刷新指针
    static uint32_t user_data = 10;
    lv_timer_t *_indic_update_timer = lv_timer_create(indic_update_timer, 100, &user_data);
    lv_timer_set_repeat_count(_indic_update_timer, -1);

    //刷新页面调度器
    set_super_knob_page_status(IOT_POINTER_PAGE);
    set_encoder_read_ignore(true);

    /*Create an animation to set the value*/
    // lv_anim_t a;
    // lv_anim_init(&a);
    // lv_anim_set_exec_cb(&a, set_value);
    // lv_anim_set_var(&a, indic);
    // lv_anim_set_values(&a, 0, 100);
    // lv_anim_set_time(&a, 2000);
    // lv_anim_set_repeat_delay(&a, 100);
    // lv_anim_set_playback_time(&a, 500);
    // lv_anim_set_playback_delay(&a, 100);
    // lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    // lv_anim_start(&a);
}