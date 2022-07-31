#include <display.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <main.h>
#include <motor.h>
#include "ui_pages/gui_super_knob.h"
#include <ws2812_driver.h>
/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-05-22 00:19:50
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-07-31 00:15:25
 */

TimerHandle_t poweron_tmr;
static const uint16_t screenWidth = 80;
static const uint16_t screenHeight = 160;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf_1[screenWidth * screenHeight];
static bool encoder_read_ignore = false;
lv_ui super_knob_ui;


TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}
/*-----------------------------------------------------------------------*/

static bool touch_pad_press(int gpio)
{
    static int press_cnt = 0;
    bool ret_cnt = false;
    if(touchRead(gpio) < 15){
        press_cnt ++;
        //10ms 消抖
        if (press_cnt > 4) {
            if( touchRead(gpio) < 15){
                ret_cnt = true;
                press_cnt = -8;
            }
        }
    }else{
        press_cnt = 0;
    }
    
    return ret_cnt;
}


//检测页面退出逻辑
void page_status_check(void)
{
    SUPER_KNOB_PAGE_NUM now_page = get_super_knob_page_status();
    switch (now_page)
    {
    // case WELCOME_PAGE:
    //     break;
    // case WELCOME_PAGE:
    //     break;
    case IOT_LIGHT_BELT_PAGE:
        if(touch_pad_press(ESP32_TOUCH_PIN2)){
            ext_iot_light_belt_page();
            setup_scr_screen_iot_main(&super_knob_ui);
            lv_scr_load_anim(super_knob_ui.screen_iot_main_boday, LV_SCR_LOAD_ANIM_FADE_ON, 100, 10, false);
            set_super_knob_page_status(SUPER_PAGE_BUSY);
            update_motor_config(1);
            update_page_status(CHECKOUT_PAGE);
        }
        break;
    case IOT_VOLUME_PAGE:
        if(touch_pad_press(ESP32_TOUCH_PIN2)){
            ext_iot_volume_page();
            setup_scr_screen_iot_main(&super_knob_ui);
            lv_scr_load_anim(super_knob_ui.screen_iot_main_boday, LV_SCR_LOAD_ANIM_FADE_ON, 100, 10, false);
            set_super_knob_page_status(SUPER_PAGE_BUSY);
            update_motor_config(1);
            update_page_status(CHECKOUT_PAGE);
        }
    case IOT_SENSOR_PAGE:
    case IOT_POINTER_PAGE:
        if(touch_pad_press(ESP32_TOUCH_PIN2)){
            setup_scr_screen_iot_main(&super_knob_ui);
            lv_scr_load_anim(super_knob_ui.screen_iot_main_boday, LV_SCR_LOAD_ANIM_FADE_ON, 100, 10, false);
            set_super_knob_page_status(SUPER_PAGE_BUSY);
            update_motor_config(1);
            update_page_status(CHECKOUT_PAGE);
        }
        break;
    default:
        break;
    }

}

void set_encoder_read_ignore(bool status)
{
    encoder_read_ignore = status;
}

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static int now_num = 0;
    static int old_num = 0;
    now_num = get_motor_position();

    data->enc_diff = 0;
    if (now_num > old_num)
    {
        data->enc_diff++;
        old_num = get_motor_position();
        if(!encoder_read_ignore)
        update_ws2812_status(WS2812_ROLL, 10);
    }
    else if (now_num < old_num)
    {
        data->enc_diff--;
        old_num = get_motor_position();
        if(!encoder_read_ignore)
        update_ws2812_status(WS2812_ROLL, 10);
    }

    if(touch_pad_press(ESP32_TOUCH_PIN2)){
        update_ws2812_status(WS2812_METEOR_OVERTURN, 10);
        data->state = LV_INDEV_STATE_PR;
    }else{
        data->state = LV_INDEV_STATE_REL;
    }

    page_status_check();
}

void poweron_timeout(TimerHandle_t pxTimer)
{
    int32_t lArrayIndex;
    configASSERT(pxTimer);
    // 读取超时定时器的ID
    lArrayIndex = (int32_t)pvTimerGetTimerID(pxTimer);
    Serial.print("[E]network_connect_timeout----");
    Serial.println(lArrayIndex);

    xTimerStop(pxTimer, 0);
}



void check_timerout(lv_timer_t *timer)
{
    setup_scr_screen_iot_main(&super_knob_ui);
    //加载动画
    lv_scr_load_anim(super_knob_ui.screen_iot_main_boday, LV_SCR_LOAD_ANIM_OVER_TOP, 200, 50, true);
}

void update_page_status(int page_status)
{
    struct _knob_message *send_message;
    send_message = &LVGL_MSG;
    send_message->ucMessageID = page_status;
    xQueueSend(motor_rcv_Queue, &send_message, (TickType_t)0);
    
    Serial.print("-------------------->>>>");
    Serial.println(send_message->ucMessageID);
}


void Task_lvgl(void *pvParameters)
{
    (void)pvParameters;

    /*初始化显示*/
    lv_init();

    tft.begin();        /* TFT init */
    tft.setRotation(0); /* Landscape orientation, flipped */
    lv_disp_draw_buf_init(&draw_buf, buf_1, NULL, screenWidth * screenHeight); //开启双缓冲
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv); //注册显示屏

    /*加载输入设备  按键或者键盘 编码器 或者触摸*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    super_knob_ui.indev_encoder = lv_indev_drv_register(&indev_drv);
    //想要和按键交互必须创建一个对象组： 并且必须使用以下命令将对象添加到其中
    
    super_knob_ui.defult_group = lv_group_create();
    lv_group_set_default(super_knob_ui.defult_group);
    lv_indev_set_group(super_knob_ui.indev_encoder, super_knob_ui.defult_group);

    setup_ui(&super_knob_ui);
    
    update_motor_config(1);
    update_page_status(0);

    //创建开机页面超时定时器
    // poweron_tmr = xTimerCreate("poweron_Timer", (400), pdTRUE, (void *)0, poweron_timeout);
    // xTimerStart(poweron_tmr, 0); //开启倒计时定时器

    for (;;)
    {
        //监听电机运行状态
        struct _knob_message *motor_message;
        if (xQueueReceive(motor_msg_Queue, &(motor_message), (TickType_t)1))
        {
            Serial.print("lvgl_msg_Queue --->");
            Serial.println(motor_message->ucMessageID);
            switch(motor_message->ucMessageID){
                case MOTOR_INIT:
                    if(super_knob_ui.power_on_bar)
                    lv_bar_set_value(super_knob_ui.power_on_bar, 30, LV_ANIM_ON);
                break;
                case MOTOR_INIT_SUCCESS:
                    if(super_knob_ui.power_on_bar)
                    lv_bar_set_value(super_knob_ui.power_on_bar, 75, LV_ANIM_ON);
                break;
                case MOTOR_INIT_END:
                {
                    if(super_knob_ui.power_on_bar)
                    lv_bar_set_value(super_knob_ui.power_on_bar, 100, LV_ANIM_ON);
                    
                    lv_timer_t *_check_timer = lv_timer_create(check_timerout, 800, NULL);
                    lv_timer_set_repeat_count(_check_timer, 1);
                }    
                break;
                default:
                break;
            }
        }

        lv_task_handler(); /* let the GUI do its work */
        vTaskDelay(1);
    }
}