/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-27 00:22:38
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-06-04 14:57:58
 */
#include "lvgl.h"
#include <stdio.h>
#include "gui_super_knod.h"
#include <Arduino.h>
#include <motor.h>
#include <display.h>

LV_IMG_DECLARE(lamp_img);       //图片初始化
LV_IMG_DECLARE(leds_img);       //图片初始化
LV_IMG_DECLARE(socket_img);       //图片初始化
LV_IMG_DECLARE(computer_img);       //图片初始化
LV_IMG_DECLARE(air_cond_img);       //图片初始化
LV_IMG_DECLARE(sensor_img);       //图片初始化

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

void lv_set_scroll_box(lv_obj_t* background_btn, void* image_src, const char * text_buff)
{
    /* style */
    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_bg_opa(&style_btn, 0); //设置背景透明
    lv_style_set_radius(&style_btn, 15);

    static lv_style_t style_text;
    lv_style_init(&style_text);
    lv_style_set_text_opa(&style_text, 255);
    lv_style_set_text_color(&style_text, lv_color_black());

    //lv_obj_t* normal_obj = lv_btn_create(cont);
    lv_obj_set_width(background_btn, lv_pct(100));
    lv_obj_set_height(background_btn, lv_pct(40));
    lv_obj_add_style(background_btn, &style_btn, LV_PART_MAIN);

    lv_obj_t* line1 = lv_line_create(background_btn);
    static lv_point_t line_points[] = { {70, 5}, {70, 70} };
    lv_line_set_points(line1, line_points, 2);

    lv_obj_t *img1 = lv_img_create(background_btn);
    lv_img_set_src(img1, image_src);
    lv_obj_align(img1, LV_ALIGN_LEFT_MID, 0, 0);

    lv_obj_t* line2 = lv_line_create(background_btn);
    static lv_point_t line_points_2[] = { {80, 40}, {180, 40} };
    lv_line_set_points(line2, line_points_2, 2);

    lv_obj_t* label = lv_label_create(background_btn);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
    lv_obj_set_width(label, 150);
    LV_FONT_DECLARE(lv_font_chinese_source_20); //加载中文字体
    lv_obj_set_style_text_font(label, &lv_font_chinese_source_20, 0);
    lv_label_set_text(label, text_buff);
    lv_obj_add_style(label, &style_text, 0);
    lv_obj_align(label, LV_ALIGN_RIGHT_MID, 50, -5);

}

static void lamp_btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        set_super_knod_page_status(SUPER_PAGE_BUSY);
        setup_scr_screen_pointer(&super_knod_ui);
        lv_scr_load_anim(super_knod_ui.screen_iot_pointer, LV_SCR_LOAD_ANIM_FADE_ON, 500, 100, true);
        update_motor_config(4);
        update_page_status(0);
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //LV_LOG_USER("Toggled");
    }
}

static void sensor_btn_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        set_super_knod_page_status(SUPER_PAGE_BUSY);
        setup_scr_screen_iot_sensor(&super_knod_ui);
        lv_scr_load_anim(super_knod_ui.screen_iot_sensor, LV_SCR_LOAD_ANIM_FADE_ON, 500, 100, true);
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //LV_LOG_USER("Toggled");
    }
}

static void sensor_computer_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        set_super_knod_page_status(SUPER_PAGE_BUSY);
        setup_scr_screen_player(&super_knod_ui);
        lv_scr_load_anim(super_knod_ui.screen_iot_player, LV_SCR_LOAD_ANIM_FADE_ON, 500, 100, true);
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //LV_LOG_USER("Toggled");
    }

}


void setup_scr_screen_iot_main(lv_ui *ui)
{
    /* style */
    // static lv_style_t style_iot_main;
    // lv_style_init(&style_iot_main);
    // lv_style_set_border_side(&style_iot_main,LV_BORDER_SIDE_NONE);

    ui->screen_iot_main = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ui->screen_iot_main, 240, 240);
    lv_obj_center(ui->screen_iot_main);
    lv_obj_set_flex_flow(ui->screen_iot_main, LV_FLEX_FLOW_COLUMN);
    lv_obj_add_event_cb(ui->screen_iot_main, scroll_event_cb, LV_EVENT_SCROLL, NULL); //每次位置变化时触发
    lv_obj_set_style_radius(ui->screen_iot_main, LV_RADIUS_CIRCLE, 0); //设置为圆形
    lv_obj_set_style_clip_corner(ui->screen_iot_main, true, 0);
    lv_obj_set_scroll_dir(ui->screen_iot_main, LV_DIR_VER); //滚动方向为上下滚动
    lv_obj_set_scroll_snap_y(ui->screen_iot_main, LV_SCROLL_SNAP_CENTER); //将子对象与滚动对象的中心对齐
    lv_obj_set_scrollbar_mode(ui->screen_iot_main, LV_SCROLLBAR_MODE_OFF); //从不显示滚动条

    //lv_obj_add_style(ui->screen_iot_main, &style_iot_main, 0);//将样式添加到对象中

    lv_obj_t* lamp_btn = lv_btn_create(ui->screen_iot_main);
    lv_obj_add_event_cb(lamp_btn, lamp_btn_event_handler, LV_EVENT_ALL, NULL);
    lv_set_scroll_box(lamp_btn, (void *)&lamp_img, "台灯");

    lv_obj_t* leds_btn = lv_btn_create(ui->screen_iot_main);
    lv_set_scroll_box(leds_btn, (void *)&leds_img, "灯带");

    lv_obj_t* socket_btn = lv_btn_create(ui->screen_iot_main);
    lv_set_scroll_box(socket_btn, (void *)&socket_img, "插座");

    lv_obj_t* computer_btn = lv_btn_create(ui->screen_iot_main);
    lv_obj_add_event_cb(computer_btn, sensor_computer_event_handler, LV_EVENT_ALL, NULL);
    lv_set_scroll_box(computer_btn, (void *)&computer_img, "电脑");

    lv_obj_t* air_cond_btn = lv_btn_create(ui->screen_iot_main);
    lv_set_scroll_box(air_cond_btn, (void *)&air_cond_img, "空调");

    lv_obj_t* sensor_btn = lv_btn_create(ui->screen_iot_main);
    lv_obj_add_event_cb(sensor_btn, sensor_btn_event_handler, LV_EVENT_ALL, NULL);
    lv_set_scroll_box(sensor_btn, (void *)&sensor_img, "传感");

    /*Update the buttons position manually for first*/
    lv_event_send(ui->screen_iot_main, LV_EVENT_SCROLL, NULL);

    /*Be sure the fist button is in the middle*/
    lv_obj_scroll_to_view(lv_obj_get_child(ui->screen_iot_main, 0), LV_ANIM_OFF);

    //刷新页面调度器
    set_super_knod_page_status(IOT_MAIN_PAGE);
}


