/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:05:12
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-27 00:20:42
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
	lv_obj_t *screen1;
	lv_obj_t *screen1_tileview_1;
	lv_obj_t *screen1_tileview_1_tileview;
	lv_obj_t *screen1_meter_1;
	lv_obj_t *screen1_btn_1;
	lv_obj_t *screen1_btn_1_label;
	lv_obj_t *screen2;
	lv_obj_t *screen2_tileview_1;
	lv_obj_t *screen2_tileview_1_tileview;
	lv_obj_t *screen2_btn_1;
	lv_obj_t *screen2_btn_1_label;
    lv_group_t *defult_group;   
}lv_ui;

void setup_ui(lv_ui *ui);
extern lv_ui super_knod_ui;
void setup_scr_screen1(lv_ui *ui);
void setup_scr_screen2(lv_ui *ui);

#ifdef __cplusplus
}
#endif
#endif