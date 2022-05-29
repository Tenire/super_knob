/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-29 16:59:07
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knod.h"


static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_end_value(super_knod_ui.screen_iot_sensor, (lv_meter_indicator_t *)indic, v);
}

/**
 * A ui->screen_iot_sensor with multiple arcs
 */
void setup_scr_screen_iot_sensor(lv_ui *ui)
{
    ui->screen_iot_sensor = lv_meter_create(NULL);
    lv_obj_center(ui->screen_iot_sensor);
    lv_obj_set_size(ui->screen_iot_sensor, 200, 200);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(ui->screen_iot_sensor, NULL, LV_PART_INDICATOR);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(ui->screen_iot_sensor);
    lv_meter_set_scale_ticks(ui->screen_iot_sensor, scale, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(ui->screen_iot_sensor, scale, 1, 2, 30, lv_color_hex3(0xeee), 15);
    lv_meter_set_scale_range(ui->screen_iot_sensor, scale, 0, 100, 270, 90);

    /*Add a three arc indicator*/
    lv_meter_indicator_t * indic1 = lv_meter_add_arc(ui->screen_iot_sensor, scale, 10, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_indicator_t * indic2 = lv_meter_add_arc(ui->screen_iot_sensor, scale, 10, lv_palette_main(LV_PALETTE_GREEN), -10);
    lv_meter_indicator_t * indic3 = lv_meter_add_arc(ui->screen_iot_sensor, scale, 10, lv_palette_main(LV_PALETTE_BLUE), -20);

    /*Create an animation to set the value*/
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, set_value);
    lv_anim_set_values(&a, 0, 100);
    lv_anim_set_repeat_delay(&a, 100);
    lv_anim_set_playback_delay(&a, 100);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);

    lv_anim_set_time(&a, 2000);
    lv_anim_set_playback_time(&a, 500);
    lv_anim_set_var(&a, indic1);
    lv_anim_start(&a);

    lv_anim_set_time(&a, 1000);
    lv_anim_set_playback_time(&a, 1000);
    lv_anim_set_var(&a, indic2);
    lv_anim_start(&a);

    lv_anim_set_time(&a, 1000);
    lv_anim_set_playback_time(&a, 2000);
    lv_anim_set_var(&a, indic3);
    lv_anim_start(&a);
}