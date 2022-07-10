/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:05:12
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-10 17:51:55
 */
/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_SUPER_KNOB_H
#define GUI_SUPER_KNOB_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
typedef struct
{
    lv_obj_t *power_on_bar;
	lv_obj_t *screen_welcome;
	lv_obj_t *screen_iot_main;
    lv_obj_t *screen_iot_main_boday;
    lv_obj_t *screen_iot_pointer;
    lv_meter_indicator_t * screen_iot_pointer_indic;

    lv_obj_t *screen_alarm_clock;
    lv_obj_t *screen_iot_sensor;
    lv_obj_t *screen_iot_player;
    lv_obj_t *screen_iot_player_play_img;

    lv_obj_t *screen_iot_tomato_clock;
    lv_obj_t *screen_iot_light_belt;
    lv_obj_t *screen_iot_smart_fan;
    
    lv_indev_t *indev_encoder; //编码器输入
    lv_group_t *defult_group;  //默认组   
}lv_ui;

typedef enum
{
    WELCOME_PAGE,
    IOT_MAIN_PAGE,
    IOT_POINTER_PAGE,
    IOT_SENSOR_PAGE,
    IOT_COMPUTER_PAGE,
    IOT_LIGHT_BELT_PAGE,
    IOT_SMART_FAN_PAGE,

    SUPER_PAGE_BUSY,
    SUPER_PAGE_MAX,
} SUPER_KNOB_PAGE_NUM;

void setup_ui(lv_ui *ui);
extern lv_ui super_knob_ui;
void setup_scr_screen_welcome(lv_ui *ui);
void setup_scr_screen_iot_main(lv_ui *ui);
void setup_scr_screen_pointer(lv_ui *ui);
void setup_scr_screen_alarm_clock(lv_ui *ui);
void setup_scr_screen_iot_sensor(lv_ui *ui);
void setup_scr_screen_player(lv_ui *ui);
void setup_scr_screen_tomato_clock(lv_ui *ui);
void setup_scr_screen_light_belt(lv_ui *ui);
void setup_scr_screen_smart_fan(lv_ui *ui);

void set_super_knob_page_status(SUPER_KNOB_PAGE_NUM now_page);
SUPER_KNOB_PAGE_NUM get_super_knob_page_status(void);
//从上至下动态加载控件
void lv_obj_add_anim_down(lv_obj_t *obj, uint32_t delay);
//从下至上动态加载控件
void lv_obj_add_anim_up(lv_obj_t *obj, uint32_t delay);
//从左至右动态加载控件
void lv_obj_add_anim_right(lv_obj_t *obj, uint32_t delay);
//从右至左动态加载控件
void lv_obj_add_anim_left(lv_obj_t *obj, uint32_t delay);


void ext_iot_light_belt_page(void);

/*
智能旋钮超级台灯插座电脑空调灯带学习传感器
*/

#ifdef __cplusplus
}
#endif
#endif