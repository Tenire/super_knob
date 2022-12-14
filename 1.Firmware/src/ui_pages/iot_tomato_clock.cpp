/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-06-04 14:15:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-10 17:28:45
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knob.h"
#include <motor.h>
#include <display.h>
#include <tuya_control.h>



TimerHandle_t tomato_tmr;
TimerHandle_t alarm_clock;

LV_IMG_DECLARE(music_close_img); //图片初始化
LV_IMG_DECLARE(music_exit_img);
LV_IMG_DECLARE(work_img);
LV_IMG_DECLARE(rest_img);

lv_obj_t *tomato_time_label;
static lv_obj_t *bg_top;
static lv_obj_t *bg_bottom;
int tomato_hour_time = 0;
static int tomato_second_time = 60;
static int tomato_minute_time = 24;

//移除全部控件
static void lv_demo_printer_anim_out_all(lv_obj_t *obj, uint32_t delay);

void reset_time_cnt(int minute, int second)
{
    tomato_second_time = second;
    tomato_minute_time = minute;
}

void update_tomato_time(void)
{
    tomato_second_time--;

    char *print_buff = (char *)malloc(6);
    memset(print_buff, 0x00, 6);

    if (tomato_second_time < 10 && tomato_minute_time < 10)
    {
        sprintf(print_buff, "0%d:0%d", tomato_minute_time, tomato_second_time);
    }
    else if (tomato_second_time < 10)
    {
        sprintf(print_buff, "%d:0%d", tomato_minute_time, tomato_second_time);
    }
    else if (tomato_minute_time < 10)
    {
        sprintf(print_buff, "0%d:%d", tomato_minute_time, tomato_second_time);
    }
    else
    {
        sprintf(print_buff, "%d:%d", tomato_minute_time, tomato_second_time);
    }

    lv_label_set_text(tomato_time_label, print_buff);
    free(print_buff);

    if (tomato_second_time == 0)
    {
        tomato_minute_time--;
        tomato_second_time = 60;
    }

    if (tomato_minute_time == -1)
    {
        xTimerStop(tomato_tmr, 0);

        if(pdFALSE == xTimerIsTimerActive(alarm_clock))
        xTimerStart(alarm_clock, 0); //开启震动定时器
    }
}

void tomato_timeout(TimerHandle_t pxTimer)
{
    int32_t lArrayIndex;
    configASSERT(pxTimer);
    // 读取超时定时器的ID
    // lArrayIndex = (int32_t)pvTimerGetTimerID(pxTimer);
    // Serial.print("[E]network_connect_timeout----");
    // Serial.println(lArrayIndex);

    update_tomato_time();
}

void alarm_clock_timeout(TimerHandle_t pxTimer)
{
    int32_t lArrayIndex;
    configASSERT(pxTimer);
    // 读取超时定时器的ID
    // lArrayIndex = (int32_t)pvTimerGetTimerID(pxTimer);
    // Serial.print("[E]network_connect_timeout----");
    // Serial.println(lArrayIndex);
    update_page_status(BUTTON_CLICK);
}

static void exit_btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        if(xTimerIsTimerActive(tomato_tmr))
        xTimerStop(tomato_tmr, 0);
        if(xTimerIsTimerActive(alarm_clock))
        xTimerStop(alarm_clock, 0); //关闭闹钟定时器
        
        reset_time_cnt(24, 60);
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

static void work_btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        update_page_status(BUTTON_CLICK);
        if(pdFALSE == xTimerIsTimerActive(tomato_tmr))
        xTimerStart(tomato_tmr, 0); //开启倒计时定时器
        reset_time_cnt(24, 60);

    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
    }
}

static void rest_btn_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        update_page_status(BUTTON_CLICK);
        if(pdFALSE == xTimerIsTimerActive(tomato_tmr))
        xTimerStart(tomato_tmr, 0); //开启倒计时定时器
        reset_time_cnt(1, 60);

    }
    else if (code == LV_EVENT_VALUE_CHANGED)
    {
    }
}



