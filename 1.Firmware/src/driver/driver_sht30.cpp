/*
 * @Copyright: 
 * @file name: File name
 * @Data: Do not edit
 * @LastEditor: 
 * @LastData: 
 * @Describe: 
 */
#include "driver_sht30.h"
#include <main.h>

#include <Wire.h>
#define Addr_SHT30 0x44
static bool sht30_status = false;



void driver_sht30_init(void)
{
    
    sht30_status = Wire.begin(19, 18);
    if(!sht30_status) {
        Serial.print("sht30 init failed !!!");
        return;
    }
    
    Wire.beginTransmission(Addr_SHT30);
    Wire.write(0x2C);
    Wire.write(0x06);
    Wire.endTransmission();
}

int get_sht30_temperature(float *tem, float *hum)
{
    if(sht30_status == false) {
        Serial.print("please chaeck the sht30 sensor !!!");
        return -1;
    }

    unsigned int data[6]; //存储获取到的六个数据
    Wire.requestFrom(Addr_SHT30, 6);

    if (Wire.available() == 6)
    {
        data[0] = Wire.read();
        data[1] = Wire.read();
        data[2] = Wire.read();
        data[3] = Wire.read();
        data[4] = Wire.read();
        data[5] = Wire.read();
    }

    //然后计算得到的数据，要转化为摄氏度、华氏度、相对湿度
    float cTemp = ((((data[0] * 256.0) + data[1]) * 175) / 65535.0) - 45;
    float humidity = ((((data[3] * 256.0) + data[4]) * 100) / 65535.0);
    *tem = cTemp;
    *hum = humidity;

    Serial.print("相对湿度：");
    Serial.print(humidity);
    Serial.println(" %RH");
    Serial.print("摄氏度温度：");
    Serial.print(cTemp);
    Serial.println(" C");
    return 0;
}