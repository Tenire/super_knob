/*
 * @Descripttion:
 * @version:
 * @Author: congsir
 * @Date: 2022-06-04 14:15:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-08-06 16:04:40
 */
#include "user_app/super_knob_user_api.h"

LV_IMG_DECLARE(light_selt_img); //图片初始化
lv_obj_t * img_center = NULL;
lv_meter_indicator_t * indic1 ;
lv_timer_t *_motor_sta_timer;
char *open_light_belt = "{\"1\":true}";
char *close_light_belt = "{\"1\":false}";

static void set_value(void * indic, int32_t v)
{
    lv_meter_set_indicator_end_value(super_knob_ui.screen_iot_light_belt, (lv_meter_indicator_t *)indic, v);
}

static void motor_status_read(void)
{
    static int now_num = 0;
    static int old_num = 0;
    now_num = get_motor_position();

    if (now_num > old_num)
    {
        old_num = get_motor_position();
        set_value(indic1, 100);
        send_iot_control_msg(0, open_light_belt);
    }
    else if (now_num < old_num)
    {

        old_num = get_motor_position();
        set_value(indic1, 0);
        send_iot_control_msg(0, close_light_belt);
    }
}

void _motor_sta_timeout(lv_timer_t *timer)
{
    /*Use the user_data*/
    // uint32_t *user_data = (uint32_t *)timer->user_data;
    // Serial.print("------------------");
    // Serial.println(*user_data);
    motor_status_read();
}


void ext_iot_light_belt_page(void)
{
    lv_timer_del(_motor_sta_timer);
}

void setup_scr_screen_light_belt(lv_ui *ui)
{

    ui->screen_iot_light_belt = lv_meter_create(NULL);
    lv_obj_center(ui->screen_iot_light_belt);
    lv_obj_set_size(ui->screen_iot_light_belt, 200, 200);

    /*Remove the circle from the middle*/
    lv_obj_remove_style(ui->screen_iot_light_belt, NULL, LV_PART_INDICATOR);

    /*Add a scale first*/
    lv_meter_scale_t * scale = lv_meter_add_scale(ui->screen_iot_light_belt);
    lv_meter_set_scale_ticks(ui->screen_iot_light_belt, scale, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));//设置刻度的属性
    //lv_meter_set_scale_major_ticks(ui->screen_iot_light_belt, scale, 1, 2, 30, lv_color_hex3(0xeee), 15);//设置一些“正常”的刻度并设置它们的属性。
    //lv_meter_set_scale_range(ui->screen_iot_light_belt, scale, 0, 100, 270, 90); //设置缩放的值和角度范围。

    /*Add a three arc indicator*/
    indic1 = lv_meter_add_arc(ui->screen_iot_light_belt, scale, 10, lv_palette_main(LV_PALETTE_BLUE), 0);

    img_center= lv_img_create(ui->screen_iot_light_belt);
    lv_img_set_src(img_center, &light_selt_img);
    lv_obj_center(img_center);
    lv_obj_add_anim_down(img_center, 500);


    //刷新页面调度器
    set_super_knob_page_status(IOT_LIGHT_BELT_PAGE);

    //开启定时器 监听旋钮状态
    static uint32_t user_data = 10;
    _motor_sta_timer = lv_timer_create(_motor_sta_timeout, 100, &user_data);
    lv_timer_set_repeat_count(_motor_sta_timer, -1);

}