/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:05:12
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-29 13:29:40
 */
/*
 * Copyright 2022 NXP
 * SPDX-License-Identifier: MIT
 */

#ifndef GUI_SUPER_KNOD_H
#define GUI_SUPER_KNOD_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
typedef struct
{
    lv_obj_t *power_on_bar;
	lv_obj_t *screen_welcome;
	lv_obj_t *screen_iot_main;
    lv_obj_t *screen_iot_pointer;
    
    lv_indev_t *indev_encoder; //编码器输入
    lv_group_t *defult_group;  //默认组   
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui super_knod_ui;
void setup_scr_screen_welcome(lv_ui *ui);
void setup_scr_screen_iot_main(lv_ui *ui);
void setup_scr_screen_pointer(lv_ui *ui);

#ifdef __cplusplus
}
#endif
#endif