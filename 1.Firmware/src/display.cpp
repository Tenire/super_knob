#include <display.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <main.h>
#include <motor.h>
#include "ui_pages/gui_super_knod.h"
/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-05-22 00:19:50
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-29 13:35:15
 */

TimerHandle_t poweron_tmr;
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf_1[screenWidth * 30];
static lv_color_t buf_2[screenWidth * 30];

lv_ui super_knod_ui;


TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    //开启DMA传输
    tft.startWrite();
    tft.setSwapBytes(true);
    tft.pushImageDMA(area->x1, area->y1, w, h,(uint16_t *)&color_p->full);
    tft.dmaWait();
    tft.endWrite();

    lv_disp_flush_ready(disp);
}
/*-----------------------------------------------------------------------*/

/*Will be called by the library to read the encoder*/
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    static int now_num = 0;
    static int old_num = 0;
    now_num = config.position;

    data->enc_diff = 0;
    if (now_num > old_num)
    {
        data->enc_diff++;
        old_num = config.position;
    }
    else if (now_num < old_num)
    {
        data->enc_diff--;
        old_num = config.position;
    }
    Serial.println(touchRead(32));

    if(touchRead(32) < 15){
        data->state = LV_INDEV_STATE_PR;
    }else{
        data->state = LV_INDEV_STATE_REL;
    }
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



void check_timer(lv_timer_t *timer)
{
    setup_scr_screen_iot_main(&super_knod_ui);
    //禁用加载动画
    //lv_scr_load_anim(super_knod_ui.screen_iot_main, LV_SCR_LOAD_ANIM_FADE_ON, 500, 100, true);
}


void Task_lvgl(void *pvParameters)
{
    (void)pvParameters;

    /*初始化显示*/
    lv_init();

    tft.begin();        /* TFT init */
    tft.initDMA();
    tft.setRotation(0); /* Landscape orientation, flipped */
    lv_disp_draw_buf_init(&draw_buf, buf_1, buf_2, screenWidth * 30); //开启双缓冲
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
    super_knod_ui.indev_encoder = lv_indev_drv_register(&indev_drv);
    //想要和按键交互必须创建一个对象组： 并且必须使用以下命令将对象添加到其中
    
    super_knod_ui.defult_group = lv_group_create();
    lv_group_set_default(super_knod_ui.defult_group);
    lv_indev_set_group(super_knod_ui.indev_encoder, super_knod_ui.defult_group);

    setup_ui(&super_knod_ui);

    //创建开机页面超时定时器
    poweron_tmr = xTimerCreate("poweron_Timer", (400), pdTRUE, (void *)0, poweron_timeout);
    xTimerStart(poweron_tmr, 0); //开启倒计时定时器

    for (;;)
    {
        //监听电机运行状态
        struct _motor_message *motor_message;
        if (xQueueReceive(motor_msg_Queue, &(motor_message), (TickType_t)1))
        {
            Serial.println("------------------------------------>>>>>>");
            Serial.println(motor_message->ucMessageID);
            switch(motor_message->ucMessageID){
                case MOTOR_INIT:
                    lv_bar_set_value(super_knod_ui.power_on_bar, 30, LV_ANIM_ON);
                break;
                case MOTOR_INIT_SUCCESS:
                    lv_bar_set_value(super_knod_ui.power_on_bar, 75, LV_ANIM_ON);
                break;
                case MOTOR_INIT_END:
                {
                    lv_bar_set_value(super_knod_ui.power_on_bar, 100, LV_ANIM_ON);
                    
                    lv_timer_t *_check_timer = lv_timer_create(check_timer, 800, NULL);
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