/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 13:57:45
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-07-30 23:59:34
 */
#pragma once

void ble_keyboard_init(void);
bool check_keyboard_connected(void);
void keyboard_enable_player(void);
int keyboard_player_next(void);
int keyboard_player_volume_up(void);
int keyboard_player_volume_down(void);
