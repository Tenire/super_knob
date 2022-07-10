/*
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-06-18 18:00:49
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-07-10 17:27:38
 */
#pragma once
#include <Arduino.h>


void Task_module(void *pvParameters);
void send_iot_control_msg(int status, char *msg);