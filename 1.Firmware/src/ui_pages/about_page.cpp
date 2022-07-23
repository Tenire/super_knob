/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-07-24 01:10:50
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knob.h"
#include <motor.h>
#include <display.h>
#include <tuya_control.h>

static void exit_btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        setup_scr_screen_iot_main(&super_knob_ui);
        lv_scr_load_anim(super_knob_ui.screen_iot_main_boday, LV_SCR_LOAD_ANIM_FADE_ON, 100, 100, true);
        set_super_knob_page_status(SUPER_PAGE_BUSY);
        update_motor_config(1);
        update_page_status(CHECKOUT_PAGE);
    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
    }
}

void setup_scr_screen_about(lv_ui *ui)
{
    ui->screen_about = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_about, 240, 240);

    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_radius(&opa_style, 10);
    lv_style_set_bg_opa(&opa_style, 0);

    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_opa(&style_text, 255);
    lv_style_set_text_color(&style_text, lv_color_make(0x1e, 0x27, 0x32));

    lv_obj_t* label = lv_label_create(ui->screen_about);
    LV_FONT_DECLARE(lv_font_super_knob_30); //加载字体
    lv_obj_set_style_text_font(label, &lv_font_super_knob_30, 0);
    lv_label_set_text(label, "Super Knob!");
    lv_obj_add_style(label, &style_text, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t* www_label = lv_label_create(ui->screen_about);
    lv_label_set_text(www_label, "www.wenzheng.club");
    lv_obj_add_style(www_label, &style_text, 0);
    lv_obj_align(www_label, LV_ALIGN_CENTER, 0, 40);

    lv_obj_t *exit_btn = lv_btn_create(ui->screen_about);
    lv_obj_add_event_cb(exit_btn, exit_btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(exit_btn, LV_RADIUS_CIRCLE, 0); //设置为圆形
    lv_obj_set_size(exit_btn, 32, 32);
    lv_obj_align(exit_btn, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_style(exit_btn, &opa_style, 0);

    //刷新页面调度器
    set_super_knob_page_status(ABOUT_PAGE);
}