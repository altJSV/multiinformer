# Мультиинформер
Проект универсального мультинформера на базе ESP32
## Основные возможности
- Отображение календаря с актуальными праздниками
- Отображение точного времени, даты с инхронизированных с NTP сервером
- Поддержка модулей BME/BMP280 и BME680 для отображения информации о температуре, влажности, атмосферном давлении и качестве воздуха в помещении
- Беспроводной монитор параметров ПК (температура и загрузка процессора и видеокарты, закруженность оперативной и видеопамяти, данные о сети) через программу Libre Hardware Monitor
- Отображение погодной сводки с сайта Open Weather Maps
- Отображение курсов валют по данным ММВБ
- Онлайн радио плеер

## Используемые компоненты
При создании проекта были использованы следующие компоненты:
- Макетная плата [Sunton-3.5inch_ESP32-3248S035](https://aliexpress.ru/item/1005004632953455.html?spm=a2g2w.orderdetail.0.0.3cb54aa6pUlYxY&sku_id=12000029911293172 "Sunton-3.5inch_ESP32-3248S035")
- Модуль [BME680](https://aliexpress.ru/item/4001113450307.html?spm=a2g2w.orderdetail.0.0.71504aa6NWWCgO&sku_id=10000014487928923 "BME680")
- Ультратонкий [динамик 4 Ом 3 Вт](https://aliexpress.ru/item/4000167008546.html?spm=a2g2w.orderdetail.0.0.61b54aa6HNTohd&sku_id=10000000562177545 "динамик 4 Ом 3 Вт")

## Компиляция проекта
Сборка проекта осуществляется в Arduino IDE версии 2.x
### Используемые библиотеки и ядра
- [Ядро ESP32 2.0.9](https://github.com/espressif/arduino-esp32 "Ядро ESP32 2.0.9")
- [lvgl 8.3.7](https://github.com/lvgl/lvgl/tree/release/v8.3 "lvgl 8.3.7")
- [GyverNTP 1.3.1](https://github.com/GyverLibs/GyverNTP "GyverNTP 1.3.1")
- [GyverTimer 3.2](https://github.com/GyverLibs/GyverTimer "GyverTimer 3.2")
- [ArduinoJson 6.21.2](https://github.com/bblanchon/ArduinoJson "ArduinoJson 6.21.2")
- [Adafruit Unified Sensor 1.1.9](https://github.com/adafruit/Adafruit_Sensor "Adafruit Unified Sensor 1.1.9")
- [Adafruit BME680 Library 2.0.2](https://github.com/adafruit/Adafruit_BME680 "Adafruit BME680 Library 2.0.2")
- [ESP32Audio I2S](https://github.com/esphome/ESP32-audioI2S "ESP32Audio I2S")
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI "TFT_eSPI")

### Настройки компилятора
Для более комфортного прослушивания радио без фризов необходимо произвести некоторые настройки компилятора. Для этого необходимо внести изменения в файл C:\Users\ **имя_пользователя** \AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.9\tools\sdk\esp32\sdkconfig
Найдите и измените следующие строки:
CONFIG_LWIP_MAX_ACTIVE_TCP=512
CONFIG_LWIP_MAX_LISTENING_TCP=512
\#CONFIG_LWIP_TCP_HIGH_SPEED_RETRANSMISSION=y
CONFIG_LWIP_TCP_MAXRTX=12
CONFIG_LWIP_TCP_SYNMAXRTX=12
CONFIG_LWIP_TCP_MSS=1460
CONFIG_LWIP_TCP_TMR_INTERVAL=200
CONFIG_LWIP_TCP_MSL=60000
CONFIG_LWIP_TCP_FIN_WAIT_TIMEOUT=20000
CONFIG_LWIP_TCP_SND_BUF_DEFAULT=8192
CONFIG_LWIP_TCP_WND_DEFAULT=32768
CONFIG_LWIP_TCP_RECVMBOX_SIZE=32

Настройте параметры платы Arduino IDE как на скриншоте ниже:

[![Параметры платы](https://github.com/altJSV/multiinformer/blob/main/additional_info/board_config.png "Параметры платы")](https://github.com/altJSV/multiinformer/blob/main/additional_info/board_config.png "Параметры платы")
