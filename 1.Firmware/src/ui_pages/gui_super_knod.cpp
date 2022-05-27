/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:05:23
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-27 00:58:34
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knod.h"


void setup_ui(lv_ui *ui){
	setup_scr_screen1(ui);
	lv_scr_load(ui->screen1);
    //lv_scr_load_anim(guider_ui.screen2, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 100, true);
}
