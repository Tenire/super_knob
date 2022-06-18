/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:05:23
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-18 23:26:24
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knob.h"

static SUPER_KNOB_PAGE_NUM super_knob_now  = WELCOME_PAGE;

SUPER_KNOB_PAGE_NUM get_super_knob_page_status(void)
{
    return super_knob_now;
}

void set_super_knob_page_status(SUPER_KNOB_PAGE_NUM now_page)
{
    super_knob_now = now_page;
}


void setup_ui(lv_ui *ui){
	setup_scr_screen_welcome(ui);
    lv_scr_load_anim(ui->screen_welcome, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 300, 100, true);
}
