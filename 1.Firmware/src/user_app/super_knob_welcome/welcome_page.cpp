/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-08-06 16:06:10
 */
#include "user_app/super_knob_user_api.h"

LV_IMG_DECLARE(motor_img);      //图片初始化
LV_IMG_DECLARE(instrument_img); //图片初始化
LV_IMG_DECLARE(chip_img);       //图片初始化

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

void welcome_timer(lv_timer_t *timer)
{
    /*Use the user_data*/
    uint32_t *user_data = (uint32_t *)timer->user_data;
    //Serial.println(*user_data);

    if (!anim_timeline)
    {
        anim_timeline_create();
    }

    /*Do something with LVGL*/
    //是否反转动画时间线
    //lv_anim_timeline_set_reverse(anim_timeline, true);
    //启动时间线
    lv_anim_timeline_start(anim_timeline);
}


void setup_scr_screen_welcome(lv_ui *ui)
{
    ui->screen_welcome = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_welcome, 240, 240);
    lv_obj_set_flex_flow(ui->screen_welcome, LV_FLEX_FLOW_ROW); //将子元素 排成一行
    lv_obj_set_flex_align(ui->screen_welcome, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    /* create slider_prg */
    static lv_style_t style_bg;
    static lv_style_t style_indic;
    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_opa(&style_text, 255);
    lv_style_set_text_color(&style_text, lv_color_black());

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

    lv_obj_t* label = lv_label_create(ui->screen_welcome);
    LV_FONT_DECLARE(lv_font_super_knob_30); //加载字体
    lv_obj_set_style_text_font(label, &lv_font_super_knob_30, 0);
    lv_label_set_text(label, "Super Knob!");
    lv_obj_add_style(label, &style_text, 0);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    ui->power_on_bar = lv_bar_create(ui->screen_welcome);
    lv_obj_remove_style_all(ui->power_on_bar);  /*To have a clean start*/
    lv_obj_add_style(ui->power_on_bar, &style_bg, 0);
    lv_obj_add_style(ui->power_on_bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(ui->power_on_bar, 200, 20);
    lv_obj_center(ui->power_on_bar);
    lv_bar_set_value(ui->power_on_bar, 0, LV_ANIM_ON);
    lv_obj_add_flag(ui->power_on_bar, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(ui->power_on_bar, LV_ALIGN_CENTER, 0, 30);

    /* create 3 objects */
    obj1 = lv_img_create(ui->screen_welcome);
    lv_img_set_src(obj1, &motor_img);
    lv_obj_set_size(obj1, obj_width, obj_height);
    lv_obj_add_flag(obj1, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(obj1, LV_ALIGN_CENTER, -60, -40);

    obj2 = lv_img_create(ui->screen_welcome);
    lv_img_set_src(obj2, &instrument_img);
    lv_obj_set_size(obj2, obj_width, obj_height);
    lv_obj_add_flag(obj2, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(obj2, LV_ALIGN_CENTER, 0, -40);

    obj3 = lv_img_create(ui->screen_welcome);
    lv_img_set_src(obj3, &chip_img);
    lv_obj_set_size(obj3, obj_width, obj_height);
    lv_obj_add_flag(obj3, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_align(obj3, LV_ALIGN_CENTER, 60, -40);

    anim_timeline_create();

    //开启定时器。晚一点开启动画
    static uint32_t user_data = 10;
    lv_timer_t *_welcom_timer = lv_timer_create(welcome_timer, 500, &user_data);
    lv_timer_set_repeat_count(_welcom_timer, 1);

    //刷新页面调度器
    set_super_knob_page_status(WELCOME_PAGE);
}