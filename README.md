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

Для  проекта не на макетной плате Sunton-3.5inch_ESP32-3248S035, а, например, на обычном модуле ESP32 S3 dev, с любым другим внешним дисплеем  в папке sunton_schemes содержатся принципиальные схемы всех элементов платы. Также если ваш дисплей имеет тач панель отличную от GT911, например XPT2046, в той же папке располагается файл touch.h с настройками для наиболее популярных тач панелей таких как GT911, XPT2046 и FT6X36. Раскоментируйте нужный вам драйвер и закоментируйте ненужные, после чего замените этим файлом одноименный в папке с проектом. И, разумеется, внесите необходимые изменения в TFT_eSPI.h
Дополнительно вам понадобятся следующие компоненты:
- [Чип усилителя аудио сигнала SC8002B](https://aliexpress.ru/item/1005001551394107.html?sku_id=12000016561901500&spm=a2g2w.productlist.search_results.0.4bde4aa6BbrnZl "SC8002B")
- [Слот microSD карты](https://aliexpress.ru/item/1005002327556213.html?sku_id=12000020103778220&spm=a2g2w.productlist.search_results.2.9d444aa6bpLGch "Слот microSD карты") или [внешний модуль](https://aliexpress.ru/item/1005004974908695.html?sku_id=12000031210383763&spm=a2g2w.productlist.search_results.5.50f54aa66AcLgX "внешний модуль")
- [Фоторезистор](https://aliexpress.ru/item/1005005692182923.html?sku_id=12000034025248753&spm=a2g2w.productlist.search_results.10.454c4aa6u2Bq2w "Фоторезистор")
- [RGB светодиод](https://aliexpress.ru/item/32325223634.html?sku_id=56906428722&spm=a2g2w.productlist.search_results.0.599e4aa6YZvfNI "RGB светодиод")
- Несколько резисторов и конденсаторов в соответствии со схемой сборки

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

В проекте используется форк библиотеки ESP32Audio I2S за авторством esphome. Стандартная версия без psram работает не очень плавно. Звук часто заикается. Так что используйте версию по ссылке выше. Также библиотека при остановке воспроизведения не совсем корректно освобождала ресурсы. Из=за чего возникали проблемы при работе устройства. Мною эта билиотека так же была немного модифицирована. В папке library_replacement_files/ESP32-audioI2S-dev содержатся модифицированные файлы библиотеки. Замените ими оригинальные.
Библиотека lvgl также была немного модифицирована. Добавлено несколько новых шрифтов со спецсимволами и русскими буквами. Также добавлен файл lv_conf.h с настройками конкретно под этот проект. Для корректной компиляции проекта добавьте файлы из папки library_replacement_files/lvgl в директорию с библиотекой lvlg сохраняя всю структуру папок.
Также в папке library_replacement_files\TFT_eSPI содержится файл TFT_eSPI.h с настройками дисплея макетной платы Sunton. Скопируйте его в папку библиотеки TFT_eSPI.

### Настройки компилятора
Для более комфортного прослушивания радио без фризов необходимо произвести некоторые настройки компилятора. Для этого необходимо внести изменения в файл C:\Users\ **имя_пользователя** \AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.9\tools\sdk\esp32\sdkconfig.
Найдите и измените следующие строки:

    CONFIG_LWIP_MAX_ACTIVE_TCP=512
    CONFIG_LWIP_MAX_LISTENING_TCP=512
    #CONFIG_LWIP_TCP_HIGH_SPEED_RETRANSMISSION=y
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

## Интерфейс
Интерфейс проекта Мультиинформер разделен на несколько экранов. Перемещение по экранам осуществялеется либо свайпами влево и вправо, либо тапом по имени вкладки в нижней части экрана.
### Главный экран
[![Главный экран](https://github.com/altJSV/multiinformer/blob/main/additional_info/main_screen.png "Главный экран")](https://github.com/altJSV/multiinformer/blob/main/additional_info/main_screen.png "Главный экран")
Является стартовым экраном при загрузке устройства. Отображает календарь с выделением текущей даты, а также знаменательных и праздничных дней. 

Также на экране отображается текущее время, синхронизированное с ntp сервером и данные полученные с подключенного датчика BME680 (температура, влажность, атмосферное давление, качество воздуха). Если датчик не подключен, то отображаются данные о погоде полученные с сайта open weather maps. 
Также в нижней части экрана, если текущий день совпадает с праздничным, появляется информация об этом дне. 
В правой верхней части экрана отображается информация о статусе WiFi и присвоенный устройству IP адрес в локальной сети.

#### Настройка списка радиостанций
Для добавления новых радиостанций создайте на sd карте файл playlist.txt примерно такого вида:

    http://rusradio.hostingradio.ru/rusradio128.mp3*Русское радио
    http://free.radioheart.ru:8000/RH54154*Перец FM
    http://radio-holding.ru:9000/marusya_default*Маруся FM
    http://vladfm.ru:8000/vfm*Владивосток FM
Каждая станция начинается с новой строки.  Формат следующий:
url\_адрес\_потока\*Отображаемое\_имя\_станции
Максимальное колличество радиостанций в плейлисте - 255!
Поддерживаемые форматы воспроизведения MP3, AAC и FLAC
После вставки SD карты и последующей перезагрузки  playlist.txt будет скопирован с SD во внутреннюю память устройства.
Также вы можете скопировать данный файл во внутренню память с помощью Arduino IDE, используя Sketch Data Upload.
Пример готового файла playlist.txt c 76 онлайн-радиостанциями находится в папке \data проекта.
