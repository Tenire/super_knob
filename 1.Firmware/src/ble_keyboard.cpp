/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-04 13:57:40
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-06 20:36:47
 */
#include <main.h>
#include <BleKeyboard.h>
#include <ble_keyboard.h>
BleKeyboard bleKeyboard("Super Knord pro","Espressif",100);


void ble_keyboard_init(void)
{
    bleKeyboard.begin();
    Serial.println("Starting BLE work!");
}


bool check_keyboard_connected(void)
{
    return bleKeyboard.isConnected();
}

void keyboard_enable_player(void)
{
    if(!check_keyboard_connected())
    {
        Serial.println("check_keyboard_connected error...");
    }
    return;
    
    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
}

int keyboard_player_next(void)
{
    if(!check_keyboard_connected())
    return -1;
    
    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
    return 0;
}