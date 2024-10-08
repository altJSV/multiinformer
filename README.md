# Мультиинформер
![Лого](https://github.com/altJSV/multiinformer/blob/main/additional_info/logo.png "Лого")](https://github.com/altJSV/multiinformer/blob/main/additional_info/logo.png "Лого")

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
- [ESPxWebFlMgr](https://github.com/holgerlembke/ESPxWebFlMgr/tree/master "ESPxWebFlMgr")
- [PubSubClient 2.8](https://pubsubclient.knolleary.net/ "PubSubClient")


В проекте используется форк библиотеки ESP32Audio I2S за авторством esphome. Стандартная версия без psram работает не очень плавно. Звук часто заикается. Так что используйте версию по ссылке выше. Также библиотека при остановке воспроизведения не совсем корректно освобождала ресурсы. Из=за чего возникали проблемы при работе устройства. Мною эта билиотека так же была немного модифицирована. В папке library_replacement_files/ESP32-audioI2S-dev содержатся модифицированные файлы библиотеки. Замените ими оригинальные.
Библиотека lvgl также была немного модифицирована. Добавлено несколько новых шрифтов со спецсимволами и русскими буквами. Также добавлен файл lv_conf.h с настройками конкретно под этот проект. Для корректной компиляции проекта добавьте файлы из папки library_replacement_files/lvgl в директорию с библиотекой lvlg сохраняя всю структуру папок.
Также в папке library_replacement_files\TFT_eSPI содержится файл TFT_eSPI.h с настройками дисплея макетной платы Sunton. Скопируйте его в папку библиотеки TFT_eSPI.
В проекте в качестве файлового менеджера используется библиотека ESPxWebFlMgr. По умолчанию она имеет английский интерфейс. Для его частичной руссификации скопируйте в корень файловой системы устройства из папки data проекта следующие файлы: fm.css, fm.js, fm.html. Затем закомментируйте строку fileManagerServerStaticsInternally в файле ESPxWebFlMgr.h библиотеки.

### Ввод персональных данных
Шаблон файла персональных данных находится в example_secrets.h. Переименуйте его в secrets.h и отредактируйте.
Содержимое его выглядит так:
    String pc_server_path="http://ip вашего пк:8085/data.json";//адрес сервера пк
    //Погода
    String api_key = "apikey;  //ваш api ключ погоды
    String qLocation = "Gubkin,ru"; //город для погоды
    //Wifi
    String SSID = "ssid";
    String PASS = "pass";
	//Настройки MQTT
	String mqtt_login="login"; //логин
	String mqtt_pass="pass"; //пароль
	String clientID="Multiinformer"; //ID клиента
	String cmdTopic="multiinformer/cmd/"; //топик управления
	String statusTopic="multiinformer/status/"; //топик статуса
	String tempTopic="multiinformer/temp/"; //топик температуры
	String humidTopic="multiinformer/humid/"; //топик влажности
	String pressureTopic="multiinformer/pressure/"; //топик давления
	String airTopic="multiinformer/air/"; //топик качества воздуха
	String mqtt_server = "192.168.1.1"; //ip или http адрес
	int mqtt_port = 1883; //порт
	
pc_server_path= - ip адрес сервера Libre Hardware Monitor
api_key = - ваш персональный ключ Open Weather Maps
qLocation = -  данные о вашем местоположении для Open Weather Maps
SSID = - название вашей точки доступа WiFi для подключения к интернету
PASS = - пароль вашей точки доступа WiFi для подключения к интернету

Эти данные нужны лишь для прошивки и первичной инициализации устройства. Позже их можно изменить в настройках мультиинформера.

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
[![Отображение погоды на главном экране](https://github.com/altJSV/multiinformer/blob/main/additional_info/main_screen_weather.png "Отображение погоды на главном экране")](https://github.com/altJSV/multiinformer/blob/main/additional_info/main_screen_weather.png "Отображение погоды на главном экране")
Также в нижней части экрана, если текущий день совпадает с праздничным, появляется информация об этом дне. 
В правой верхней части экрана отображается информация о статусе WiFi и присвоенный устройству IP адрес в локальной сети.

### Экран мониторинга параметров ПК
[![Экран мониторинга параметров ПК](https://github.com/altJSV/multiinformer/blob/main/additional_info/hardwaremon.png "Экран мониторинга параметров ПК")](https://github.com/altJSV/multiinformer/blob/main/additional_info/hardwaremon.png "Экран мониторинга параметров ПК")
Служит для мониторинга параметров ПК посредством программы Libre Hardware Monitor.
Отображает следующие параметры
Процессор:
- Температура
- Общий процент загруженности по всем ядрам
- Потребляемая мощность

Видеокарта
- Температура
- Общий процент загруженности
- Скорость работы вентилятора в процентах

Оперативная память
- Общее колличество памяти
- Использованная память
- Оставшаяся память

Видеопамять
- Общее колличество памяти
- Использованная память
- Оставшаяся память

Подключение по локальной сети
- Скорость исходящего потока
- Скорость входящего потока

Работа данного экрана требует предварительной настройки Libre Hardware Monitor и модификации исходников программы в файле hardwaremonitor.ino. Полная инструкция по настройке приведена в [этой статье](https://projectalt.ru/publ/arduino_i_esp/sozdanie_wifi_monitora_parametrov_pk_na_platformakh_esp32_ili_esp8266/3-1-0-55 "этой статье").

### Экран погоды
[![Экран погоды](https://github.com/altJSV/multiinformer/blob/main/additional_info/weather.png "Экран погоды")](https://github.com/altJSV/multiinformer/blob/main/additional_info/weather.png "Экран погоды")
Экран  данных о погоде полученных с сайта Open Weather Maps.
Отображает следующую информацию:
- Город
- Направление и силу ветра
- Температуру на улице, ее фактическое ощущение, максимум и минимум за день
- Относительную влажность воздуха
- Атмосферное давление
- Видимость
- Время рассвета и заката
- Иконку и описание текущей погоды за окном
- Графики изменения атмосферного давления, температуры и влажности за сутки

Для получения данных о погоде вам предварительно нужно получить API ключ с сайта Open Weather Maps [по этой инструкции](https://projectalt.ru/load/proekty_na_arduino_i_esp8266/esp8266/prostaja_meteostancija_na_esp8266/12-1-0-37#id-2 "по этой инструкции").

Далее ввести его либо в файл secrets.h в поле api_key = "", либо воспользоваться экраном настроек **Погода** непосредственно на устройстве. Аналогично укажите название города латинскими буквами в формате "City,code". Например "Gubkin,ru".

### Экран курса валют 
[![Экран курсов валют](https://github.com/altJSV/multiinformer/blob/main/additional_info/currency.png "Экран курсов валют")](https://github.com/altJSV/multiinformer/blob/main/additional_info/currency.png "Экран курсов валют")
Отображает актуальную информацию различных курсов валют по отношению к рублю. Данный экран настроек не имеет. Пролистывание списка осуществляется свайпами вверх и вниз.

### Экран онлайн радио проигрывателя
[![Экран радиоплеера](https://github.com/altJSV/multiinformer/blob/main/additional_info/radio_screen.png "Экран радиоплеера")](https://github.com/altJSV/multiinformer/blob/main/additional_info/radio_screen.png "Экран радиоплеера")
Позволяет проигрывать онлайн аудиопоток различных интернет-радиостанций.
Остановка и запуск воспроизведения, а также переключение треков осущесвляется с помощью соответствующих кнопок. 
Изменение громкости осуществляется тапом по слайдеру, либо по нажатию соответствующих кнопок.
Имеется простенький визуализатор аудиопотока, но на данных момент он не привязан непосредственно к самому аудио.
Ниже визуализатора располагается информация о воспроизводимом в данный момент треке, если радиостанция предоставляет данную информацию.
Сохранение последней воспроиведенной радиостанции и установленного уровня громкости осуществляется автоматически после нажатия кнопки остановки воспроизведения.
Реализовано обнаружение низкой скорости потока. При частых фризах и подвисаниях устройство автоматически переподключается к точке доступа WiFi.
Вызов списка воспроизведения, имеющихся в памяти устройства радиостанций, осуществляется по кнопке в правом нижнем углу экрана.
Первый запуск плейлиста занимает некоторое время из-за индексации списка станций. Далее список окрывается мгновенно.
[![Плейлист](https://github.com/altJSV/multiinformer/blob/main/additional_info/playlist.png "Плейлист")](https://github.com/altJSV/multiinformer/blob/main/additional_info/playlist.png "Плейлист")
Переключение воспроизводимой радиостанции осуществляется тапом по ее названию.
Пролистывание списка осуществляется свайпами вверх и вниз.
Имеется возможность отредактировать список воспроизвдения прямо на устройстве, перейдя в режим редактирования вокне списка воспроизведения, нажав на кнопку в левом верхнем углу окна. Далее тапом выбирается изменяемый параметр и с помощью экранной клавиатуры редактируется. Сохранение правок осуществляется автоматически после закрытия окна списка воспроизведения.

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

##### Добавление списка радиостанций через Файловый менеджер
Также playlist.txt можно переместить и редактировать прямо на устройстве с помощью встроенного файлового менеджера. Для этого введите в адресной строке браузера компьютера или смартфона IP адрес устройства и с помощью открывшегося файлового менеджера переместите  playlist.txt во внутреннюю память устройства, либо отредактируйте его прямо в браузере.

### Экран настроек
На данном экране представлены различные настройки устройства разделенные на 6 категорий:
- Основные
- Монитор ПК
- Погода
- Датчик BME
- SD карта
Переключение  категорий настроек осуществляется свайпами вверх и вниз, а также тапами по имени категории в левой части экрана. Сохранение настроек производится автоматически при закрытии панели настроек.

#### Основные
[![Основные настройки](https://github.com/altJSV/multiinformer/blob/main/additional_info/main_settings.png "Основные настройки")](https://github.com/altJSV/multiinformer/blob/main/additional_info/main_settings.png "Основные настройки")
Здесь производится настройка яркости подсветки экрана, а также включение и отключение автоматической регулировки яркости подсветки в зависимости от внешнего освещения.
Также здесь можно изменть время работы RGB индикатора раздичных состояний устройства, например для его отключения в ночное время, а также отключить его совсем. 
Также на данном экране настроек можно установить часовой пояс для отображаемого времени на экране.

#### WiFi
[![Настройки WiFi](https://github.com/altJSV/multiinformer/blob/main/additional_info/wifi_settings1.png "Настройки WiFi")](https://github.com/altJSV/multiinformer/blob/main/additional_info/wifi_settings1.png "Настройки WiFi")
На данном экране вы можете ввести учетные данные для подключения по WiFi к вашей точке доступа, а также просмотреть информацию об активном WiFi подкючении.
[![Информация о WiFi](https://github.com/altJSV/multiinformer/blob/main/additional_info/wif_settings2.png "Информация о WiFi")](https://github.com/altJSV/multiinformer/blob/main/additional_info/wif_settings2.png "Информация о WiFi")

#### Монитор ПК
[![Монитор ПК](https://github.com/altJSV/multiinformer/blob/main/additional_info/pc_settings.png "Монитор ПК")](https://github.com/altJSV/multiinformer/blob/main/additional_info/pc_settings.png "Монитор ПК")
На данном экране вы можете указать адрес веб сервера Libre Hardware Monitor для чтения информации о параметрах ПК, а также изменить интервал опроса датчиков.

#### Погода
[![Настройки погоды](https://github.com/altJSV/multiinformer/blob/main/additional_info/weather_settings.png "Настройки погоды")](https://github.com/altJSV/multiinformer/blob/main/additional_info/weather_settings.png "Настройки погоды")
На данном экране вы может настроить параметры подключения к погодному серверу Open Weather Maps. С помощью экранной клавиатуры вы можете ввести ваш API ключ для доступа к сервису, а также город и код страны для которого будт отображаться погода.
С помощью слайдера вы можете установить временной интервал обновления данных о погоде.

#### Датчик BME
[![Настройки BME](https://github.com/altJSV/multiinformer/blob/main/additional_info/bme_settings.png "Настройки BME")](https://github.com/altJSV/multiinformer/blob/main/additional_info/bme_settings.png "Настройки BME")
На данной вкладке настроек вы можете включить использование датчика BME680, для отображения различных параметров микроклимата в помещении на главном экране. Также вы можете с помощью слайдера изменить интервал опроса датчика.

#### SD карта
[![Настройки SD](https://github.com/altJSV/multiinformer/blob/main/additional_info/sd_settings.png "Настройки SD")](https://github.com/altJSV/multiinformer/blob/main/additional_info/sd_settings.png "Настройки SD")
На этом экране вы можете импортировать/экспортировать файл настроек устройства, а также плейлист радиостанций с (на) SD карты(у) во(из) внутреннюю(ей) память(и) мультиинформера.

## Дополнительная информация
Ниже приведена информация по дополнительным функциям устройства.
### MQTT
Устройство через определенные промежутки времени отправляет данные, полученные с датчика BME680 в различные MQTT топики. Ниже список по умолчанию. Названия топиков могут быть изменены в конфигурационном файле secrets.h.
* "multiinformer/temp/"- топик температуры
* "multiinformer/humid/" -топик влажности
* "multiinformer/pressure/" - топик атмосферного давления
* "multiinformer/air/" - топик уровня качества воздуха
