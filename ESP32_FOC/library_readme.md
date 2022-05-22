<!--
 * @Descripttion: 
 * @version: 
 * @Author: congsir
 * @Date: 2022-05-22 23:52:31
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2022-05-22 23:59:05
-->
## 依赖库的配置与注意事项



TFT_eSPI/User_Setup.h
```

#define GC9A01_DRIVER

#define TFT_MOSI 13 // In some display driver board, it might be written as "SDA" and so on.15
#define TFT_SCLK 14 //14
#define TFT_CS   15  // Chip select control pin
#define TFT_DC   27  // Data Command control pin
#define TFT_RST  33  // Reset pin (could connect to Arduino RESET pin)
#define TFT_BL   12  // LED back-light

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

//#define SMOOTH_FONT

#define SPI_FREQUENCY  27000000
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000 

//https://www.bilibili.com/read/cv13270143?spm_id_from=333.999.0.0


```
lvgl/lv_conf.h

```
#define LV_TICK_CUSTOM 1

```


simple FOC 的版本2.2.1→ ESP32版本4.1.0！！！！！！！！