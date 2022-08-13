/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-08-13 08:54:37
 */
#include "user_app/super_knob_user_api.h"

LV_IMG_DECLARE(copy_img);
LV_IMG_DECLARE(paste_img);

static lv_timer_t *motor_sta_timer;


static void motor_status_read(void)
{
    static int now_num = 0;
    static int old_num = 0;
    now_num = get_motor_position();

    if (now_num > old_num)
    {
        old_num = get_motor_position();
        keyboard_pass_ctrl_c(); //该接口中有延时，待优化，可以再起一个任务 用消息队列来处理
    }
    else if (now_num < old_num)
    {
        old_num = get_motor_position();
        keyboard_pass_ctrl_v(); //该接口中有延时，待优化，可以再起一个任务 用消息队列来处理
    }
}

void ext_cv_developer_page(void)
{
    lv_timer_del(motor_sta_timer);
}


void motor_sta_timeout(lv_timer_t *timer)
{
    /*Use the user_data*/
    // uint32_t *user_data = (uint32_t *)timer->user_data;
    // Serial.print("------------------");
    // Serial.println(*user_data);
    motor_status_read();
}


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

void setup_scr_screen_cv_developer(lv_ui *ui)
{
    ui->screen_cv_developer = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_cv_developer, 80, 160);
    lv_obj_center(ui->screen_cv_developer);


    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_radius(&opa_style, 10);
    lv_style_set_bg_opa(&opa_style, 0);

    // lv_obj_t *exit_btn = lv_btn_create(ui->screen_cv_developer);
    // lv_obj_add_event_cb(exit_btn, exit_btn_event_handler, LV_EVENT_ALL, NULL);
    // lv_obj_set_style_radius(exit_btn, LV_RADIUS_CIRCLE, 0); //设置为圆形
    // lv_obj_set_size(exit_btn, 32, 32);
    // lv_obj_align(exit_btn, LV_ALIGN_TOP_MID, 0, 10);
    // lv_obj_add_style(exit_btn, &opa_style, 0);

    lv_obj_t *copy_btn = lv_btn_create(ui->screen_cv_developer);
    lv_obj_t *copy_btn_img = lv_img_create(copy_btn);
    lv_img_set_src(copy_btn_img, &copy_img);
    lv_obj_align(copy_btn, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_add_style(copy_btn, &opa_style, 0);

    lv_obj_t *paste_btn = lv_btn_create(ui->screen_cv_developer);
    lv_obj_t *paste_btn_img = lv_img_create(paste_btn);
    lv_img_set_src(paste_btn_img, &paste_img);
    lv_obj_align(paste_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_obj_add_style(paste_btn, &opa_style, 0);

    //开启定时器 监听旋钮状态
    static uint32_t user_data = 10;
    motor_sta_timer = lv_timer_create(motor_sta_timeout, 100, &user_data);
    lv_timer_set_repeat_count(motor_sta_timer, -1);


    //刷新页面调度器
    set_super_knob_page_status(IOT_CV_DEVELOPER_PAGE);
}


