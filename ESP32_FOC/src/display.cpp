#include <display.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <main.h>
#include <motor.h>
/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-05-22 00:19:50
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-23 00:44:17
 */

TimerHandle_t poweron_tmr;

/*Change to your screen resolution*/
static const uint16_t screenWidth = 240;
static const uint16_t screenHeight = 240;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
lv_group_t *defult_group;              //默认组
lv_indev_t *indev_encoder;      //编码器输入
LV_IMG_DECLARE(motor_img);      //图片初始化
LV_IMG_DECLARE(instrument_img); //图片初始化
LV_IMG_DECLARE(chip_img);       //图片初始化
LV_IMG_DECLARE(lamp_img);       //图片初始化

lv_obj_t* power_on_bar;


TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */
/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}
/*-----------------------------------------------------------------------*/

//动画时间线
static lv_anim_timeline_t *anim_timeline = NULL;

static lv_obj_t *obj1 = NULL;
static lv_obj_t *obj2 = NULL;
static lv_obj_t *obj3 = NULL;

static const lv_coord_t obj_width = 48;
static const lv_coord_t obj_height = 48;

static void set_width(void *var, int32_t v)
{
    lv_obj_set_width((lv_obj_t *)var, v);
}

static void set_height(void *var, int32_t v)
{
    lv_obj_set_height((lv_obj_t *)var, v);
}

static void anim_timeline_create(void)
{
    /*
    3个对象，对应6个动画 宽高逐渐缩小

    动画轨迹
    lv_anim_path_linear 线性动画
    lv_anim_path_step最后一步改变
    lv_anim_path_ease_in 开始时很慢
    lv_anim_path_ease_out 最后慢
    lv_anim_path_ease_in_out 开始和结束都很慢
    lv_anim_path_overshoot 超过结束值
    lv_anim_path_bounce 从最终值反弹一点（比如撞墙）
    */
    /* obj1 */
    lv_anim_t a1;
    lv_anim_init(&a1);
    lv_anim_set_var(&a1, obj1);                              //设置动画对象
    lv_anim_set_values(&a1, 0, obj_width);                   //设置动画的开始和结束值
    lv_anim_set_early_apply(&a1, false);                     //设置动画是立即应用还是仅在延迟过期时应用。
    lv_anim_set_exec_cb(&a1, (lv_anim_exec_xcb_t)set_width); //在动画过程中执行的函数
    lv_anim_set_path_cb(&a1, lv_anim_path_overshoot);        //轨迹超过结束值
    lv_anim_set_time(&a1, 300);

    lv_anim_t a2;
    lv_anim_init(&a2);
    lv_anim_set_var(&a2, obj1);
    lv_anim_set_values(&a2, 0, obj_height);
    lv_anim_set_early_apply(&a2, false);
    lv_anim_set_exec_cb(&a2, (lv_anim_exec_xcb_t)set_height);
    lv_anim_set_path_cb(&a2, lv_anim_path_ease_out); //轨迹最后很慢
    lv_anim_set_time(&a2, 300);

    /* obj2 */
    lv_anim_t a3;
    lv_anim_init(&a3);
    lv_anim_set_var(&a3, obj2);
    lv_anim_set_values(&a3, 0, obj_width);
    lv_anim_set_early_apply(&a3, false);
    lv_anim_set_exec_cb(&a3, (lv_anim_exec_xcb_t)set_width);
    lv_anim_set_path_cb(&a3, lv_anim_path_overshoot);
    lv_anim_set_time(&a3, 300);

    lv_anim_t a4;
    lv_anim_init(&a4);
    lv_anim_set_var(&a4, obj2);
    lv_anim_set_values(&a4, 0, obj_height);
    lv_anim_set_early_apply(&a4, false);
    lv_anim_set_exec_cb(&a4, (lv_anim_exec_xcb_t)set_height);
    lv_anim_set_path_cb(&a4, lv_anim_path_ease_out);
    lv_anim_set_time(&a4, 300);

    /* obj3 */
    lv_anim_t a5;
    lv_anim_init(&a5);
    lv_anim_set_var(&a5, obj3);
    lv_anim_set_values(&a5, 0, obj_width);
    lv_anim_set_early_apply(&a5, false);
    lv_anim_set_exec_cb(&a5, (lv_anim_exec_xcb_t)set_width);
    lv_anim_set_path_cb(&a5, lv_anim_path_overshoot);
    lv_anim_set_time(&a5, 300);

    lv_anim_t a6;
    lv_anim_init(&a6);
    lv_anim_set_var(&a6, obj3);
    lv_anim_set_values(&a6, 0, obj_height);
    lv_anim_set_early_apply(&a6, false);
    lv_anim_set_exec_cb(&a6, (lv_anim_exec_xcb_t)set_height);
    lv_anim_set_path_cb(&a6, lv_anim_path_ease_out);
    lv_anim_set_time(&a6, 300);

    /* Create anim timeline */
    // 创建动画时间线 并将动画添加到时间线中
    anim_timeline = lv_anim_timeline_create();
    lv_anim_timeline_add(anim_timeline, 0, &a1);
    lv_anim_timeline_add(anim_timeline, 0, &a2);
    lv_anim_timeline_add(anim_timeline, 200, &a3);
    lv_anim_timeline_add(anim_timeline, 200, &a4);
    lv_anim_timeline_add(anim_timeline, 400, &a5);
    lv_anim_timeline_add(anim_timeline, 400, &a6);
}
/**
 * Create an animation timeline
 */
