/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:05:23
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-18 22:43:01
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knod.h"

static SUPER_KNOD_PAGE_NUM super_knod_now  = WELCOME_PAGE;

SUPER_KNOD_PAGE_NUM get_super_knod_page_status(void)
{
    return super_knod_now;
}

void set_super_knod_page_status(SUPER_KNOD_PAGE_NUM now_page)
{
    super_knod_now = now_page;
}


void setup_ui(lv_ui *ui){
	setup_scr_screen_welcome(ui);
    lv_scr_load_anim(ui->screen_welcome, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 300, 100, true);
}
