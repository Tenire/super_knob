/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 13:57:40
 * @LastEditors: wenzheng 565402462@qq.com
 * @LastEditTime: 2022-07-30 23:59:23
 */
#include <main.h>
#ifdef ENABLE_BLE_KEY_BOARD
#include <BleKeyboard.h>
#endif
#include <ble_keyboard.h>

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
