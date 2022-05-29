/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:05:23
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-29 13:42:48
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knod.h"


void setup_ui(lv_ui *ui){
	setup_scr_screen_welcome(ui);
    lv_scr_load_anim(ui->screen_welcome, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 300, 100, true);
}
