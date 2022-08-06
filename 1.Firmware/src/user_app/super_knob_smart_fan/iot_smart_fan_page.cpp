/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 14:15:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-08-06 16:05:52
 */
#include "user_app/super_knob_user_api.h"
lv_obj_t * btn_open_label;
LV_IMG_DECLARE(fan_big_img);       //图片初始化
char *open_smart_fan = "{\"101\":true}";
char *close_smart_fan = "{\"101\":false}";
char *open_smart_fan_shake = "{\"103\":true}";
char *close_smart_fan_shake = "{\"103\":false}";
char *set_smart_fan_speed = "{\"102\":60}";
char *check_smart_fan_speed = "{\"102\":30}";

static void btn_ext_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        setup_scr_screen_iot_main(&super_knob_ui);
        lv_scr_load_anim(super_knob_ui.screen_iot_main_boday, LV_SCR_LOAD_ANIM_FADE_ON, 100, 10, true);
        set_super_knob_page_status(SUPER_PAGE_BUSY);
        update_motor_config(1);
        update_page_status(CHECKOUT_PAGE);
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {

    }
}

static void btn_open_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    static int play_cnt = 0;
    if(code == LV_EVENT_CLICKED) {
        update_page_status(BUTTON_CLICK);
        if (play_cnt){
            send_iot_control_msg(0, close_smart_fan);
            lv_label_set_text(btn_open_label, "OPEN");
            play_cnt = 0;
        }else{
            play_cnt = 1;
            send_iot_control_msg(0, open_smart_fan);
            lv_label_set_text(btn_open_label, "CLOSE");
        }
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //keyboard_enable_player();
    }
}

static void btn_shake_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    static int play_cnt = 0;
    if(code == LV_EVENT_CLICKED) {
        update_page_status(BUTTON_CLICK);
        if (play_cnt){
            send_iot_control_msg(0, close_smart_fan_shake);
            play_cnt = 0;
        }else{
            play_cnt = 1;
            send_iot_control_msg(0, open_smart_fan_shake);
        }
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //keyboard_enable_player();
    }
}

static void btn_speed_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    static int play_cnt = 0;
    if(code == LV_EVENT_CLICKED) {
        update_page_status(BUTTON_CLICK);
        if (play_cnt){
            send_iot_control_msg(0, set_smart_fan_speed);
            play_cnt = 0;
        }else{
            play_cnt = 1;
            send_iot_control_msg(0, check_smart_fan_speed);
        }
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //keyboard_enable_player();
    }
}

void setup_scr_screen_smart_fan(lv_ui *ui)
{
    ui->screen_iot_smart_fan = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_iot_smart_fan, 240, 240);
    lv_obj_center(ui->screen_iot_smart_fan);

    lv_obj_t * panel = lv_obj_create(ui->screen_iot_smart_fan);
    lv_obj_set_size(panel, 100, 240);
    lv_obj_set_scroll_snap_y(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_align(panel, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF); //从不显示滚动条

    /*Properties to transition*/
    static lv_style_prop_t props[] = {
    LV_STYLE_TRANSFORM_WIDTH, LV_STYLE_TRANSFORM_HEIGHT, LV_STYLE_TEXT_LETTER_SPACE, (lv_style_prop_t)0};

    /*Transition descriptor when going back to the default state.
     *Add some delay to be sure the press transition is visible even if the press was very short*/
    static lv_style_transition_dsc_t transition_dsc_def;
    lv_style_transition_dsc_init(&transition_dsc_def, props, lv_anim_path_overshoot, 250, 100, NULL);

    /*Transition descriptor when going to pressed state.
     *No delay, go to presses state immediately*/
    static lv_style_transition_dsc_t transition_dsc_pr;
    lv_style_transition_dsc_init(&transition_dsc_pr, props, lv_anim_path_ease_in_out, 250, 0, NULL);

    /*Add only the new transition to he default state*/
    static lv_style_t style_def;
    lv_style_init(&style_def);
    lv_style_set_transition(&style_def, &transition_dsc_def);

    /*Add the transition and some transformation to the presses state.*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    lv_style_set_transform_width(&style_pr, 10);
    lv_style_set_transform_height(&style_pr, -10);
    lv_style_set_text_letter_space(&style_pr, 10);
    lv_style_set_transition(&style_pr, &transition_dsc_pr);

    lv_obj_t * btn_open = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_open, btn_open_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn_open, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_open, &style_def, 0);
    btn_open_label = lv_label_create(btn_open);
    lv_label_set_text(btn_open_label, "OPEN");
    lv_obj_center(btn_open_label);

    lv_obj_t * btn_shake = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_shake, btn_shake_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn_shake, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_shake, &style_def, 0);
    lv_obj_t * btn_shake_label = lv_label_create(btn_shake);
    lv_label_set_text(btn_shake_label, "SHAKE");
    lv_obj_center(btn_shake_label);

    lv_obj_t * btn_speed = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_speed, btn_speed_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn_speed, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_speed, &style_def, 0);
    lv_obj_t * btn_speed_label = lv_label_create(btn_speed);
    lv_label_set_text(btn_speed_label, "SPEED");
    lv_obj_center(btn_speed_label);

    lv_obj_t * btn_ext = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_ext, btn_ext_event_handler, LV_EVENT_ALL, NULL);
    //lv_obj_align(btn_ext, LV_ALIGN_TOP_MID, 0, 30);
    lv_obj_add_style(btn_ext, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_ext, &style_def, 0);
    lv_obj_t * btn_ext_label = lv_label_create(btn_ext);
    lv_label_set_text(btn_ext_label, "EXIT");
    lv_obj_center(btn_ext_label);

    lv_obj_add_anim_left(panel, 50);
    lv_obj_update_snap(panel, LV_ANIM_ON);

    lv_obj_t * fan_img = lv_img_create(ui->screen_iot_smart_fan);
    lv_img_set_src(fan_img, &fan_big_img);
    lv_obj_align(fan_img, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_fade_in(fan_img, 500, 501);


    //刷新页面调度器
    set_super_knob_page_status(IOT_COMPUTER_PAGE);
}