void lv_poweron_anim_timeline(void)
{
    lv_obj_t *par = lv_scr_act();
    lv_obj_set_flex_flow(par, LV_FLEX_FLOW_ROW); //将子元素 排成一行
    lv_obj_set_flex_align(par, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* create slider_prg */
    static lv_style_t style_bg;
    static lv_style_t style_indic;

    lv_style_init(&style_bg);
    lv_style_set_border_color(&style_bg, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_bg, 2);
    lv_style_set_pad_all(&style_bg, 6); /*To make the indicator smaller*/
    lv_style_set_radius(&style_bg, 6);
    lv_style_set_anim_time(&style_bg, 1000);

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_radius(&style_indic, 3);

    power_on_bar = lv_bar_create(par);
    lv_obj_remove_style_all(power_on_bar);  /*To have a clean start*/
    lv_obj_add_style(power_on_bar, &style_bg, 0);
    lv_obj_add_style(power_on_bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(power_on_bar, 200, 20);
    lv_obj_center(power_on_bar);
    lv_bar_set_value(power_on_bar, 0, LV_ANIM_ON);
    lv_obj_add_flag(power_on_bar, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(power_on_bar, LV_ALIGN_CENTER, 0, 20);

    /* create 3 objects */
    obj1 = lv_img_create(par);
    lv_img_set_src(obj1, &motor_img);
    lv_obj_set_size(obj1, obj_width, obj_height);
    lv_obj_add_flag(obj1, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(obj1, LV_ALIGN_CENTER, -60, -20);

    obj2 = lv_img_create(par);
    lv_img_set_src(obj2, &instrument_img);
    lv_obj_set_size(obj2, obj_width, obj_height);
    lv_obj_add_flag(obj2, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(obj2, LV_ALIGN_CENTER, 0, -20);

    obj3 = lv_img_create(par);
    lv_img_set_src(obj3, &chip_img);
    lv_obj_set_size(obj3, obj_width, obj_height);
    lv_obj_add_flag(obj3, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(obj3, LV_ALIGN_CENTER, 60, -20);
}

static void scroll_event_cb(lv_event_t *e)
{
    lv_obj_t *cont = lv_event_get_target(e);

    lv_area_t cont_a;
    lv_obj_get_coords(cont, &cont_a);
    lv_coord_t cont_y_center = cont_a.y1 + lv_area_get_height(&cont_a) / 2;

    lv_coord_t r = lv_obj_get_height(cont) * 7 / 10;
    uint32_t i;
    uint32_t child_cnt = lv_obj_get_child_cnt(cont);
    for (i = 0; i < child_cnt; i++)
    {
        lv_obj_t *child = lv_obj_get_child(cont, i);
        lv_area_t child_a;
        lv_obj_get_coords(child, &child_a);

        lv_coord_t child_y_center = child_a.y1 + lv_area_get_height(&child_a) / 2;

        lv_coord_t diff_y = child_y_center - cont_y_center;
        diff_y = LV_ABS(diff_y);

        /*Get the x of diff_y on a circle.*/
        lv_coord_t x;
        /*If diff_y is out of the circle use the last point of the circle (the radius)*/
        if (diff_y >= r)
        {
            x = r;
        }
        else
        {
            /*Use Pythagoras theorem to get x from radius and y*/
            uint32_t x_sqr = r * r - diff_y * diff_y;
            lv_sqrt_res_t res;
            lv_sqrt(x_sqr, &res, 0x8000); /*Use lvgl's built in sqrt root function*/
            x = r - res.i;
        }

        /*Translate the item by the calculated X coordinate*/
        lv_obj_set_style_translate_x(child, x, 0);

        /*Use some opacity with larger translations*/
        lv_opa_t opa = lv_map(x, 0, r, LV_OPA_TRANSP, LV_OPA_COVER);
        lv_obj_set_style_opa(child, LV_OPA_COVER - opa, 0);
    }
}

/**
 * Translate the object as they scroll
 */
void lv_example_scroll_a(void)
{
    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_size(cont, 240, 240);

    lv_obj_center(cont);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(cont, scroll_event_cb, LV_EVENT_SCROLL, NULL); //每次位置变化时触发
    lv_obj_set_style_radius(cont, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_style_clip_corner(cont, true, 0);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER); //滚动方向为上下滚动
    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_CENTER); //将子对象与滚动对象的中心对齐
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF); //从不显示滚动条

    uint32_t i;
    for (i = 0; i < 20; i++)
    {
        lv_obj_t *normal_obj = lv_btn_create(cont);
        lv_obj_set_width(normal_obj, lv_pct(100));//百分比
        lv_obj_set_height(normal_obj, lv_pct(20));

        lv_obj_t *img1 = lv_img_create(normal_obj);
        lv_img_set_src(img1, &lamp_img);
        lv_obj_align(img1, LV_ALIGN_LEFT_MID, 0, 0);

        lv_obj_t* line1 = lv_line_create(normal_obj);
        static lv_point_t line_points[] = { {50, 2}, {50, 15}}; 
        lv_line_set_points(line1, line_points, 2);

    }

    /*Update the buttons position manually for first*/
    lv_event_send(cont, LV_EVENT_SCROLL, NULL);

    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(cont, 0), LV_ANIM_OFF);
}

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
    data->state = LV_INDEV_STATE_REL;
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

void welcome_timer(lv_timer_t *timer)
{
    /*Use the user_data*/
    uint32_t *user_data = (uint32_t *)timer->user_data;
    Serial.println(*user_data);

    static int in_cnt = 0;
    in_cnt ++;

    if(in_cnt > 1)
    {
       // lv_example_scroll_a();
    }

    if (!anim_timeline)
    {
        anim_timeline_create();
    }

    /*Do something with LVGL*/
    //是否反转动画时间线
    // lv_anim_timeline_set_reverse(anim_timeline, true);
    //启动时间线
    lv_anim_timeline_start(anim_timeline);

}

void check_timer(lv_timer_t *timer)
{

    lv_example_scroll_a();

}

void Task_lvgl(void *pvParameters)
{
    (void)pvParameters;

    /*初始化显示*/
    lv_init();
    tft.begin();        /* TFT init */
    tft.setRotation(0); /* Landscape orientation, flipped */
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);
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
    indev_encoder = lv_indev_drv_register(&indev_drv);
    //想要和按键交互必须创建一个对象组： 并且必须使用以下命令将对象添加到其中
    defult_group = lv_group_create();
    lv_group_set_default(defult_group);
    lv_indev_set_group(indev_encoder, defult_group);

    lv_poweron_anim_timeline();

    static uint32_t user_data = 10;
    lv_timer_t *_welcom_timer = lv_timer_create(welcome_timer, 500, &user_data);
    lv_timer_set_repeat_count(_welcom_timer, 1);

    //创建开机页面超时定时器
    poweron_tmr = xTimerCreate("poweron_Timer", (400), pdTRUE, (void *)0, poweron_timeout);
    xTimerStart(poweron_tmr, 0); //开启倒计时定时器

    for (;;)
    {
        struct _motor_message *motor_message;
        if (xQueueReceive(motor_msg_Queue, &(motor_message), (TickType_t)10))
        {
            Serial.println("------------------------------------>>>>>>");
            Serial.println(motor_message->ucMessageID);
            switch(motor_message->ucMessageID){
                case MOTOR_INIT:
                    lv_bar_set_value(power_on_bar, 30, LV_ANIM_ON);
                break;
                case MOTOR_INIT_SUCCESS:
                    lv_bar_set_value(power_on_bar, 75, LV_ANIM_ON);
                break;
                case MOTOR_INIT_END:
                {
                    lv_bar_set_value(power_on_bar, 100, LV_ANIM_ON);
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