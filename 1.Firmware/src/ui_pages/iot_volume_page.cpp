/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-07-31 00:24:51
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knob.h"
#include <motor.h>
#include <display.h>
#include <tuya_control.h>
#include <ble_keyboard.h>
lv_timer_t *volume_motor_sta_timer;
uint8_t volume_cnt=50;
lv_obj_t * volume_bar;

static void motor_status_read(void)
{
    static int now_num = 0;
    static int old_num = 0;
    now_num = get_motor_position();

    if (now_num > old_num)
    {
        old_num = get_motor_position();
        keyboard_player_volume_up();
        volume_cnt++;
        lv_bar_set_value(volume_bar, volume_cnt, LV_ANIM_ON);
    }
    else if (now_num < old_num)
    {
        old_num = get_motor_position();
        keyboard_player_volume_down();
        volume_cnt--;
        lv_bar_set_value(volume_bar, volume_cnt, LV_ANIM_ON);
    }
}

void ext_iot_volume_page(void)
{
    lv_timer_del(volume_motor_sta_timer);
    set_encoder_read_ignore(false);
}



void _volume_sta_timeout(lv_timer_t *timer)
{
    /*Use the user_data*/
    // uint32_t *user_data = (uint32_t *)timer->user_data;
    // Serial.print("------------------");
    // Serial.println(*user_data);
    motor_status_read();
}

/**
 * A temperature meter example
 */
void lv_example_bar_3(lv_obj_t * obj)
{
    static lv_style_t style_indic;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_bg_grad_dir(&style_indic, LV_GRAD_DIR_VER);

    volume_bar = lv_bar_create(obj);
    lv_obj_add_style(volume_bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(volume_bar, 40, 150);
    lv_obj_center(volume_bar);
    lv_bar_set_range(volume_bar, 0, 100);


}



void setup_scr_screen_volume(lv_ui *ui)
{
    ui->screen_iot_volume = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_iot_volume, 80, 160);

    lv_example_bar_3(ui->screen_iot_volume);


    //开启定时器 监听旋钮状态
    static uint32_t user_data = 10;
    volume_motor_sta_timer = lv_timer_create(_volume_sta_timeout, 100, &user_data);
    lv_timer_set_repeat_count(volume_motor_sta_timer, -1);

    //刷新页面调度器
    set_super_knob_page_status(IOT_VOLUME_PAGE);
    set_encoder_read_ignore(true);
}