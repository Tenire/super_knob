/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 14:15:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-06 01:34:44
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knod.h"
#include <ble_keyboard.h>
#include <motor.h>
#include <display.h>

lv_obj_t * btn_control  = NULL;

LV_IMG_DECLARE(music_img);       //图片初始化
LV_IMG_DECLARE(music_open_img);       //图片初始化
LV_IMG_DECLARE(music_stop_img);       //图片初始化

static void exit_btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        setup_scr_screen_iot_main(&super_knod_ui);
        lv_scr_load_anim(super_knod_ui.screen_iot_main_boday, LV_SCR_LOAD_ANIM_FADE_ON, 100, 10, true);
        set_super_knod_page_status(SUPER_PAGE_BUSY);
        update_motor_config(1);
        update_page_status(0);
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {

    }
}

static void player_btn_control_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        keyboard_enable_player();
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //keyboard_enable_player();
    }
}

void setup_scr_screen_player(lv_ui *ui)
{
    ui->screen_iot_player = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_iot_player, 240, 240);
    lv_obj_center(ui->screen_iot_player);

    /*Init the style for the default state*/
    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_radius(&style, 10);

    lv_style_set_bg_opa(&style, LV_OPA_100);
    lv_style_set_bg_color(&style, lv_color_make(0xaa, 0xaa, 0xf8));//#aaaaf8
    lv_style_set_bg_grad_color(&style, lv_color_make(0xaa, 0xaa, 0xf8));
    lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);

    lv_style_set_border_opa(&style, LV_OPA_40);
    lv_style_set_border_width(&style, 2);
    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_GREY));

    lv_style_set_shadow_width(&style, 8);
    lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_shadow_ofs_y(&style, 8);

    lv_style_set_outline_opa(&style, LV_OPA_COVER);
    lv_style_set_outline_color(&style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_pad_all(&style, 10);

    /*Init the pressed style*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);

    /*Add a large outline when pressed*/
    lv_style_set_outline_width(&style_pr, 30);
    lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

    lv_style_set_translate_y(&style_pr, 5);
    lv_style_set_shadow_ofs_y(&style_pr, 3);
    lv_style_set_bg_color(&style_pr, lv_color_make(0xaa, 0xaa, 0xf8));
    lv_style_set_bg_grad_color(&style_pr, lv_color_make(0xaa, 0xaa, 0xf8));

    /*Add a transition to the outline*/
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, (lv_style_prop_t)0};
    lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 300, 0, NULL);

    lv_style_set_transition(&style_pr, &trans);

    lv_obj_t * exit_btn = lv_btn_create(ui->screen_iot_player);
    lv_obj_add_event_cb(exit_btn, exit_btn_event_handler, LV_EVENT_ALL, NULL);
    //lv_obj_remove_style_all(btn1);                          /*Remove the style coming from the theme*/
    lv_obj_add_style(exit_btn, &style, 0);
    lv_obj_add_style(exit_btn, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(exit_btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(exit_btn, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t * label = lv_label_create(exit_btn);
    lv_label_set_text(label, "EXIT");
    lv_obj_center(label);

    lv_obj_t * img_1= lv_img_create(ui->screen_iot_player);
    lv_img_set_src(img_1, &music_img);
    lv_obj_center(img_1);

    lv_obj_t * btn_control = lv_btn_create(ui->screen_iot_player);
    lv_obj_add_event_cb(btn_control, player_btn_control_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn_control, &style, 0);
    lv_obj_add_style(btn_control, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(btn_control, 48, 48);
    lv_obj_t *img1 = lv_img_create(btn_control);
    lv_img_set_src(img1, &music_open_img);
    lv_obj_center(img1);
    lv_obj_align(btn_control, LV_ALIGN_BOTTOM_MID, 0, -10);


    //刷新页面调度器
    set_super_knod_page_status(IOT_COMPUTER_PAGE);
}