void setup_scr_screen_tomato_clock(lv_ui *ui)
{
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_bg_opa(&style_btn, 0); //设置背景透明
    lv_style_set_radius(&style_btn, 15);


    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_opa(&style_text, 255);
    lv_style_set_text_color(&style_text, lv_color_black());

    /*Init the opa_style for the default state*/
    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_radius(&opa_style, 10);
    lv_style_set_bg_opa(&opa_style, LV_OPA_100);
    lv_style_set_bg_color(&opa_style, lv_color_make(0xaa, 0xaa, 0xf8)); //#aaaaf8
    lv_style_set_bg_grad_color(&opa_style, lv_color_make(0xaa, 0xaa, 0xf8));
    lv_style_set_bg_grad_dir(&opa_style, LV_GRAD_DIR_VER);

    lv_style_set_border_opa(&opa_style, LV_OPA_40);
    lv_style_set_border_width(&opa_style, 2);
    lv_style_set_border_color(&opa_style, lv_palette_main(LV_PALETTE_GREY));

    lv_style_set_shadow_width(&opa_style, 8);
    lv_style_set_shadow_color(&opa_style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_shadow_ofs_y(&opa_style, 8);

    lv_style_set_outline_opa(&opa_style, LV_OPA_COVER);
    lv_style_set_outline_color(&opa_style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_text_color(&opa_style, lv_color_white());
    lv_style_set_pad_all(&opa_style, 10);

    /*Init the pressed opa_style*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);
    /*Add a large outline when pressed*/
    lv_style_set_outline_width(&style_pr, 30);
    lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);
    lv_style_set_translate_y(&style_pr, 5);
    lv_style_set_shadow_ofs_y(&style_pr, 3);
    lv_style_set_bg_color(&style_pr, lv_color_make(0xaa, 0xaa, 0xf8));
    lv_style_set_bg_grad_color(&style_pr, lv_color_make(0xaa, 0xaa, 0xf8));

    ui->screen_iot_tomato_clock = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_iot_tomato_clock, 240, 240);
    lv_obj_center(ui->screen_iot_tomato_clock);

    //工作UI界面
    static lv_style_t style_working_bg;
    lv_style_init(&style_working_bg);
    lv_style_set_radius(&style_working_bg, 10);
    lv_style_set_bg_opa(&style_working_bg, LV_OPA_100);
    lv_style_set_bg_color(&style_working_bg, lv_color_make(0xaa, 0xaa, 0xf8)); //#aaaaf8
    lv_style_set_bg_grad_color(&style_working_bg, lv_color_make(0xaa, 0xaa, 0xf8));
    lv_style_set_bg_grad_dir(&style_working_bg, LV_GRAD_DIR_VER);

    lv_obj_t *working_bg = lv_obj_create(ui->screen_iot_tomato_clock);
    lv_obj_set_size(working_bg, 240, 120);
    lv_obj_add_style(working_bg, &style_working_bg, 0);
    lv_obj_fade_in(working_bg, 500, 500);


    tomato_time_label = lv_label_create(ui->screen_iot_tomato_clock);
    //lv_label_set_long_mode(tomato_time_label, LV_LABEL_LONG_CLIP); /*Circular scroll*/
    LV_FONT_DECLARE(lv_font_lcd_font_60);                          //加载字体
    lv_obj_set_style_text_font(tomato_time_label, &lv_font_lcd_font_60, 0);
    lv_label_set_text(tomato_time_label, "25:00");
    lv_obj_align(tomato_time_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_anim_down(tomato_time_label,600);


    lv_obj_t *working_btn = lv_btn_create(ui->screen_iot_tomato_clock);
    lv_obj_add_event_cb(working_btn, work_btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_size(working_btn, 120, 48);
    lv_obj_t *img_working = lv_img_create(working_btn);
    lv_img_set_src(img_working, &work_img);
    lv_obj_align(img_working, LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_add_style(working_btn, &style_btn, 0);
    lv_obj_align(working_btn, LV_ALIGN_BOTTOM_LEFT, 0, -10);
    lv_obj_add_anim_right(working_btn, 1000);

    lv_obj_t *rest_btn = lv_btn_create(ui->screen_iot_tomato_clock);
    lv_obj_add_event_cb(rest_btn, rest_btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_size(rest_btn, 120, 48);
    lv_obj_t *img_rest = lv_img_create(rest_btn);
    lv_img_set_src(img_rest, &rest_img);
    lv_obj_align(img_rest, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_add_style(rest_btn, &style_btn, 0);
    lv_obj_align(rest_btn, LV_ALIGN_BOTTOM_RIGHT, 0, -10);
    lv_obj_add_anim_left(rest_btn, 1000);

    lv_obj_t *exit_btn = lv_btn_create(ui->screen_iot_tomato_clock);
    lv_obj_add_event_cb(exit_btn, exit_btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(exit_btn, LV_RADIUS_CIRCLE, 0); //设置为圆形
    lv_obj_set_size(exit_btn, 32, 32);
    lv_obj_t *img_test = lv_img_create(exit_btn);
    lv_img_set_src(img_test, &music_exit_img);
    lv_obj_center(img_test);

    lv_obj_add_style(exit_btn, &opa_style, 0);
    lv_obj_add_style(exit_btn, &style_pr, LV_STATE_PRESSED);
    lv_obj_align(exit_btn, LV_ALIGN_TOP_MID, 0, 10);


    //刷新页面调度器
    set_super_knob_page_status(IOT_COMPUTER_PAGE);
    //创建相关定时器
    tomato_tmr = xTimerCreate("tomato_Timer", (1000), pdTRUE, (void *)0, tomato_timeout);
    alarm_clock = xTimerCreate("alarm_clock", (1000), pdTRUE, (void *)0, alarm_clock_timeout);
}