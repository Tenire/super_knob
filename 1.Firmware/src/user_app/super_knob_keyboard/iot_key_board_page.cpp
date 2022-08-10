/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 14:15:44
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-08-10 22:24:21
 */
#include "user_app/super_knob_user_api.h"

lv_obj_t * btn_control  = NULL;
lv_obj_t * player_img_center = NULL;
lv_anim_t player_anim;

LV_IMG_DECLARE(music_close_img);       //图片初始化
LV_IMG_DECLARE(music_img);
LV_IMG_DECLARE(music_next_img);        
LV_IMG_DECLARE(music_last_img);
LV_IMG_DECLARE(music_open_img);  
LV_IMG_DECLARE(music_stop_img);     
LV_IMG_DECLARE(volume_img);   
LV_IMG_DECLARE(back_img_16);   

static void exit_btn_event_handler(lv_event_t * e)
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

static void player_btn_control_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    static int play_cnt = 0;
    if(code == LV_EVENT_CLICKED) {
        update_page_status(BUTTON_CLICK);
        keyboard_enable_player();
        if (play_cnt){
            lv_anim_del(player_img_center, NULL);
            lv_img_set_src(super_knob_ui.screen_iot_player_play_img, &music_open_img);
            lv_img_set_src(player_img_center, &music_close_img);
            play_cnt = 0;
        }else{
            play_cnt = 1;
            lv_anim_start(&player_anim);
            lv_img_set_src(super_knob_ui.screen_iot_player_play_img, &music_stop_img);
            lv_img_set_src(player_img_center, &music_img);
        }
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //keyboard_enable_player();
    }
}

static void player_btn_next_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        update_page_status(BUTTON_CLICK);
        keyboard_player_next();
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {

    }
}
static void player_btn_volume_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) {
        update_page_status(CHECKOUT_PAGE);
        set_super_knob_page_status(SUPER_PAGE_BUSY);
        update_motor_config(2);
        setup_scr_screen_volume(&super_knob_ui);
        lv_scr_load_anim(super_knob_ui.screen_iot_volume, LV_SCR_LOAD_ANIM_FADE_ON, 200, 100, true);
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {
        //LV_LOG_USER("Toggled");
    }
}


static void player_btn_last_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if(code == LV_EVENT_CLICKED) {
        update_page_status(BUTTON_CLICK);
        keyboard_player_next();
    }
    else if(code == LV_EVENT_VALUE_CHANGED) {

    }
}

static void anim_y_cb(void * var, int32_t v)
{
    lv_obj_set_y((lv_obj_t*)var, v);
}

static void anim_size_cb(void * var, int32_t v)
{
    lv_obj_set_size((lv_obj_t*)var, v, v);
}


