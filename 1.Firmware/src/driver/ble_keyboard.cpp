/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 13:57:40
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-08-13 09:00:52
 * https://cloud.tencent.com/developer/article/1891221
 * 键值对应表 http://t.zoukankan.com/noticeable-p-14893583.html
 */
#include <main.h>
#ifdef ENABLE_BLE_KEY_BOARD
#include <BleKeyboard.h>
#endif
#include "ble_keyboard.h"

#ifdef ENABLE_BLE_KEY_BOARD
BleKeyboard bleKeyboard("Super Knord pro","Espressif",100);
#endif


void ble_keyboard_init(void)
{
    #ifdef ENABLE_BLE_KEY_BOARD
    bleKeyboard.begin();
    Serial.println("Starting BLE work!");
    #endif

}


bool check_keyboard_connected(void)
{
    #ifdef ENABLE_BLE_KEY_BOARD
    return bleKeyboard.isConnected();
    #else
    return false;
    #endif
}

void keyboard_enable_player(void)
{
    if(!check_keyboard_connected())
    {
        Serial.println("check_keyboard_connected error...");
    }
    
    Serial.println("Sending Play/Pause media key...");
    #ifdef ENABLE_BLE_KEY_BOARD
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    #endif
    return;
}

int keyboard_player_next(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Sending Play/Pause media key...");
    #ifdef ENABLE_BLE_KEY_BOARD
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    #endif
    return 0;
}

int keyboard_player_volume_up(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Sending Play/Pause media key...");
    #ifdef ENABLE_BLE_KEY_BOARD
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
    #endif
    return 0;
}

int keyboard_player_volume_down(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Sending Play/Pause media key...");
    #ifdef ENABLE_BLE_KEY_BOARD
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
    #endif
    return 0;
}


int keyboard_pass_ctrl_c(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Sending ctrl + c");
    #ifdef ENABLE_BLE_KEY_BOARD
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(67);
    delay(50);
    bleKeyboard.releaseAll();
    #endif
    return 0;
}

int keyboard_pass_ctrl_v(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Sending ctrl + v");
    #ifdef ENABLE_BLE_KEY_BOARD
    bleKeyboard.press(KEY_LEFT_CTRL);
    bleKeyboard.press(86);
    delay(50);
    bleKeyboard.releaseAll();
    #endif
    return 0;
}