void setup_scr_screen_player(lv_ui *ui)
{
    ui->screen_iot_player = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_iot_player, 80, 160);
    lv_obj_center(ui->screen_iot_player);
    lv_obj_set_scrollbar_mode(ui->screen_iot_player, LV_SCROLLBAR_MODE_OFF); //从不显示滚动条

    static lv_style_t opa_style;
    lv_style_init(&opa_style);
    lv_style_set_radius(&opa_style, 10);
    lv_style_set_bg_opa(&opa_style, 0);

    static lv_style_t style_none_opa_btn;
    lv_style_init(&style_none_opa_btn);
    lv_style_set_bg_opa(&style_none_opa_btn, 0); //设置背景透明

    /*Init the style for the default state*/
    static lv_style_t style;
    lv_style_init(&style);

    lv_style_set_radius(&style, 10);

    lv_style_set_bg_opa(&style, LV_OPA_100);
    lv_style_set_bg_color(&style, lv_color_make(0xaa, 0xaa, 0xf8));//#aaaaf8
    lv_style_set_bg_grad_color(&style, lv_color_make(0xaa, 0xaa, 0xf8));
    lv_style_set_bg_grad_dir(&style, LV_GRAD_DIR_VER);

    lv_style_set_border_opa(&style, LV_OPA_40);
    lv_style_set_border_width(&style, 2);
    lv_style_set_border_color(&style, lv_palette_main(LV_PALETTE_GREY));

    lv_style_set_shadow_width(&style, 8);
    lv_style_set_shadow_color(&style, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_shadow_ofs_y(&style, 8);

    lv_style_set_outline_opa(&style, LV_OPA_COVER);
    lv_style_set_outline_color(&style, lv_palette_main(LV_PALETTE_BLUE));

    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_pad_all(&style, 10);

    /*Init the pressed style*/
    static lv_style_t style_pr;
    lv_style_init(&style_pr);

    /*Add a large outline when pressed*/
    lv_style_set_outline_width(&style_pr, 30);
    lv_style_set_outline_opa(&style_pr, LV_OPA_TRANSP);

    lv_style_set_translate_y(&style_pr, 5);
    lv_style_set_shadow_ofs_y(&style_pr, 3);
    lv_style_set_bg_color(&style_pr, lv_color_make(0xaa, 0xaa, 0xf8));
    lv_style_set_bg_grad_color(&style_pr, lv_color_make(0xaa, 0xaa, 0xf8));

    /*Add a transition to the outline*/
    static lv_style_transition_dsc_t trans;
    static lv_style_prop_t props[] = {LV_STYLE_OUTLINE_WIDTH, LV_STYLE_OUTLINE_OPA, (lv_style_prop_t)0};
    lv_style_transition_dsc_init(&trans, props, lv_anim_path_linear, 300, 0, NULL);

    lv_style_set_transition(&style_pr, &trans);

    lv_obj_t * exit_btn = lv_btn_create(ui->screen_iot_player);
    lv_obj_add_event_cb(exit_btn, exit_btn_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(exit_btn, LV_RADIUS_CIRCLE, 0); //设置为圆形
    lv_obj_set_size(exit_btn, 20, 20);
    lv_obj_t * img_test = lv_img_create(exit_btn);
    lv_img_set_src(img_test, &back_img_16);
    lv_obj_center(img_test);

    lv_obj_add_style(exit_btn, &opa_style, 0);
    lv_obj_align(exit_btn, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_fade_in(exit_btn, 500, 500);

    player_img_center= lv_img_create(ui->screen_iot_player);
    lv_img_set_src(player_img_center, &music_close_img);
    //lv_obj_center(player_img_center);
    lv_obj_align(player_img_center, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_add_anim_down(player_img_center, 500);
    //lv_group_add_obj(super_knob_ui.defult_group, img_1);

    //设计播放音乐的动画

    lv_anim_init(&player_anim);
    lv_anim_set_var(&player_anim, player_img_center);
    lv_anim_set_time(&player_anim, 800);
    lv_anim_set_playback_delay(&player_anim, 100);                   //开始播放动画前的延时
    lv_anim_set_playback_time(&player_anim, 300);
    lv_anim_set_repeat_delay(&player_anim, 300);                     //重复之前的延迟
    lv_anim_set_repeat_count(&player_anim, LV_ANIM_REPEAT_INFINITE); //动画重复
    lv_anim_set_path_cb(&player_anim, lv_anim_path_ease_out);

    // lv_anim_set_exec_cb(&a, anim_size_cb);
    // lv_anim_start(&a);
    lv_anim_set_exec_cb(&player_anim, anim_y_cb);
    lv_anim_set_values(&player_anim, 0, 10);
    //lv_anim_start(&player_anim);

    static lv_style_t style_panel;
    lv_style_init(&style_panel);
    lv_style_set_bg_opa(&style_panel, 0); //设置背景透明
    lv_style_set_border_opa(&style_panel,0);
    lv_obj_t *panel = lv_obj_create(ui->screen_iot_player);
    lv_obj_set_size(panel, 80, 60);
    lv_obj_set_scroll_snap_x(panel, LV_SCROLL_SNAP_CENTER);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_align(panel, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_add_flag(panel, LV_OBJ_FLAG_SCROLL_ONE);
    lv_obj_set_scrollbar_mode(panel, LV_SCROLLBAR_MODE_OFF); //从不显示滚动条
    lv_obj_add_style(panel, &style_panel, 0);

    // //-------------------------button_last----------------------------------//
    lv_obj_t * btn_last = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_last, player_btn_last_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(btn_last, LV_RADIUS_CIRCLE, 0); //设置为圆形
    lv_obj_set_size(btn_last, 45, 45);
    lv_obj_t * btn_last_img = lv_img_create(btn_last);
    lv_img_set_src(btn_last_img, &music_last_img);
    lv_obj_center(btn_last_img);
    lv_obj_add_style(btn_last, &style_none_opa_btn, 0);
    lv_obj_add_style(btn_last, &style_pr, LV_STATE_PRESSED);

    //-------------------------button_control----------------------------------//
    lv_obj_t * btn_control = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_control, player_btn_control_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_style(btn_control, &style, 0);
    lv_obj_add_style(btn_control, &style_pr, LV_STATE_PRESSED);
    lv_obj_set_size(btn_control, 45, 45);
    ui->screen_iot_player_play_img = lv_img_create(btn_control);
    lv_img_set_src(ui->screen_iot_player_play_img, &music_open_img);
    lv_obj_center(ui->screen_iot_player_play_img);

    //-------------------------button_next----------------------------------//
    lv_obj_t * btn_next = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_next, player_btn_next_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_style_radius(btn_next, LV_RADIUS_CIRCLE, 0); //设置为圆形
    lv_obj_set_size(btn_next, 45, 45);
    lv_obj_t * btn_next_img = lv_img_create(btn_next);
    lv_img_set_src(btn_next_img, &music_next_img);
    lv_obj_center(btn_next_img);
    lv_obj_add_style(btn_next, &style_none_opa_btn, 0);
    lv_obj_add_style(btn_next, &style_pr, LV_STATE_PRESSED);

        //-------------------------button_next----------------------------------//
    lv_obj_t * btn_volume = lv_btn_create(panel);
    lv_obj_add_event_cb(btn_volume, player_btn_volume_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_set_size(btn_volume, 48, 48);
    lv_obj_t * btn_volume_img = lv_img_create(btn_volume);
    lv_img_set_src(btn_volume_img, &volume_img);
    lv_obj_center(btn_volume_img);
    lv_obj_add_style(btn_volume, &style_none_opa_btn, 0);
    lv_obj_add_style(btn_volume, &style_pr, LV_STATE_PRESSED);
    lv_obj_add_anim_up(btn_volume,500);

    //lv_obj_scroll_to_view(btn_control, LV_ANIM_OFF);


    //刷新页面调度器
    set_super_knob_page_status(IOT_COMPUTER_PAGE);
}