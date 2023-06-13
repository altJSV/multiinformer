
//Подключаем библиотеки
  #include <lvgl.h> //библиотека пользовательского интерфейса
  #include "secrets.h"
  #include <FS.h> //Работа с файловой системой
  #include <SPIFFS.h> //файловая система esp32
  #include "SD.h"//работа с sd картой
  #include <SPI.h>//интерфейс взаимодействия с sd картой
  #include <Wire.h> //i2c инетрфейс для тач панели и датчика bme
  #include "touch.h"//функции для работы с тачскрином
  #include <GyverNTP.h>//синхронизация времени по интернету
  #include <GyverTimer.h>//подключение различных таймеров
  #include <ArduinoJson.h>//библиотека для работы с файлами конфигурации
  #include <WiFi.h>//wifi функции
  #include <WiFiClient.h>//для связи по протоколу https
  #include <HTTPClient.h>//отправка http запросов
  #include <Adafruit_Sensor.h>//универсальная библиотека для работы с различными сенсорами
  #include "Adafruit_BME680.h"//библиотека для работы с датчиком BME680
  #include "Audio.h" //ESP32 Audio I2S от ESPHome
  #include <TFT_eSPI.h> //Работа с дисплеем

//Определяем различные параметры устройств
  //Пины ESP32 подключенные к SD карте
  #define SD_SCK 18
  #define SD_MISO 19
  #define SD_MOSI 23
  #define SD_CS 5
  
  //Пины перефирии
  #define TFT_BACKLIGHT 27 //пин подсветки экрана
  #define PHOTO_PIN 34 //пин фоторезистора
  
  //Пины RGB светодиода
  #define RED_PIN 4 //красный
  #define GREEN_PIN 16 //зеленый
  #define BLUE_PIN 17 //синий

  //Флаг форматирования файловой системы при ошибке инициализации SPIFFS
  #define FORMAT_SPIFFS_IF_FAILED true

  //Дополнительные символы в шрифте
  #define LV_SYMBOL_SUNRISE "\xEF\x82\xB3" //Восход
  #define LV_SYMBOL_SUNSET "\xEF\x82\xB4" //Закат
  #define LV_SYMBOL_HUMIDITY "\xEF\x82\xB5" //Влажность
  #define LV_SYMBOL_PRESSURE "\xEF\x82\xB6"//Атм. давление
  #define LV_SYMBOL_CPU "\xEF\x82\xB7" //процессор
  #define LV_SYMBOL_GPU "\xEF\x82\xB8" //видеокарта
  #define LV_SYMBOL_FAN "\xEF\x82\xB9" //вентилятор
  #define LV_SYMBOL_AIR "\xEF\x82\xBA" //качество воздуха
  #define LV_SYMBOL_TEMP "\xEF\x82\xBB" //температура

//Объявление графических ресурсов используемых в проекте
  LV_IMG_DECLARE(clouds100);//облака для экрана погоды
  LV_IMG_DECLARE(drizzle100);//слякоть
  LV_IMG_DECLARE(mist100);//Туман
  LV_IMG_DECLARE(rain100);//Дождь
  LV_IMG_DECLARE(snow100);//Снег
  LV_IMG_DECLARE(storm100);//Гроза
  LV_IMG_DECLARE(sun100);//Солнце
  LV_IMG_DECLARE(play); //кнопка play
  LV_IMG_DECLARE(stop); //кнопка stop
  LV_IMG_DECLARE(prevst); //кнопка prev
  LV_IMG_DECLARE(nextst); //кнопка next
  LV_IMG_DECLARE(volmin); //кнопка vol-
  LV_IMG_DECLARE(volmax); //кнопка vol+
  LV_IMG_DECLARE(playlist); //редактор плейлиста

//Различные глобальные переменные
//Режим радио
  int sn = 1, vol = 5; //sn номер станции, vol громкость макс 20, un колличество станций в массива
  char URL[100], sta[50]; //URL и название станции
  uint8_t un=0; //Колличество треков в плейлисте
  String url="http://vladfm.ru:8000/vfm*Владивосток FM";
  bool rp = false; //проигрывается ли радио в данный момент
  bool refresh_playlist = true; //Признак обновления плейлиста
  bool playlist_edit=false; //плейлист находится в режиме редактирования
  String tempout="";
  //tempout.reserve(160);
  u_int16_t param; //параметр передаваемой редактируемой строки и столбца плейлиста
//Календарь
  uint8_t lastday=1; //здесь хранится значение предыдущего дня при смене даты. Необходимо для обновления массива дат праздников календаря

//Использование различных компонентов информера
  bool photosensor = false; //использовать фоторезистор
  bool usesensor = true; //Использовать датчик температуры
  //uint8_t sensortype = 1; //тип датчика 0 - bmp280, 1 - bme280, 2 - bme680, 
  bool ledindicator = true; //Включение или отключение rgb светодиода 

//Переменные конфигурации дисплея
  static const uint16_t screenWidth  = 480; //ширина экрана
  static const uint16_t screenHeight = 320; //высота экрана
  //Подсветка дисплея
  uint8_t bright_level=250; //яркость подсветки экрана


  
//Значения различных таймеров
    uint32_t refpcinterval=3000;//Обновление парметров ПК 3 секунды
    uint32_t refweatherinterval=300000;//Погода 5 минут
    uint32_t refsensorinterval=5000;//получение данных с сенсора 5 секунд
//Предварительная инициализация json для монитора ПК
//StaticJsonDocument<8000> hwm;  
//служебные переменные для анимаций
    u_int8_t prev_cpu_usage=0;//предыдущее значение загрузки проца
    u_int8_t prev_gpu_usage=0;//предыдущее значение загрузки видео
    u_int8_t prev_cpu_temp=0;//предыдущее значение температуры проца
    u_int8_t prev_gpu_temp=0;//предыдущее значение температуры видео
    u_int8_t prev_cpu_fan=0;//предыдущее значение температуры проца
    u_int8_t prev_gpu_fan=0;//предыдущее значение температуры видео
    u_int8_t prev_ram=0;//предыдущее значение загрузки ОЗУ
    u_int32_t prev_gpuram=0;//предыдущее значение загрузки видеопамяти

  
  //Переменные для отслеживание изменений в настройках и вызов функции сохранения
  bool saveconf=false; //флаг измения настроек
  uint8_t prev_tab=0; //предыдущая активная вкладка
  
  int8_t gmt=3; //часовой пояс
  char * ntpserver="pool.ntp.org";

  const char *filename = "/config.txt";  // имя файла конфигурации


//Создание указателей для LVGL
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t buf[screenWidth * screenHeight / 8];
    static lv_obj_t * wifistatus; //статус wifi
    static lv_obj_t * ui_status_clock; //Часы в статус баре
    static lv_obj_t * tabview; //Вкладки
  
    //Экраны
    //1 экран
    static lv_obj_t * statusbox;//служебные сообщения на 1 вкладке
    static lv_obj_t * displayclock;//часы
    static lv_obj_t * calendar;//календарь
    static lv_obj_t * ui_weatherimage_main;//Изображение погоды
    static lv_obj_t * roomtemp;//температура с датчика
    static lv_obj_t * roompress;//давление с датчика
    static lv_obj_t * roomhumid;//влажность с датчика
    static lv_obj_t * roomair;// показания датчика качества воздуха
    static lv_obj_t * roomair_bar;//индикатор качества воздуха
    static lv_obj_t * roomair_bar_label;//баллы качества воздуха
    //4 экран курсы валют
    static lv_obj_t * cur_table;//таблица
    lv_obj_t  * ui_cur_title; //Заголовок таблицы
    //2 экран. Монитор ПК
    static lv_obj_t * cpumeter; //загрузка и теипература процессора
    static lv_meter_indicator_t * cpu_indic;//полоса загрузки процессора
    static lv_meter_indicator_t * cpufan_indic;//вентилятор процессора
    static lv_meter_indicator_t * cputemp_indic;//полоса температуры процессора
    static lv_obj_t * gpumeter;//загрузка и температура видеокарты
    static lv_meter_indicator_t * gpu_indic;//загрузка видеокарты
    static lv_meter_indicator_t  *gputemp_indic;//температура видеокарты
    static lv_meter_indicator_t * gpufan_indic;//вентилятор видеокарты
    static lv_obj_t * rambar;//полоса оперативной памяти
    static lv_obj_t * gpurambar;//полоса памяти видеокарты
    static lv_obj_t * ui_upload_label;//передача по сети
    static lv_obj_t * ui_download_label;//прием по сети
    //Анимации изменения длины полос
    static lv_anim_t cpu;
    static lv_anim_t gpu;
    static lv_anim_t ramcpu;
    static lv_anim_t ramgpu;
    //Различные надписи
    static lv_obj_t * cpu_load_label;
    static lv_obj_t * cpu_temp_label;
    static lv_obj_t * cpu_fan_label;
    static lv_obj_t * gpu_load_label;
    static lv_obj_t * gpu_temp_label;
    static lv_obj_t * gpu_fan_label;
    static lv_obj_t * ram_val_label;
    static lv_obj_t * gpuram_val_label;
    //3 экран. Объявление переменных экрана погоды
    static lv_obj_t * windmeter; //Отображение направления ветра
    static lv_obj_t * ui_windspeed;// надпись для скорости ветра
    static lv_obj_t * ui_windgust;// надпись для скорости ветра
    static lv_meter_indicator_t * wind_indic;//индикатор направления ветра
    //Различные надписи для экрана погоды
    static lv_obj_t * weathercity;//Название города
    static lv_obj_t *ui_weathertemp;//температура
    static lv_obj_t *ui_humid;//влажность
    static lv_obj_t *ui_feeltemp;//температура ощущется как
    static lv_obj_t *ui_avgtemp;//максимальная и минимальная температуры
    static lv_obj_t *ui_weatherdesc;//описание погодных условий
    static lv_obj_t *ui_pressure;//атмосферное давление
    static lv_obj_t *ui_visiblity;//видимость
    static lv_obj_t *ui_sunset;//закат
    static lv_obj_t *ui_sunrise;//рассвет
    static lv_obj_t *ui_weatherimage;//картинка с погодными условиями
    //графики
    static lv_obj_t *ui_pressurechart;//график давления
    static lv_obj_t *ui_tempchart;//график температуры
    static lv_obj_t *ui_humidchart;//график влажности
    //массивы
    static lv_chart_series_t * ui_prser; //массив координат графика давления
    static lv_chart_series_t * ui_tempser; //массив координат графика температуры
    static lv_chart_series_t * ui_huser; //массив координат графика влажности
    //Экран радио
    static lv_obj_t * radio_volumembar; //полоса громкости
    static lv_obj_t * radio_station_num_label; //номер станции
    static lv_obj_t * radio_playing_value_label; //текущий трек
    static lv_obj_t  * radio_playing_label; //название трека
    static lv_obj_t  * radio_visualiser; //визуализация трека
    static lv_chart_series_t  * radio_visualiser_series; //массив данных для визуализации
    lv_obj_t * playlistwin; //редактор плейлиста
    lv_obj_t * playlist_table;//таблица плейлиста

    //7 экран настройки
    static lv_obj_t * slider_label;
    static lv_obj_t * set_tabview; //Вкладки настроек
    static lv_obj_t * kb; //клавиатура
    static lv_obj_t * wt_ta; //поле ввода API погоды
    static lv_obj_t * pc_ta; //поле ввода адреса пк
    static lv_obj_t * ya_ta; //поле ввода API яндекса
    static lv_obj_t * wtl_ta; //поле ввода местоположения погоды
    static lv_obj_t * yandex_int_slider_label;
    static lv_obj_t * weather_int_slider_label;
    static lv_obj_t * pc_int_slider_label;
    static lv_obj_t * bme_int_slider_label;
    static lv_obj_t * gmt_slider_label;
    static lv_obj_t * wifitable;
    static lv_obj_t * wifipass_ta;
    static lv_obj_t * wifissid_ta;




//Инициализация аудио библиотеки
  SPIClass SDSPI(VSPI);
  Audio audio(true, I2S_DAC_CHANNEL_LEFT_EN);
//Инициализация BME датчика
  Adafruit_BME680 bme;
//Инициализация Wifi Manager  
  //WiFiManager wm;
//Инициализации библиотек и переферии
  //инициализация библиотеки TFT_eSPI
  TFT_eSPI tft = TFT_eSPI();  
    
  //NTP инициализация
  GyverNTP ntp(3); //часовой пояс GMT+3

  //Таймеры
  GTimer reftime(MS);//часы
  GTimer reflvgl(MS); //обновление экранов LVGL 
  GTimer refpc(MS);//обновление параметров ПК
  GTimer refweather(MS);//обновление погоды
  GTimer refsensor(MS);//обновление внешнего датчика температуры
  GTimer refbright(MS);//обновление датчика яркости света
  GTimer refvisualiser(MS);//обновление массива визуализации



//Служебные функции
//Здесь напишем функцию для вывода содержимого буфера на экран
  void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
  {
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  tft.startWrite();
  tft.setAddrWindow( area->x1, area->y1, w, h );
  tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
  tft.endWrite();

  lv_disp_flush_ready( disp );
  }

// Вычисление координат касания
  void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
  {
  if (touch_has_signal()) //если есть касание
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PR; //сообщаем библиотеке, что есть касание  и удерживается

      //Отправка координат
      data->point.x = touch_last_x; //координата касания по X
      data->point.y = touch_last_y; //координата касания по Y
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL; //сообщаем библиотеке, что касания больше нет
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
  }

//Функция обработчик изменения активной вкладки
  static void change_tab_event(lv_event_t * e)
  {
    uint16_t acttab=lv_tabview_get_tab_act(tabview);
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        
        if (acttab!=5 && saveconf==true)
          {
            saveconf=false;
            saveConfiguration(filename);
          }
        if (acttab==0) {
           lv_obj_add_flag(ui_status_clock, LV_OBJ_FLAG_HIDDEN);
          }
          else
          {
           lv_obj_clear_flag(ui_status_clock, LV_OBJ_FLAG_HIDDEN); 
          }
        if (acttab==3) {
           cur_check();
          }
    }
  }
//Функция вызываемая при изменении активной вкладки настроек
  static void change_settings_tab_event(lv_event_t * e)
  {
    uint16_t acttab=lv_tabview_get_tab_act(set_tabview);
    if(lv_event_get_code(e) == LV_EVENT_VALUE_CHANGED) {
        //Настройки Wifi
        if (acttab==1) 
        {
         lv_table_set_cell_value_fmt(wifitable,1,1,"%s",WiFi.status() == WL_CONNECTED ? "Подключен" : "Не подключен");
         if (WiFi.status() == WL_CONNECTED)
         {
            lv_table_set_cell_value_fmt(wifitable,2,1,"%s",WiFi.SSID().c_str());
            lv_table_set_cell_value_fmt(wifitable,3,1,"%s",WiFi.localIP().toString().c_str());
            uint8_t macAddr[6];
            WiFi.macAddress(macAddr);
            lv_table_set_cell_value_fmt(wifitable,4,1,"%02x:%02x:%02x:%02x:%02x:%02x",macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
            lv_table_set_cell_value_fmt(wifitable,5,1,"%s",WiFi.getHostname());
            lv_table_set_cell_value_fmt(wifitable,6,1,"%d дБм",WiFi.RSSI());
            lv_table_set_cell_value_fmt(wifitable,7,1,"%s",WiFi.gatewayIP().toString().c_str());
            
         } 
        }
        //Настройки ПК
        if (acttab==2) {
            lv_keyboard_set_textarea(kb, pc_ta);
          }
        //Настройки погоды  
        if (acttab==3) {
            lv_keyboard_set_textarea(kb, wt_ta);
          }
    }
  }
//Универсальная функция обработки ввода текста в text_area
  static void ta_event_cb(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * ta = lv_event_get_target(e);
    lv_obj_t * tv_but = lv_tabview_get_tab_btns(tabview);
    if(code == LV_EVENT_FOCUSED) {
        if(lv_indev_get_type(lv_indev_get_act()) != LV_INDEV_TYPE_KEYPAD) {
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_set_style_max_height(kb, LV_HOR_RES * 2 / 3, 0);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(tv_but, LV_OBJ_FLAG_HIDDEN);
            lv_obj_scroll_to_view_recursive(ta, LV_ANIM_OFF);
        }
    }
    else if(code == LV_EVENT_DEFOCUSED) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(tv_but, LV_OBJ_FLAG_HIDDEN);
        lv_indev_reset(NULL, ta);

    }
    else if(code == LV_EVENT_READY || code == LV_EVENT_CANCEL) {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(tv_but, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_state(ta, LV_STATE_FOCUSED);
        lv_indev_reset(NULL, ta);   /*To forget the last clicked object to make it focusable again*/
    }
  }
//Обработка введенного текста в поле API погоды
  static void wt_ta_event_cb(lv_event_t * e)
  {
  api_key=lv_textarea_get_text(wt_ta);
  saveconf=true;
  }  
//Обработка введенного текста в поле города для погоды
  static void wtl_ta_event_cb(lv_event_t * e)
  {
  qLocation=lv_textarea_get_text(wtl_ta);
  saveconf=true;
  }
//Обработка введенного текста в поле адреса сервера пк информера
  static void pc_ta_event_cb(lv_event_t * e)
  {
  pc_server_path=lv_textarea_get_text(pc_ta);
  saveconf=true;
  }
//Обработка введенного текста в поле ssid wifi
  static void wifissid_ta_event_cb(lv_event_t * e)
  {
  SSID=lv_textarea_get_text(wifissid_ta);
  saveconf=true;
  } 
//Обработка введенного текста в поле pass wifi
  static void wifipass_ta_event_cb(lv_event_t * e)
  {
    PASS=lv_textarea_get_text(wifipass_ta);
    saveconf=true;
  }


//Изменение параметра плейлиста
  static void radio_ta_event_cb(lv_event_t * e)
  {
  lv_obj_t * textarea = lv_event_get_target(e);
  tempout="";
  uint16_t row=param/100;
  Serial.println(row);
  uint16_t col=param%100;
  Serial.println(col);
  lv_table_set_cell_value(playlist_table, row, col, lv_textarea_get_text(textarea));
  tempout+=String(lv_table_get_cell_value(playlist_table,row,1));
  tempout+="*";
  tempout+=String(lv_table_get_cell_value(playlist_table,row,0));
  url=tempout;
  //strcpy(*url[row-1],out);
  Serial.println(url[row-1]); 
  lv_obj_del(textarea);
  
  } 

//Обработка изменения значения слайдера яркости экрана
  static void slider_brightness_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(slider_label, "%d%", (int)lv_slider_get_value(slider));
    lv_obj_align_to(slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    bright_level=(int)lv_slider_get_value(slider);
    ledcWrite(0, bright_level);
    saveconf=true;
  }
//Обработка изменения значения слайдера часового пояса
  static void slider_gmt_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(gmt_slider_label, "GMT: %d%", (int8_t)lv_slider_get_value(slider));
    lv_obj_align_to(gmt_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    gmt=(int8_t)lv_slider_get_value(slider);
    ntp.setGMT(gmt);
    saveconf=true;
  }
//Обработка выбора часового пояса
  static void ui_dd_ntp_server_event(lv_event_t * e)
  {
    lv_obj_t * obj = lv_event_get_target(e);
    
        char buf[32];
        lv_dropdown_get_selected_str(obj, buf, 0);
        ntpserver=&buf[0];
        ntp.setHost(ntpserver);
        saveconf=true;
    
  }   
//Обработка изменения значения слайдера интервала обновления пк информера
  static void slider_pcint_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(pc_int_slider_label, "%d%", (int)lv_slider_get_value(slider));
    lv_obj_align_to(pc_int_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    refpcinterval=(int)lv_slider_get_value(slider)*1000;
    refpc.setInterval(refpcinterval);
    saveconf=true;
  }
//Обработка изменения значения слайдера интервала обновления датчика bme
  static void slider_bmeint_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(bme_int_slider_label, "%d%", (int)lv_slider_get_value(slider));
    lv_obj_align_to(bme_int_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    refsensorinterval=(int)lv_slider_get_value(slider)*1000;
    saveconf=true;
  }
//Обработка изменения значения слайдера интервала обновления информера погоды
  static void slider_weatherint_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(weather_int_slider_label, "%d%", (int)lv_slider_get_value(slider));
    lv_obj_align_to(weather_int_slider_label, slider, LV_ALIGN_CENTER, 0, 0);
    refweatherinterval=(int)lv_slider_get_value(slider)*1000;
    refweather.setInterval(refweatherinterval);
    saveconf=true;
  }


// Выключатель BME 
  static void bme_switch_event(lv_event_t * e)
  {
  lv_obj_t * obj = lv_event_get_target(e);
  //Меняем значение флага включения сенсора BME и изменяем положение объектов для отображения погоды или показаний датчиков
  if (lv_obj_has_state(obj, LV_STATE_CHECKED))
    {
      //показываем данные сенсора
      usesensor=true;
      refsensor.setInterval(refsensorinterval);
      lv_obj_add_flag(ui_weatherimage_main, LV_OBJ_FLAG_HIDDEN);
      lv_obj_align(roomtemp, LV_ALIGN_TOP_LEFT, 210, 100); //положение на экране
      lv_obj_align(roomhumid, LV_ALIGN_TOP_RIGHT, 0, 100); //положение на экране
      lv_obj_align(roompress, LV_ALIGN_TOP_LEFT, 210, 130); //положение на экране
      lv_obj_clear_flag(roomair, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(roomair_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_clear_flag(roomair_bar_label, LV_OBJ_FLAG_HIDDEN);

    }
    else
    {
      usesensor=false;
      refsensor.stop();
      //getweather();
      //показываем значение погоды OWM
      lv_obj_clear_flag(ui_weatherimage_main, LV_OBJ_FLAG_HIDDEN);
      lv_obj_align_to(roomtemp,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_TOP, 5, 0); //положение на экране
      lv_obj_align_to(roomhumid,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_MID, 5, 0); //положение на экране
      lv_obj_align_to(roompress,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 0); //положение на экране
      lv_obj_add_flag(roomair, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(roomair_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(roomair_bar_label, LV_OBJ_FLAG_HIDDEN);

    }
    saveconf=true;
  }


//включение выключение автояркости
  static void autobright_switch_event(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)) 
        {
          photosensor=true;
        }
        else
        {
          photosensor=false;
        }
    }
  saveconf=true;  
  }
//включение и выключение led индикатора
  static void rgb_indic_switch_event(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if (lv_obj_has_state(obj, LV_STATE_CHECKED)) 
        {
          ledindicator=true;
        }
        else
        {
          ledindicator=false;
        }
    }
  saveconf=true;  
  }  

//Изменение цветого оформления таблицы вылют
  static void draw_table_part_event_cb(lv_event_t * e)
  {
    lv_obj_t * obj = lv_event_get_target(e);
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    /*Отрисовка ячеек*/
    if(dsc->part == LV_PART_ITEMS) {
        uint32_t row = dsc->id /  lv_table_get_col_cnt(obj);
        uint32_t col = dsc->id - row * lv_table_get_col_cnt(obj);

        /*Закрашиваем строку заголовка синим*/
        if(row == 0) {
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE), dsc->rect_dsc->bg_color, LV_OPA_20);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        /*Выравнивание первой колонки полевому краю*/
        else if(col == 0) {
            dsc->label_dsc->align = LV_TEXT_ALIGN_LEFT;
        }

        /*Закрашиваем каждую вторую строку серым*/
        if((row != 0 && row % 2) == 0) {
            dsc->rect_dsc->bg_color = lv_color_mix(lv_palette_main(LV_PALETTE_GREY), dsc->rect_dsc->bg_color, LV_OPA_10);
            dsc->rect_dsc->bg_opa = LV_OPA_COVER;
        }
        }
    
  }
/*функция обработчик касания точек на графике
  static void show_cash_value_cb(lv_event_t * e)
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * chart = lv_event_get_target(e);

    if(code == LV_EVENT_DRAW_POST_END) {
        int32_t id = lv_chart_get_pressed_point(chart);
        if(id == LV_CHART_POINT_NONE) return;

        LV_LOG_USER("Selected point %d", (int)id);

        lv_chart_series_t * ser = lv_chart_get_series_next(chart, NULL);
        while(ser) {
            lv_point_t p;
            lv_chart_get_point_pos_by_id(chart, ser, id, &p);

            lv_coord_t * y_array = lv_chart_get_y_array(chart, ser);
            lv_coord_t value = y_array[id];

            char buf[16];
            lv_snprintf(buf, sizeof(buf), LV_SYMBOL_DUMMY"%d", value);

            lv_draw_rect_dsc_t draw_rect_dsc;
            lv_draw_rect_dsc_init(&draw_rect_dsc);
            draw_rect_dsc.bg_color = lv_color_black();
            draw_rect_dsc.bg_opa = LV_OPA_50;
            draw_rect_dsc.radius = 3;
            draw_rect_dsc.bg_img_src = buf;
            draw_rect_dsc.bg_img_recolor = lv_color_white();

            lv_area_t a;
            a.x1 = chart->coords.x1 + p.x - 20;
            a.x2 = chart->coords.x1 + p.x + 20;
            a.y1 = chart->coords.y1 + p.y - 30;
            a.y2 = chart->coords.y1 + p.y - 10;

            lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);
            lv_draw_rect(draw_ctx, &draw_rect_dsc, &a);

            ser = lv_chart_get_series_next(chart, ser);
        }
    }
    else if(code == LV_EVENT_RELEASED) {
        lv_obj_invalidate(chart);
    }
}*/
//Функции установки значений параметров пк для работы анимаций
//загрузка проца
  static void set_cpu_value(void * indic, int32_t v)
  {
    lv_meter_set_indicator_end_value(cpumeter, (lv_meter_indicator_t *)indic, v);
  }
//загрузка видюхи
  static void set_gpu_value(void * indic, int32_t v)
  {
    lv_meter_set_indicator_end_value(gpumeter, (lv_meter_indicator_t *)indic, v);
  }
//использование озу
  static void set_cpuram(void * bar, int32_t ram)
  {
    lv_bar_set_value(rambar, ram, LV_ANIM_ON);
  }
//использование видеопамяти
  static void set_gpuram(void * bar, int32_t ram)
  {
    lv_bar_set_value(gpurambar, ram, LV_ANIM_ON);
  }
//Управление радио
//Громкость вверх
  void radio_volume_up(lv_event_t * e) //громкость+
  {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) 
    {
        if (vol<=21)
        {
          vol++;
          lv_bar_set_value(radio_volumembar, vol, LV_ANIM_ON);
          audio.setVolume(vol);
        }
    }
  }
//Громкость вниз
  void radio_volume_down(lv_event_t * e)//громкость-
  {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) 
    {
        if (vol>0)
        {
          vol--;
          lv_bar_set_value(radio_volumembar, vol, LV_ANIM_ON);
          audio.setVolume(vol);
        }
    }  
  }
//Изменение громкости слайдером
  static void radio_volumembar_event_cb(lv_event_t * e)
  {
    lv_obj_t * slider = lv_event_get_target(e);
    vol=lv_slider_get_value(slider);
    audio.setVolume(vol);
  }
//Канал вперед
  void radio_chanel_up(lv_event_t * e) //громкость+
  {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) 
    {
        //mp3 ->stop();
        sn++;
        if (sn>un)
        {
          sn=0;
        }
        lv_label_set_text_fmt(radio_station_num_label, "%d/%d%", sn+1, un+1); 
        url_sta(sn);
        lv_label_set_text(radio_playing_value_label, sta);
        if (rp==true) {audio.connecttohost(URL);
        audio.forceMono(true);
          lv_chart_set_type(radio_visualiser, LV_CHART_TYPE_BAR);
          refvisualiser.setInterval(50);
          }
    } 
  }
//Канал назад
  void radio_chanel_down(lv_event_t * e)//громкость-
  {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) 
    {
        
         sn--;
        if (sn<0)
        {
          sn=un;
        }
        lv_label_set_text_fmt(radio_station_num_label, "%d/%d%", sn+1, un+1); 
        url_sta(sn);
        lv_label_set_text(radio_playing_value_label, sta);
        if (rp==true) {audio.connecttohost(URL); 
        audio.forceMono(true);
          lv_chart_set_type(radio_visualiser, LV_CHART_TYPE_BAR);
          refvisualiser.setInterval(50);
          }
          }
  }
//запуск воспроизведения
  void radio_play_btn_press(lv_event_t * e)//громкость-
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * imgbtn = lv_event_get_target(e);
    if(code == LV_EVENT_CLICKED) 
    {
        if (rp==false)
        {
          lv_imgbtn_set_src(imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &stop, NULL);
         url_sta(sn);
          lv_label_set_text(radio_playing_value_label, sta);
          audio.connecttohost(URL); 
          lv_chart_set_type(radio_visualiser, LV_CHART_TYPE_BAR);
          refvisualiser.setInterval(50);
        }
        else
        {
          lv_imgbtn_set_src(imgbtn, LV_IMGBTN_STATE_RELEASED, NULL, &play, NULL);
          audio.stopSong();
          refvisualiser.stop();
          lv_chart_set_type(radio_visualiser, LV_CHART_TYPE_NONE);
        }
        rp=!rp;
    }
  }
//Закрытия окна плейлиста
  static void playlistwin_close_event( lv_event_t * e)
  {
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_RELEASED) 
    {
      lv_obj_add_flag(playlistwin, LV_OBJ_FLAG_HIDDEN);
    }
  }
//Включение режима редактирования плейлиста
  static void playlistwin_edit_event( lv_event_t * e)
  {
  playlist_edit=!playlist_edit;
  }
//Открытие плейлиста
  void radio_playlist_edit(lv_event_t * e) //Кнопка нажатия 
  {
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED) 
    {
        lv_obj_clear_flag(playlistwin, LV_OBJ_FLAG_HIDDEN);
        //Добавляем таблицу плейлиста
        if (refresh_playlist)
        {
        for (uint8_t i=1;i<=un;i++)
          {
            Serial.println(i);
            String tempurl=playlistread(SPIFFS,"/playlist.txt",i);
            if (tempurl!="Failed to open file for reading")
              {
            char radio_url[100], radio_sta[50];
            String u=tempurl.substring(0,tempurl.indexOf("*")+1);
            u.toCharArray(radio_url,u.length());           // URL
            String s=tempurl.substring(tempurl.indexOf("//")+2);
            String n=s.substring(s.indexOf("*")+1)+1;
            n.toCharArray(radio_sta,n.length());           // название станции
            lv_table_set_cell_value(playlist_table, i+1, 0, radio_sta);
            lv_table_set_cell_value(playlist_table, i+1, 1, radio_url);
              }
              else
              {
                lv_table_set_cell_value(playlist_table, i, 0, "Владивосток FM");
                lv_table_set_cell_value(playlist_table, i, 1, "http://vladfm.ru:8000/vfm");
              }
          }
          refresh_playlist=false;
        }
    } 
  }
//Обработка нажатия на таблицу плейлиста
  void playlist_table_press_event_cb(lv_event_t * e)//громкость-
  {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * table = lv_event_get_target(e);

    uint16_t col;
    uint16_t row;
    if(code == LV_EVENT_VALUE_CHANGED) 
    {
      lv_table_get_selected_cell(table, &row, &col);
      if (playlist_edit)
      {
      if (row>0)
      {
      lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(kb);
      lv_obj_t *textarea = lv_textarea_create(lv_scr_act());
      lv_textarea_set_one_line(textarea, true);
      lv_obj_align_to(textarea, kb,LV_ALIGN_OUT_TOP_LEFT, 0, 0);
      lv_obj_set_width(textarea,480);
      lv_textarea_set_text(textarea, lv_table_get_cell_value(table, row, col));
      lv_obj_add_state(textarea, LV_STATE_FOCUSED);
      lv_keyboard_set_textarea(kb, textarea);
      lv_obj_add_event_cb(textarea, ta_event_cb, LV_EVENT_ALL, kb);
      param=row*100+col;
      lv_obj_add_event_cb(textarea, radio_ta_event_cb, LV_EVENT_READY, &param);
      lv_obj_add_flag(textarea, LV_OBJ_FLAG_FLOATING);
      }
      }
      else
      {
        if (row>0)
        {
          sn=row-1;
        lv_label_set_text_fmt(radio_station_num_label, "%d/%d%", sn+1, un+1); 
       url_sta(sn);
        lv_label_set_text(radio_playing_value_label, sta);
       if (rp==true) {audio.connecttohost(URL);}
        }
      }
    }
    
  } 
//Перезагрузка ESP32
  void esp_restart_event(lv_event_t * e)
  {
    lv_obj_t * obj = lv_event_get_current_target(e);
    if (lv_msgbox_get_active_btn(obj)==0) ESP.restart();
  }    
//Сохранение плейлиста на sd карту
void sd_settings_playlist_save_event(lv_event_t * e)
    {
      if(!SD.begin(5,SDSPI)){
        Serial.println("Card Mount Failed");
      }
      else
      {
        sdSaveConf(SD, "/playlist.txt", "/playlist.txt");
        SD.end();
      }
    }
//Загрузка плейлиста с sd карты
void sd_settings_playlist_save_event(lv_event_t * e)
    {
      bool result;
      if(!SD.begin(5,SDSPI)){
        Serial.println("Card Mount Failed");
      }
      else
      {
        result=sdLoadConf(SD, "/playlist.txt", "/playlist.txt");
        SD.end();  
      }
    }
//Кнопка сохранения настроек на Sd карту
  void sd_settings_save_event(lv_event_t * e)
    {
      if(!SD.begin(5,SDSPI)){
        Serial.println("Card Mount Failed");
      }
      else
      {
        sdSaveConf(SD, "/confbackup.txt", "/config.txt");
        SD.end();
      }
    }
//Кнопка загрузки настроека с sd карты
     void sd_settings_load_event(lv_event_t * e)
    {
      bool result;
      if(!SD.begin(5,SDSPI)){
        Serial.println("Card Mount Failed");
      }
      else
      {
        result=sdLoadConf(SD, "/confbackup.txt", "/config.txt");
        SD.end();  
      }
    }

//Запуск WifiManager
/*  void button_set_wifimanager_event (lv_event_t * e)
    {
      if (!wm.startConfigPortal("Multiinformer")) {
        Serial.println("failed to connect or hit timeout");
        //delay(3000);
        // ESP.restart();
      } else {
        //if you get here you have connected to the WiFi
        Serial.println("connected...)");
      }
    }*/ 

//инициализация LVGL и создание всех объектов
void lvlg_create()
{
//Далее идут функции настройки LVGL   
      lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 8 ); //создаем буфер для вывода информации на экран
      //далее идет настройка параметров экрана
      static lv_disp_drv_t disp_drv; //объявляем переменную для хранения драйвера дисплея
      lv_disp_drv_init( &disp_drv ); //базовая инициализация драйвера
      disp_drv.hor_res = screenWidth; //ширина экрана
      disp_drv.ver_res = screenHeight; //высота экрана
      disp_drv.flush_cb = my_disp_flush; //функция которая выводит содержимое буфера в заданное место экрана. Указываем имя функции которую мы написали выше
      disp_drv.draw_buf = &draw_buf; //объявляем библиотеке, что содержимое буфера экрана находится в переменной draw_buf
      lv_disp_drv_register( &disp_drv ); //регистрируем драйвер дисплея и сохранем его настройки
      // Инициализируем драйвер тачскрина
      static lv_indev_drv_t indev_drv; //объявляем переменные для хранения драйвера тачскрина
      lv_indev_drv_init( &indev_drv ); // базовая инициализация драйвера
      indev_drv.type = LV_INDEV_TYPE_POINTER; //указываем тип драйвера. В данном случае это тачскрин
      indev_drv.read_cb = my_touchpad_read; //указываем имя функции обработчика нажатий на тачскрин, которую мы создали
      lv_indev_drv_register( &indev_drv ); //регистрация драйвера тачскрина и сохранение его настроек

//Создаем экранные объекты
 static lv_style_t bigtext;
    lv_style_init(&bigtext);
    lv_style_set_text_font(&bigtext, &fira24);       

 static lv_style_t style_indic;
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_GREEN));
    lv_style_set_bg_grad_color(&style_indic, lv_palette_main(LV_PALETTE_RED));

  tabview = lv_tabview_create(lv_scr_act(), LV_DIR_BOTTOM, 20); //создаем интерфейс вкладок
    //Создаем вкладки и присваиваем им отображаемые имена
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Главный");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "ПК");
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "Погода");
    lv_obj_t * tab4 = lv_tabview_add_tab(tabview, "Валюта");
    //lv_obj_t * tab5 = lv_tabview_add_tab(tabview, "РСЯ");
    lv_obj_t * tab6 = lv_tabview_add_tab(tabview, "Радио");
    lv_obj_t * tab7 = lv_tabview_add_tab(tabview, "Настройки");
    //обработчик событий для вкладок
    lv_obj_add_event_cb(tabview, change_tab_event, LV_EVENT_ALL, NULL);//смена активной вкладки
  //Создаем экранную клавиатуру 
  kb = lv_keyboard_create(lv_scr_act());//создаем экранную клавиатуру
  lv_obj_align(kb, LV_ALIGN_BOTTOM_MID, 0, 0); //положение на экране
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
  lv_obj_add_flag(kb, LV_OBJ_FLAG_FLOATING);  
  Serial.println("status");
  //Создаем иконки панели статуса
    //Метка статуса wifi
    wifistatus = lv_label_create(tabview);
    lv_obj_align(wifistatus, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_label_set_text(wifistatus, LV_SYMBOL_WIFI);
    lv_obj_add_flag(wifistatus, LV_OBJ_FLAG_FLOATING); 
    lv_obj_add_flag(wifistatus, LV_OBJ_FLAG_HIDDEN); 
    //Метка статуса wifi
    ui_status_clock = lv_label_create(tabview);
    lv_obj_align(ui_status_clock, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_label_set_text(ui_status_clock, "12:00");
    lv_obj_add_flag(ui_status_clock, LV_OBJ_FLAG_FLOATING); 
    lv_obj_add_flag(ui_status_clock, LV_OBJ_FLAG_HIDDEN);
 Serial.println("1 screen");   
//Первый экран
  //Календарь
  calendar = lv_calendar_create(tab1);
  lv_calendar_header_dropdown_create(calendar);
  lv_obj_set_size(calendar, 185, 200);
  lv_obj_align(calendar, LV_ALIGN_LEFT_MID, 0, -10);
  //lv_obj_add_event_cb(calendar, showceleb_event, LV_EVENT_ALL, NULL);
  //панель статуса
  statusbox = lv_label_create(tab1);
  lv_obj_align(statusbox, LV_ALIGN_BOTTOM_LEFT, 0, 0); //положение на экране
  lv_obj_set_size(statusbox,450,20);
  lv_label_set_long_mode(statusbox, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_label_set_text(statusbox, "");
  //часы
  displayclock = lv_label_create(tab1);
  lv_obj_align(displayclock, LV_ALIGN_TOP_LEFT, 210, 35); //положение на экране
  lv_obj_set_size(displayclock,LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(displayclock, &digital, 0);
  //Далее идут данные с датчика BME280
  //Температура в комнате
   ui_weatherimage_main = lv_img_create(tab1);
  lv_img_set_src(ui_weatherimage_main, &storm100);
  lv_obj_set_size( ui_weatherimage_main, LV_SIZE_CONTENT, LV_SIZE_CONTENT);  /// 1
  lv_obj_align( ui_weatherimage_main, LV_ALIGN_TOP_LEFT,210,100);
  lv_obj_add_flag(ui_weatherimage_main, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
  
  roomtemp = lv_label_create(tab1);
  //lv_obj_align_to(roomtemp,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_TOP, 5, 0); //положение на экране
  lv_obj_align(roomtemp, LV_ALIGN_TOP_LEFT, 210, 100); //положение на экране
  lv_obj_set_size(roomtemp,LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(roomtemp, &fira24, LV_PART_MAIN| LV_STATE_DEFAULT);
  lv_label_set_text(roomtemp, "25°С"); //сам текст для надписи
  
  roomhumid = lv_label_create(tab1);
  //lv_obj_align_to(roomhumid,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_MID, 5, 0); //положение на экране
  lv_obj_align(roomhumid, LV_ALIGN_TOP_RIGHT, 0, 100); //положение на экране
  lv_obj_set_size(roomhumid,LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(roomhumid, "66%"); //сам текст для надписи
  lv_obj_set_style_text_font(roomhumid, &fira24, LV_PART_MAIN| LV_STATE_DEFAULT);
  
  //атм. давление
  roompress = lv_label_create(tab1);
  lv_obj_align(roompress, LV_ALIGN_TOP_LEFT, 210, 130); //положение на экране
  //lv_obj_align_to(roompress,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 0); //положение на экране
  lv_obj_set_size(roompress,LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(roompress, "746 мм"); //сам текст для надписи
  lv_obj_set_style_text_font(roompress, &fira24, LV_PART_MAIN| LV_STATE_DEFAULT);
    roomair_bar = lv_bar_create(tab1);
    lv_obj_add_style(roomair_bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(roomair_bar, 230, 20);
    lv_obj_align(roomair_bar, LV_ALIGN_TOP_LEFT, 210, 180); //положение на экране
    lv_bar_set_range(roomair_bar, 0, 50);

  roomair = lv_label_create(tab1);
  lv_obj_align(roomair, LV_ALIGN_TOP_LEFT, 210, 160); //положение на экране
  lv_obj_set_size(roomair,LV_SIZE_CONTENT, LV_SIZE_CONTENT);

  roomair_bar_label = lv_label_create(roomair_bar);
  lv_obj_center(roomair_bar_label);
  //lv_label_set_text(roomair, LV_SYMBOL_AIR"good"); //сам текст для надписи
  //Изменяем положение объектов в зависимости от того включен ли датчик или нет
  if (usesensor==true)
    {
      lv_obj_add_flag(ui_weatherimage_main, LV_OBJ_FLAG_HIDDEN);
    }
    else
    {
      lv_obj_align_to(roomtemp,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_TOP, 5, 0); //положение на экране
      lv_obj_align_to(roomhumid,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_MID, 5, 0); //положение на экране
      lv_obj_align_to(roompress,ui_weatherimage_main, LV_ALIGN_OUT_RIGHT_BOTTOM, 5, 0); //положение на экране
      lv_obj_add_flag(roomair, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(roomair_bar, LV_OBJ_FLAG_HIDDEN);
      lv_obj_add_flag(roomair_bar_label, LV_OBJ_FLAG_HIDDEN);
    }
  
  /*Кнопка с информацией о системе
  lv_obj_t * ui_systeminfo_btn = lv_btn_create(tab1);
    lv_obj_add_event_cb(ui_systeminfo_btn, systeminfo_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_align(ui_systeminfo_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_size(ui_systeminfo_btn,15, 15);
  lv_obj_t * ui_systeminfo_label = lv_label_create(ui_systeminfo_btn);
    lv_label_set_text(ui_systeminfo_label, "I");
    lv_obj_center(ui_systeminfo_label);*/
Serial.println("2 screen"); 
//Вклака 2. параметры ПК
  //Монитор процессора
  cpumeter = lv_meter_create(tab2);
  lv_obj_align(cpumeter, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_set_size(cpumeter, 210, 210);
  lv_obj_remove_style(cpumeter, NULL, LV_PART_INDICATOR);
  //Шкала
  lv_meter_scale_t *cpuscale = lv_meter_add_scale(cpumeter);
  lv_meter_set_scale_ticks(cpumeter, cpuscale, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(cpumeter, cpuscale, 1, 2, 30, lv_color_hex3(0xeee), 15);
  lv_meter_set_scale_range(cpumeter, cpuscale, 0, 100, 270, 90);
  //Индикаторы
  cpufan_indic = lv_meter_add_arc(cpumeter, cpuscale, 10, lv_palette_main(LV_PALETTE_ORANGE), -20);
  cpu_indic = lv_meter_add_arc(cpumeter, cpuscale, 10, lv_palette_main(LV_PALETTE_BLUE), -10);
  cputemp_indic = lv_meter_add_arc(cpumeter, cpuscale, 10, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value(cpumeter, cpu_indic, 0);
  lv_meter_set_indicator_start_value(cpumeter, cputemp_indic, 0);
  //Метки для вывода данных
  cpu_load_label = lv_label_create(cpumeter); //
  lv_obj_align(cpu_load_label, LV_ALIGN_CENTER, 40, 20); //положение на экране
  lv_obj_set_style_text_color(cpu_load_label,lv_palette_main(LV_PALETTE_BLUE),0);
  cpu_temp_label = lv_label_create(cpumeter); //
  lv_obj_align(cpu_temp_label, LV_ALIGN_CENTER, 40, 40); //положение на экране
  lv_obj_set_style_text_color(cpu_temp_label,lv_palette_main(LV_PALETTE_RED),0);
  cpu_fan_label = lv_label_create(cpumeter); //
  lv_obj_align(cpu_fan_label, LV_ALIGN_CENTER, 40, 60); //положение на экране
  lv_obj_set_style_text_color(cpu_fan_label,lv_palette_main(LV_PALETTE_ORANGE),0);
  lv_obj_t *ui_cpuname= lv_label_create(cpumeter);
  lv_obj_align(ui_cpuname, LV_ALIGN_CENTER, 0, 0); //положение на экране
  lv_obj_set_style_text_font(ui_cpuname, &fira24, 0);
  lv_label_set_text(ui_cpuname, "CPU");

  //Монитор видеокарты
  gpumeter = lv_meter_create(tab2);
  lv_obj_align(gpumeter, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_size(gpumeter, 210, 210);
  lv_obj_remove_style(gpumeter, NULL, LV_PART_INDICATOR);
  //Шкала
  lv_meter_scale_t *gpuscale = lv_meter_add_scale(gpumeter);
  lv_meter_set_scale_ticks(gpumeter, gpuscale, 11, 2, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_major_ticks(gpumeter, gpuscale, 1, 2, 30, lv_color_hex3(0xeee), 15);
  lv_meter_set_scale_range(gpumeter, gpuscale, 0, 100, 270, 90);
  
  //Индикаторы
  gpufan_indic = lv_meter_add_arc(gpumeter, gpuscale, 10, lv_palette_main(LV_PALETTE_YELLOW), -20);
  gpu_indic = lv_meter_add_arc(gpumeter, gpuscale, 10, lv_palette_main(LV_PALETTE_GREEN), -10);
  gputemp_indic = lv_meter_add_arc(gpumeter, gpuscale, 10, lv_palette_main(LV_PALETTE_RED), 0);
  lv_meter_set_indicator_start_value(gpumeter, gpu_indic, 0);
  lv_meter_set_indicator_start_value(gpumeter, gputemp_indic, 0);
  
  //Метки для данных
  gpu_load_label = lv_label_create(gpumeter); //создаем объект Надпись как дочерний объект screenMain
  lv_obj_align(gpu_load_label, LV_ALIGN_CENTER, 40, 20); //положение на экране
  lv_obj_set_style_text_color(gpu_load_label,lv_palette_main(LV_PALETTE_GREEN),0);
  gpu_temp_label = lv_label_create(gpumeter); //создаем объект Надпись как дочерний объект screenMain
  lv_obj_align(gpu_temp_label, LV_ALIGN_CENTER, 40, 40); //положение на экране
  lv_obj_set_style_text_color(gpu_temp_label,lv_palette_main(LV_PALETTE_RED),0);
  gpu_fan_label = lv_label_create(gpumeter); //создаем объект Надпись как дочерний объект screenMain
  lv_obj_align(gpu_fan_label, LV_ALIGN_CENTER, 40, 60); //положение на экране
  lv_obj_set_style_text_color(gpu_fan_label,lv_palette_main(LV_PALETTE_YELLOW),0);
  lv_obj_t *ui_gpuname= lv_label_create(gpumeter);
  lv_obj_align(ui_gpuname, LV_ALIGN_CENTER, 0, 0); //положение на экране
  lv_obj_set_style_text_font(ui_gpuname, &fira24, 0);
  lv_label_set_text(ui_gpuname, "GPU");

    //Иницализация анимации
    lv_anim_init(&cpu);
    lv_anim_set_exec_cb(&cpu, set_cpu_value);
    lv_anim_init(&gpu);
    lv_anim_set_exec_cb(&gpu, set_gpu_value);
    
    //создание полосок загрузки RAM
    rambar = lv_bar_create(tab2);
    lv_obj_set_style_bg_color(rambar,lv_color_hex(0x005cb8),LV_PART_INDICATOR);
    lv_obj_set_size(rambar, 460, 20);
    lv_obj_align(rambar, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_t * ramlabel= lv_label_create(rambar);
    lv_obj_align(ramlabel, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_style_text_color(ramlabel,lv_color_hex(0xfafafa),0);
    lv_label_set_text(ramlabel, "ОЗУ"); //сам текст для надписи
    ram_val_label= lv_label_create(rambar);
    lv_obj_set_style_text_color(ram_val_label,lv_color_hex(0xfafafa),0);
    lv_obj_align(ram_val_label, LV_ALIGN_RIGHT_MID, 0, 0);

    gpurambar = lv_bar_create(tab2);
    //lv_obj_add_event_cb(rambar, event_cb, LV_EVENT_DRAW_PART_END, NULL);
    lv_obj_set_style_bg_color(gpurambar,lv_color_hex(0x005800),LV_PART_INDICATOR);
    lv_obj_set_size(gpurambar, 460, 20);
    lv_obj_align(gpurambar, LV_ALIGN_BOTTOM_LEFT, 0, -30);
    lv_obj_t * gpuramlabel= lv_label_create(gpurambar);
    lv_obj_align(gpuramlabel, LV_ALIGN_LEFT_MID, 0, 0);
    lv_label_set_text(gpuramlabel, "GPU ОЗУ"); //сам текст для надписи
    lv_obj_set_style_text_color(gpuramlabel,lv_color_hex(0xfafafa),0);
    gpuram_val_label= lv_label_create(gpurambar);
    lv_obj_align(gpuram_val_label, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_text_color(gpuram_val_label,lv_color_hex(0xfafafa),0);
    lv_bar_set_range(rambar, 0, 32);
    lv_bar_set_range(gpurambar, 0, 4096);

    //Инициализация анимации
    lv_anim_init(&ramcpu);
    lv_anim_set_exec_cb(&ramcpu, set_cpuram);
    lv_anim_init(&ramgpu);
    lv_anim_set_exec_cb(&ramgpu, set_gpuram);

    //сеть 
    ui_upload_label= lv_label_create(tab2);
    lv_obj_align(ui_upload_label, LV_ALIGN_BOTTOM_MID, 0, -70);
    ui_download_label= lv_label_create(tab2);
    lv_obj_align(ui_download_label, LV_ALIGN_BOTTOM_MID, 0, -50);

Serial.println("3 screen"); 
//Вкладка 3 экран погоды
  //Название города
  
  weathercity = lv_label_create(tab3); //создаем объект 
  lv_label_set_text(weathercity, "Губкин"); //сам текст для надписи
  lv_obj_align(weathercity, LV_ALIGN_TOP_MID, 0, 0); //положение на экране
  //Направление ветра
  windmeter = lv_meter_create(tab3);
  lv_obj_align(windmeter, LV_ALIGN_TOP_LEFT, 10, 10);
  lv_obj_set_size(windmeter, 120, 120);
  lv_obj_remove_style(windmeter, NULL, LV_PART_INDICATOR);
  lv_meter_scale_t * scale_wind = lv_meter_add_scale(windmeter);
  lv_meter_set_scale_ticks(windmeter, scale_wind, 37, 1, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_range(windmeter, scale_wind, 0, 36, 360, 270);
  //wind_indic = lv_meter_add_needle_img(windmeter, scale_wind, &arrow, 24, 12);
  wind_indic = lv_meter_add_needle_line(windmeter, scale_wind, 2, lv_palette_main(LV_PALETTE_GREY), 0);
  lv_obj_add_flag(windmeter, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
  //Вывод скорости ветра
  ui_windspeed = lv_label_create(windmeter); //создаем объект 
  lv_label_set_text(ui_windspeed, "15"); //сам текст для надписи
  lv_obj_align(ui_windspeed, LV_ALIGN_CENTER, 0, 0); //положение на экране
  //lv_obj_set_style_text_font(ui_windspeed, &fira28, LV_PART_MAIN| LV_STATE_DEFAULT);

  ui_windgust = lv_label_create(tab3); //создаем объект 
  lv_label_set_text(ui_windgust, "Порывы ветра"); //сам текст для надписи
  lv_obj_align(ui_windgust, LV_ALIGN_TOP_LEFT, 0, 130); //положение на экране
  //Метки сторон света
  lv_obj_t * wind_north = lv_label_create(windmeter);
  lv_obj_align( wind_north, LV_ALIGN_TOP_MID,0,-20);
  lv_label_set_text(wind_north,"С");
  
  lv_obj_t * wind_south = lv_label_create(windmeter);
  lv_obj_align( wind_south, LV_ALIGN_BOTTOM_MID,0,20 );
  lv_label_set_text(wind_south,"Ю");

  lv_obj_t * wind_west = lv_label_create(windmeter);
  lv_obj_align( wind_west, LV_ALIGN_LEFT_MID,-15,0);
  lv_label_set_text(wind_west,"З");

  lv_obj_t * wind_east = lv_label_create(windmeter);
  lv_obj_align( wind_east, LV_ALIGN_RIGHT_MID,15,0 );
  lv_label_set_text(wind_east,"В");

  //Создаем надписи для погоды
  //Температура
  ui_weathertemp = lv_label_create(tab3);
  lv_obj_set_pos(ui_weathertemp, 155, 30);
  lv_label_set_text(ui_weathertemp,"1.0");
  lv_obj_set_style_text_font(ui_weathertemp, &fira24, LV_PART_MAIN| LV_STATE_DEFAULT);
  //Влажность
  ui_humid = lv_label_create(tab3);
  lv_obj_set_pos(ui_humid, 250, 30);
  lv_label_set_text(ui_humid,"55%");
  lv_obj_set_style_text_font(ui_humid, &fira24, LV_PART_MAIN| LV_STATE_DEFAULT);
  //Мин и макст температура
  ui_avgtemp = lv_label_create(tab3);
  lv_obj_set_pos(ui_avgtemp, 155, 80);
  lv_label_set_text(ui_avgtemp,"1 | 5");
  //Температура ощущается как
  ui_feeltemp = lv_label_create(tab3);
  lv_obj_set_pos(ui_feeltemp, 155, 60);
  lv_label_set_text(ui_feeltemp,"25");

  //Атм давление
  ui_pressure = lv_label_create(tab3);
  lv_obj_set_pos(ui_pressure, 155, 100);
  lv_label_set_text(ui_pressure,"1016Pa");
  //Видимость
  ui_visiblity = lv_label_create(tab3);
  lv_obj_set_pos(ui_visiblity, 250, 100);
  lv_label_set_text(ui_visiblity,"10000");
  //Восход
  ui_sunrise = lv_label_create(tab3);
  lv_obj_set_pos(ui_sunrise, 155, 120);
  lv_label_set_text(ui_sunrise,"Р07:15");
  //Закат
  ui_sunset = lv_label_create(tab3);
  lv_obj_set_pos(ui_sunset, 250, 120);
  lv_label_set_text(ui_sunset,"З21:15");
    
  //Картинка с текущей погодой
  ui_weatherimage = lv_img_create(tab3);
  lv_img_set_src(ui_weatherimage, &storm100);
  lv_obj_set_size( ui_weatherimage, LV_SIZE_CONTENT, LV_SIZE_CONTENT);  /// 1
  lv_obj_align( ui_weatherimage, LV_ALIGN_TOP_RIGHT,-10,30 );
  lv_obj_add_flag(ui_weatherimage, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
  //Описание погоды
  ui_weatherdesc = lv_label_create(tab3);
  lv_obj_align_to( ui_weatherdesc,ui_weatherimage, LV_ALIGN_OUT_BOTTOM_MID,0,0 );
  lv_obj_set_size(ui_weatherdesc,120,LV_SIZE_CONTENT);
  lv_label_set_text(ui_weatherdesc,"Broken clouds");
  lv_label_set_long_mode(ui_weatherdesc, LV_LABEL_LONG_WRAP);

  //Графики
  //Атмосферного давления
  ui_pressurechart = lv_chart_create(tab3);
  lv_obj_set_size(ui_pressurechart,110,80);
  lv_obj_align( ui_pressurechart, LV_ALIGN_BOTTOM_LEFT,40,-5 );
  lv_chart_set_type( ui_pressurechart, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count( ui_pressurechart, 24);
  lv_obj_set_style_size(ui_pressurechart, 0, LV_PART_INDICATOR);
  lv_chart_set_range( ui_pressurechart, LV_CHART_AXIS_PRIMARY_Y, 700, 800);
  lv_chart_set_range( ui_pressurechart, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
  lv_chart_set_axis_tick( ui_pressurechart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 3, 2, false, 50);
  lv_chart_set_axis_tick( ui_pressurechart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 2, true, 50);
  ui_prser = lv_chart_add_series(ui_pressurechart, lv_color_hex(0x00AA00), LV_CHART_AXIS_PRIMARY_Y);
  lv_obj_add_flag(ui_pressurechart, LV_OBJ_FLAG_OVERFLOW_VISIBLE);
  //Заголовок
  lv_obj_t * ui_pressurechartlabel = lv_label_create(ui_pressurechart);
  lv_obj_set_size(ui_pressurechartlabel,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
  lv_obj_align( ui_pressurechartlabel, LV_ALIGN_TOP_MID,0,-30 );
  lv_label_set_text(ui_pressurechartlabel,"Давление");

  //температуры
  ui_tempchart = lv_chart_create(tab3);
  lv_obj_set_size(ui_tempchart,110,80);
  lv_obj_align( ui_tempchart, LV_ALIGN_BOTTOM_LEFT,190,-5 );
  lv_chart_set_type( ui_tempchart, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count( ui_tempchart, 24);
  lv_obj_set_style_size(ui_tempchart, 0, LV_PART_INDICATOR);
  lv_chart_set_range( ui_tempchart, LV_CHART_AXIS_PRIMARY_Y, -30, 40);
  lv_chart_set_range( ui_tempchart, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
  lv_chart_set_axis_tick( ui_tempchart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 3, 2, false, 50);
  lv_chart_set_axis_tick( ui_tempchart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 5, 2, true, 50);
  ui_tempser = lv_chart_add_series(ui_tempchart, lv_color_hex(0xAA0000), LV_CHART_AXIS_PRIMARY_Y);
  lv_obj_add_flag(ui_tempchart, LV_OBJ_FLAG_OVERFLOW_VISIBLE); 
  //Заголовок
  lv_obj_t * ui_tempchartlabel = lv_label_create(ui_tempchart);
  lv_obj_set_size(ui_tempchartlabel,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
  lv_obj_align( ui_tempchartlabel, LV_ALIGN_TOP_MID,0,-30 );
  lv_label_set_text(ui_tempchartlabel,"Температура");

  //Влажности
  ui_humidchart = lv_chart_create(tab3);
  lv_obj_set_size(ui_humidchart,110,80);
  lv_obj_align( ui_humidchart, LV_ALIGN_BOTTOM_LEFT,350,-5 );
  lv_chart_set_type( ui_humidchart, LV_CHART_TYPE_LINE);
  lv_chart_set_point_count( ui_humidchart, 24);
  lv_obj_set_style_size(ui_humidchart, 0, LV_PART_INDICATOR);
  lv_chart_set_range( ui_humidchart, LV_CHART_AXIS_SECONDARY_Y, 0, 0);
  lv_chart_set_axis_tick( ui_humidchart, LV_CHART_AXIS_PRIMARY_X, 10, 5, 3, 2, false, 50);
  lv_chart_set_axis_tick( ui_humidchart, LV_CHART_AXIS_PRIMARY_Y, 10, 5, 6, 2, true, 50);
  ui_huser= lv_chart_add_series(ui_humidchart, lv_color_hex(0x0000CC), LV_CHART_AXIS_PRIMARY_Y);
  lv_obj_add_flag(ui_humidchart, LV_OBJ_FLAG_OVERFLOW_VISIBLE); 
  //Заголовок
  lv_obj_t * ui_humidchartlabel = lv_label_create(ui_humidchart);
  lv_obj_set_size(ui_humidchartlabel,LV_SIZE_CONTENT,LV_SIZE_CONTENT);
  lv_obj_align( ui_humidchartlabel, LV_ALIGN_TOP_MID,0,-30 );
  lv_label_set_text(ui_humidchartlabel,"Влажность");
  Serial.println("4 screen"); 
//Вкладка 4. Курс валют     
    ui_cur_title = lv_label_create(tab4);
    lv_obj_align(ui_cur_title, LV_ALIGN_TOP_MID, 0, 0); //положение на экране
    //lv_obj_set_style_text_font(ui_cur_title, &fira28, 0);
    lv_label_set_text(ui_cur_title, "Курсы валют"); //сам текст для надписи

    //создание таблицы курса валют
    cur_table = lv_table_create(tab4);
     lv_obj_set_size(cur_table, 440, 230);
     lv_obj_align(cur_table, LV_ALIGN_TOP_LEFT, 0, 20);
     lv_table_set_col_width(cur_table, 0, 300);
     lv_obj_add_event_cb(cur_table, draw_table_part_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

   
  Serial.println("5 screen"); 

//Экран радио вкладка 6  
  Serial.println("radio");
  radio_playing_value_label = lv_label_create(tab6); //номер станции
  //lv_label_set_text(radio_station_num_label, "1/73:"); //сам текст для надписи
  lv_obj_align(radio_playing_value_label, LV_ALIGN_TOP_MID, 0, 0); //положение на экране

  radio_playing_label = lv_label_create(tab6); //создаем объект заголовок
  lv_label_set_text(radio_playing_label, "Трек:"); //сам текст для надписи
  lv_obj_align(radio_playing_label, LV_ALIGN_BOTTOM_LEFT, 0, -100); //положение на экране
  //lv_obj_set_width(radio_playing_label, 450);
  //lv_label_set_long_mode(radio_playing_label, LV_LABEL_LONG_CLIP);

  radio_visualiser = lv_chart_create(tab6);
  lv_obj_set_size(radio_visualiser, 430, 120);
  lv_chart_set_type(radio_visualiser, LV_CHART_TYPE_BAR);
  lv_chart_set_update_mode(radio_visualiser, LV_CHART_UPDATE_MODE_CIRCULAR);
  lv_chart_set_point_count(radio_visualiser, 32);
  lv_chart_set_div_line_count(radio_visualiser, 0, 0);
  lv_obj_align(radio_visualiser, LV_ALIGN_BOTTOM_LEFT, 10, -120);
  //массив данных визуализатора
  radio_visualiser_series = lv_chart_add_series(radio_visualiser, lv_palette_main(LV_PALETTE_BLUE), LV_CHART_AXIS_PRIMARY_Y);
  for (int i=0; i<32;i++){lv_chart_set_next_value(radio_visualiser, radio_visualiser_series, lv_rand(20, 100));}
  lv_chart_set_type(radio_visualiser, LV_CHART_TYPE_NONE);  
  radio_station_num_label = lv_label_create(tab6); //номер станции
  lv_label_set_text(radio_station_num_label, "1/73"); //сам текст для надписи
  lv_obj_align(radio_station_num_label, LV_ALIGN_TOP_RIGHT, 0, 0); //положение на экране
  
  radio_volumembar = lv_slider_create(tab6);
    //lv_obj_set_style_bg_color(gpurambar,lv_color_hex(0x005800),LV_PART_INDICATOR);
  lv_obj_set_size(radio_volumembar, 380, 20);
  lv_obj_align(radio_volumembar, LV_ALIGN_BOTTOM_MID, 0, -80); //положение на экране
  lv_slider_set_range(radio_volumembar, 0, 21);
  lv_slider_set_value(radio_volumembar, vol, LV_ANIM_OFF);
  lv_obj_add_event_cb(radio_volumembar, radio_volumembar_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  Serial.println("playlist screen"); 
  //Окно плейлиста
        playlistwin = lv_win_create(lv_scr_act(), 40);
        lv_obj_t * playlistedit_btn = lv_win_add_btn(playlistwin, LV_SYMBOL_EDIT, 40);
        lv_obj_add_event_cb(playlistedit_btn, playlistwin_edit_event, LV_EVENT_VALUE_CHANGED, NULL);
        lv_obj_add_flag(playlistedit_btn, LV_OBJ_FLAG_CHECKABLE);
        lv_win_add_title(playlistwin, "Список станций");
        lv_obj_t * btn = lv_win_add_btn(playlistwin, LV_SYMBOL_CLOSE, 60);
        lv_obj_add_event_cb(btn, playlistwin_close_event, LV_EVENT_ALL, NULL);
        lv_obj_t * cont = lv_win_get_content(playlistwin);
        playlist_table = lv_table_create(cont);
        lv_obj_set_width(playlist_table, 445);
        lv_obj_set_pos(playlist_table, 0, 0);
        lv_table_set_col_cnt(playlist_table, 2);
        lv_table_set_col_width(playlist_table, 0, 127);
        lv_table_set_col_width(playlist_table, 1, 314);
        lv_table_set_cell_value(playlist_table, 0, 0, "Название");
        lv_table_set_cell_value(playlist_table, 0, 1, "Адрес");
        lv_obj_add_event_cb(playlist_table, playlist_table_press_event_cb, LV_EVENT_ALL, NULL);
        lv_obj_add_flag(playlistwin, LV_OBJ_FLAG_HIDDEN);
  Serial.println("radio buttons");
  //кнопки
  lv_obj_t * radio_volmin_btn = lv_imgbtn_create(tab6);
    lv_imgbtn_set_src(radio_volmin_btn, LV_IMGBTN_STATE_RELEASED, NULL, &volmin, NULL);
    lv_obj_align_to(radio_volmin_btn, radio_volumembar, LV_ALIGN_LEFT_MID, -20, 0);
    lv_obj_set_size(radio_volmin_btn, 24, 24);
    lv_obj_add_event_cb(radio_volmin_btn, radio_volume_down, LV_EVENT_ALL, NULL);

    lv_obj_t * radio_volmax_btn = lv_imgbtn_create(tab6);
    lv_imgbtn_set_src(radio_volmax_btn, LV_IMGBTN_STATE_RELEASED, NULL, &volmax, NULL);
    lv_obj_align_to(radio_volmax_btn, radio_volumembar, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_set_size(radio_volmax_btn, 24, 24);
    lv_obj_add_event_cb(radio_volmax_btn, radio_volume_up, LV_EVENT_ALL, NULL);
    lv_label_set_text_fmt(radio_station_num_label, "%d/%d", sn+1,un+1); 
    url_sta(sn);
    lv_label_set_text(radio_playing_value_label, sta);
  //кнопки управления плеером
    lv_obj_t * radio_play_btn = lv_imgbtn_create(tab6);
    lv_imgbtn_set_src(radio_play_btn, LV_IMGBTN_STATE_RELEASED, NULL, &play, NULL);
    lv_obj_align(radio_play_btn, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_size(radio_play_btn, 70, 70);
    lv_obj_add_event_cb(radio_play_btn, radio_play_btn_press, LV_EVENT_ALL, NULL);
    
    lv_obj_t * radio_next_btn = lv_imgbtn_create(tab6);
    lv_imgbtn_set_src(radio_next_btn, LV_IMGBTN_STATE_RELEASED, NULL, &nextst, NULL);
    lv_obj_align_to(radio_next_btn, radio_play_btn, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_obj_set_size(radio_next_btn, 30, 30);
    lv_obj_add_event_cb(radio_next_btn, radio_chanel_up, LV_EVENT_ALL, NULL);
    
    lv_obj_t * radio_prev_btn = lv_imgbtn_create(tab6);
    lv_imgbtn_set_src(radio_prev_btn, LV_IMGBTN_STATE_RELEASED, NULL, &prevst, NULL);
    lv_obj_align_to(radio_prev_btn, radio_play_btn, LV_ALIGN_LEFT_MID, -40, 0);
    lv_obj_set_size(radio_prev_btn, 30, 30);
    lv_obj_add_event_cb(radio_prev_btn, radio_chanel_down, LV_EVENT_ALL, NULL);

    lv_obj_t * radio_playlist_btn = lv_imgbtn_create(tab6);
    lv_imgbtn_set_src(radio_playlist_btn, LV_IMGBTN_STATE_RELEASED, NULL, &playlist, NULL);
    lv_obj_align(radio_playlist_btn, LV_ALIGN_BOTTOM_RIGHT, 0, -20);
    lv_obj_set_size(radio_playlist_btn, 30, 30);
    lv_obj_add_event_cb(radio_playlist_btn, radio_playlist_edit, LV_EVENT_ALL, NULL);

  Serial.println("Settings screen");
//Экран настроек вкладка 7
   //Создаем категории настроек
    set_tabview = lv_tabview_create(tab7, LV_DIR_LEFT, 80);
        lv_obj_t * settab1 = lv_tabview_add_tab(set_tabview, "Основные");
        lv_obj_t * settab6 = lv_tabview_add_tab(set_tabview, "WiFi");
        lv_obj_t * settab2 = lv_tabview_add_tab(set_tabview, "Монитор ПК");
        lv_obj_t * settab3 = lv_tabview_add_tab(set_tabview, "Погода");
        lv_obj_t * settab4 = lv_tabview_add_tab(set_tabview, "Датчик BME");
        lv_obj_t * settab5 = lv_tabview_add_tab(set_tabview, "SD карта");
        lv_obj_add_event_cb(set_tabview, change_settings_tab_event, LV_EVENT_ALL, NULL);//смена активной вкладки
//1 вкладка настроек Основные
    //Настройки дисплея
    lv_obj_t * settingspanel1 = lv_obj_create(settab1);
    lv_obj_set_size(settingspanel1, 340,125);
    lv_obj_t  * ui_label_set_cat_display = lv_label_create(settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_set_cat_display, "Дисплей"); //сам текст для надписи
    lv_obj_align(ui_label_set_cat_display, LV_ALIGN_TOP_MID, 0, 0); //положение на экране  
  
    lv_obj_t  * ui_label_changebrightness = lv_label_create(settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_changebrightness, "Настройка яркости экрана"); //сам текст для надписи
    lv_obj_align(ui_label_changebrightness, LV_ALIGN_TOP_LEFT, 0, 20); //положение на экране
    //выключаетль автояркости
    lv_obj_t * autobright_switch = lv_switch_create(settingspanel1);
    lv_obj_add_event_cb(autobright_switch, autobright_switch_event, LV_EVENT_ALL, NULL);
    lv_obj_align(autobright_switch, LV_ALIGN_TOP_RIGHT, 0, 20); //положение на экране
    lv_obj_set_size(autobright_switch,32,16);
    if (photosensor)
    {
      lv_obj_add_state(autobright_switch, LV_STATE_CHECKED); 
    }
    else
    {
      lv_obj_clear_state(autobright_switch, LV_STATE_CHECKED);
    }
    //Надпись авторяркости
    lv_obj_t  * ui_label_autobrightness = lv_label_create(settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_autobrightness, "Авто"); //сам текст для надписи
    lv_obj_align(ui_label_autobrightness, LV_ALIGN_TOP_RIGHT, -36, 20); //положение на экране
    //Создаем слайдер изменения яркости
    lv_obj_t * slider_brightness = lv_slider_create(settingspanel1);
    lv_obj_align(slider_brightness, LV_ALIGN_TOP_LEFT, 0, 50);
    lv_obj_set_width(slider_brightness,300);
    lv_slider_set_range(slider_brightness, 1 , 255);
    lv_slider_set_value(slider_brightness, bright_level, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_brightness, slider_brightness_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
    slider_label = lv_label_create(settingspanel1);
    lv_label_set_text_fmt(slider_label, "%d%", (int)lv_slider_get_value(slider_brightness));
    lv_obj_align_to(slider_label, slider_brightness, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t  * ui_label_change_rgb_ind = lv_label_create(settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_change_rgb_ind, "RGB индикатор"); //сам текст для надписи
    lv_obj_align(ui_label_change_rgb_ind, LV_ALIGN_TOP_LEFT, 0, 80); //положение на экране
    //выключатель rgb индикатора
    lv_obj_t * rgb_indic_switch = lv_switch_create(settingspanel1);
    lv_obj_add_event_cb(rgb_indic_switch , rgb_indic_switch_event, LV_EVENT_ALL, NULL);
    lv_obj_align(rgb_indic_switch , LV_ALIGN_TOP_RIGHT, 0, 80); //положение на экране
    lv_obj_set_size(rgb_indic_switch ,32,16);
    if (ledindicator)
    {
      lv_obj_add_state(rgb_indic_switch , LV_STATE_CHECKED); 
    }
    else
    {
      lv_obj_clear_state(rgb_indic_switch , LV_STATE_CHECKED);
    }

    //Настройки NTP
    lv_obj_t * settingspanel2 = lv_obj_create(settab1);
    lv_obj_set_size(settingspanel2, 340,LV_SIZE_CONTENT);
    lv_obj_set_pos(settingspanel2, 0, 135);
    lv_obj_t  * ui_label_set_cat_time = lv_label_create(settingspanel2); //создаем объект заголовок
    lv_label_set_text(ui_label_set_cat_time, "NTP"); //сам текст для надписи
    lv_obj_align(ui_label_set_cat_time, LV_ALIGN_TOP_MID, 0, 0); //положение на экране

    lv_obj_t  * ui_label_ntp_server = lv_label_create(settingspanel2); //создаем объект заголовок
    lv_label_set_text(ui_label_ntp_server, "Сервер:"); //сам текст для надписи
    lv_obj_align(ui_label_ntp_server, LV_ALIGN_TOP_LEFT, 0, 30); //положение на экране 

    lv_obj_t * ui_dd_ntp_server = lv_dropdown_create(settingspanel2);
    lv_dropdown_set_options(ui_dd_ntp_server, "pool.ntp.org\n"
                            "ntp.msk-ix.ru\n"
                            "ntp1.vniiftri.ru\n"
                            "ntp1.stratum2.ru\n"
                            "ntp2.stratum2.ru\n"
                            "ntp.msk-ix.ru");
    lv_obj_align_to(ui_dd_ntp_server, ui_label_ntp_server, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_width(ui_dd_ntp_server,240);
    lv_dropdown_set_selected(ui_dd_ntp_server, lv_dropdown_get_option_index(ui_dd_ntp_server, ntpserver));
    lv_obj_add_event_cb(ui_dd_ntp_server, ui_dd_ntp_server_event, LV_EVENT_VALUE_CHANGED, NULL);
  
    lv_obj_t  * ui_label_ntp_gmt = lv_label_create(settingspanel2); //создаем объект заголовок
    lv_label_set_text(ui_label_ntp_gmt, "Часовой пояс:"); //сам текст для надписи
    lv_obj_align(ui_label_ntp_gmt, LV_ALIGN_TOP_LEFT, 0, 70); 

    lv_obj_t * slider_gmt = lv_slider_create(settingspanel2);
    lv_obj_align(slider_gmt, LV_ALIGN_TOP_LEFT, 0, 100);
    lv_obj_set_width(slider_gmt,300);
    lv_slider_set_range(slider_gmt, -12 , 14);
    lv_slider_set_value(slider_gmt, gmt, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_gmt, slider_gmt_event_cb, LV_EVENT_VALUE_CHANGED, NULL);

    gmt_slider_label = lv_label_create(settingspanel2);
    lv_label_set_text_fmt(gmt_slider_label, "GMT: %d", (int)lv_slider_get_value(slider_gmt));
    lv_obj_align_to(gmt_slider_label, slider_gmt, LV_ALIGN_CENTER, 0, 0);
  
//2 вкладка настроек ПК монитор
  lv_obj_t * pc_settingspanel1 = lv_obj_create(settab2);
  lv_obj_set_size(pc_settingspanel1, 340,LV_SIZE_CONTENT);
 
  lv_obj_t  * ui_label_pc_server = lv_label_create(pc_settingspanel1); //создаем объект заголовок
  lv_label_set_text(ui_label_pc_server, "Адрес сервера:"); //сам текст для надписи
  lv_obj_align(ui_label_pc_server , LV_ALIGN_TOP_LEFT, 0, 0); //положение на экране
 
  pc_ta = lv_textarea_create(pc_settingspanel1);
    lv_textarea_set_one_line(pc_ta, true);
    lv_obj_align(pc_ta, LV_ALIGN_TOP_LEFT, 0, 20);
     lv_obj_set_width(pc_ta,300);
     lv_textarea_set_text(pc_ta, pc_server_path.c_str());
     lv_obj_add_event_cb(pc_ta, ta_event_cb, LV_EVENT_ALL, kb);
     lv_obj_add_event_cb(pc_ta, pc_ta_event_cb, LV_EVENT_READY, NULL);

  lv_obj_t  * ui_label_pc_interval = lv_label_create(pc_settingspanel1); //создаем объект заголовок
  lv_label_set_text(ui_label_pc_interval, "Интервал обновления (секунды):"); //сам текст для надписи
  lv_obj_align(ui_label_pc_interval , LV_ALIGN_TOP_LEFT, 0, 70); //положение на экране
  
  //Создаем слайдер изменения обновления
  lv_obj_t * slider_pc_int = lv_slider_create(pc_settingspanel1);
  lv_obj_align(slider_pc_int, LV_ALIGN_TOP_LEFT, 0, 100);
  lv_obj_set_width(slider_pc_int,300);
  lv_slider_set_range(slider_pc_int, 1 , 60);
  lv_slider_set_value(slider_pc_int, refpcinterval/1000, LV_ANIM_OFF);
  lv_obj_add_event_cb(slider_pc_int, slider_pcint_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
  pc_int_slider_label = lv_label_create(pc_settingspanel1);
  lv_label_set_text_fmt(pc_int_slider_label, "%d%", (int)lv_slider_get_value(slider_pc_int));
   lv_obj_align_to(pc_int_slider_label, slider_pc_int, LV_ALIGN_CENTER, 0, 0);

//3 вкладка настроек Погода
  lv_obj_t * wt_settingspanel1 = lv_obj_create(settab3);
  lv_obj_set_size(wt_settingspanel1, 340,LV_SIZE_CONTENT);
  
  lv_obj_t  * ui_label_weather_api = lv_label_create(wt_settingspanel1); //создаем объект заголовок
  lv_label_set_text(ui_label_weather_api, "OpenWeatherMap API ключ:"); //сам текст для надписи
  lv_obj_align(ui_label_weather_api , LV_ALIGN_TOP_LEFT, 0, 0); //положение на экране
 
  wt_ta = lv_textarea_create(wt_settingspanel1);
    lv_textarea_set_one_line(wt_ta, true);
    lv_obj_align(wt_ta, LV_ALIGN_TOP_LEFT, 0, 20);
     lv_obj_set_width(wt_ta,300);
     lv_textarea_set_text(wt_ta, api_key.c_str());
     lv_obj_add_event_cb(wt_ta, ta_event_cb, LV_EVENT_ALL, kb);
     lv_obj_add_event_cb(wt_ta, wt_ta_event_cb, LV_EVENT_READY, NULL);

  lv_obj_t  * ui_label_weather_qLocation = lv_label_create(wt_settingspanel1); //создаем объект заголовок
  lv_label_set_text(ui_label_weather_qLocation, "Местоположение:"); //сам текст для надписи
  lv_obj_align(ui_label_weather_qLocation , LV_ALIGN_TOP_LEFT, 0, 70); //положение на экране
 
  wtl_ta = lv_textarea_create(wt_settingspanel1);
    lv_textarea_set_one_line(wtl_ta, true);
    lv_obj_align(wtl_ta, LV_ALIGN_TOP_LEFT, 0, 90);
     lv_obj_set_width(wtl_ta,300);
     lv_textarea_set_text(wtl_ta, qLocation.c_str());
     lv_obj_add_event_cb(wtl_ta, ta_event_cb, LV_EVENT_ALL, kb);
     lv_obj_add_event_cb(wtl_ta, wtl_ta_event_cb, LV_EVENT_READY, NULL);    


  lv_obj_t  * ui_label_weather_interval = lv_label_create(wt_settingspanel1); //создаем объект заголовок
  lv_label_set_text(ui_label_weather_interval, "Интервал обновления (секунды):"); //сам текст для надписи
  lv_obj_align(ui_label_weather_interval , LV_ALIGN_TOP_LEFT, 0, 140); //положение на экране
  //Создаем слайдер изменения обновления
  lv_obj_t * slider_weather_int = lv_slider_create(wt_settingspanel1);
  lv_obj_align(slider_weather_int, LV_ALIGN_TOP_LEFT, 0, 170);
  lv_obj_set_width(slider_weather_int,300);
  lv_slider_set_range(slider_weather_int, 1 , 600);
  lv_slider_set_value(slider_weather_int, refweatherinterval/1000, LV_ANIM_OFF);
  lv_obj_add_event_cb(slider_weather_int, slider_weatherint_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
  weather_int_slider_label = lv_label_create(wt_settingspanel1);
  lv_label_set_text_fmt(weather_int_slider_label, "%d%", (int)lv_slider_get_value(slider_weather_int));
  lv_obj_align_to(weather_int_slider_label, slider_weather_int, LV_ALIGN_CENTER, 0, 0);

  
  
//4 вкладка настроек. Датчик BME 
    lv_obj_t * bme_settingspanel1 = lv_obj_create(settab4);
    lv_obj_set_size(bme_settingspanel1, 340,LV_SIZE_CONTENT);
    
    lv_obj_t  * ui_label_bme_use = lv_label_create(bme_settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_bme_use, "Использовать датчик температуры"); //сам текст для надписи
    lv_obj_align(ui_label_bme_use , LV_ALIGN_TOP_LEFT, 0, 0); //положение на экране

    //Выключатель датчика
    lv_obj_t * bme_switch = lv_switch_create(bme_settingspanel1);
    lv_obj_add_event_cb(bme_switch, bme_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align(bme_switch, LV_ALIGN_TOP_RIGHT, 0, 5); //положение на экране
    if (usesensor)  lv_obj_add_state(bme_switch, LV_STATE_CHECKED);
    lv_obj_set_size(bme_switch,32,16);
    //Интервал обновления
    lv_obj_t  * ui_label_bme_interval = lv_label_create(bme_settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_bme_interval, "Интервал обновления (секунды):"); //сам текст для надписи
    lv_obj_align(ui_label_bme_interval , LV_ALIGN_TOP_LEFT, 0, 30); //положение на экране
    //Создаем слайдер изменения обновления
    lv_obj_t * slider_bme_int = lv_slider_create(bme_settingspanel1);
    lv_obj_align(slider_bme_int, LV_ALIGN_TOP_LEFT, 0, 60);
    lv_obj_set_width(slider_bme_int,300);
    lv_slider_set_range(slider_bme_int, 1 , 60);
    lv_slider_set_value(slider_bme_int, refsensorinterval/1000, LV_ANIM_OFF);
    lv_obj_add_event_cb(slider_bme_int, slider_bmeint_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  
    bme_int_slider_label = lv_label_create(bme_settingspanel1);
    lv_label_set_text_fmt(bme_int_slider_label, "%d%", (int)lv_slider_get_value(slider_bme_int));
    lv_obj_align_to(bme_int_slider_label, slider_bme_int, LV_ALIGN_CENTER, 0, 0);

//5 вкладка настроек sd карта
    lv_obj_t * sd_settingspanel1 = lv_obj_create(settab5);
    lv_obj_set_size(sd_settingspanel1, 340,LV_SIZE_CONTENT);
    //Заголовок панели
    lv_obj_t  * ui_label_set_cat_sdsave = lv_label_create(sd_settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_set_cat_sdsave , "Запись/загрузка настроек с SD карты"); //сам текст для надписи
    lv_obj_align(ui_label_set_cat_sdsave , LV_ALIGN_TOP_MID, 0, 0); //положение на экране
    
    //Кнопки
    //Кнопка сохранения
    lv_obj_t * ui_button_set_sdsave = lv_btn_create(sd_settingspanel1);
    lv_obj_add_event_cb(ui_button_set_sdsave, sd_settings_save_event, LV_EVENT_CLICKED, NULL);
    lv_obj_align(ui_button_set_sdsave , LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_t * ui_button_label_set_sdsave = lv_label_create(ui_button_set_sdsave);
    lv_label_set_text(ui_button_label_set_sdsave, "Сохранить");
    lv_obj_center(ui_button_label_set_sdsave);  

    //кнопка загрузки
    //Кнопка загрузки настроек
    lv_obj_t * ui_button_set_sdload = lv_btn_create(sd_settingspanel1);
    lv_obj_add_event_cb(ui_button_set_sdload, sd_settings_load_event, LV_EVENT_CLICKED, NULL);
    lv_obj_align(ui_button_set_sdload , LV_ALIGN_TOP_MID, 0, 90);
    lv_obj_t * ui_button_label_set_sdload = lv_label_create(ui_button_set_sdload);
    lv_label_set_text(ui_button_label_set_sdload, "Загрузить");
    lv_obj_center(ui_button_label_set_sdload);     
    
    //Панель сохранения плейлиста на sd карту
    lv_obj_t * sd_settingspanel2 = lv_obj_create(settab5);
    lv_obj_set_size(sd_settingspanel2, 340,LV_SIZE_CONTENT);
    lv_obj_set_pos(sd_settingspanel2, 0, 170);
    //Заголовок панели
    lv_obj_t  * ui_label_set_cat_playlist_save = lv_label_create(sd_settingspanel2); //создаем объект заголовок
    lv_label_set_text(ui_label_set_cat_playlist_save , "Сохранение плейлиста на SD карту"); //сам текст для надписи
    lv_obj_align(ui_label_set_cat_playlist_save , LV_ALIGN_TOP_MID, 0, 0); //положение на экране
    //Кнопки
    lv_obj_t * ui_button_set_playlistsave = lv_btn_create(sd_settingspanel2);
    lv_obj_add_event_cb(ui_button_set_playlistsave, sd_settings_playlist_save_event, LV_EVENT_CLICKED, NULL);
    lv_obj_align(ui_button_set_playlistsave , LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_t * ui_button_label_set_playlistsave = lv_label_create(ui_button_set_playlistsave);
    lv_label_set_text(ui_button_label_set_playlistsave, "Сохранить");
    lv_obj_center(ui_button_label_set_playlistsave);

    lv_obj_t * ui_button_set_playlistload = lv_btn_create(sd_settingspanel2);
    lv_obj_add_event_cb(ui_button_set_playlistload, sd_settings_playlist_load_event, LV_EVENT_CLICKED, NULL);
    lv_obj_align(ui_button_set_playlistload , LV_ALIGN_TOP_MID, 0, 90);
    lv_obj_t * ui_button_label_set_playlistload = lv_label_create(ui_button_set_playlistload);
    lv_label_set_text(ui_button_label_set_playlistload, "Загрузить");
    lv_obj_center(ui_button_label_set_playlistload);
//Экран настроек WiFi
  lv_obj_t * wifi_settingspanel1 = lv_obj_create(settab6);
  lv_obj_set_size(wifi_settingspanel1, 340,130);
  //Заголовок панели
    lv_obj_t  * ui_label_set_cat_wifi_set = lv_label_create(wifi_settingspanel1); //создаем объект заголовок
    lv_label_set_text(ui_label_set_cat_wifi_set , "Настройки WiFi"); //сам текст для надписи
    lv_obj_align(ui_label_set_cat_wifi_set , LV_ALIGN_TOP_MID, 0, 0); //положение на экране 

  lv_obj_t  * ui_label_wifi_ssid = lv_label_create(wifi_settingspanel1); //создаем объект заголовок
  lv_label_set_text(ui_label_wifi_ssid, "SSID:"); //сам текст для надписи
  lv_obj_align(ui_label_wifi_ssid , LV_ALIGN_TOP_LEFT, 0, 20); //положение на экране
 
  wifissid_ta = lv_textarea_create(wifi_settingspanel1);
    lv_textarea_set_one_line(wifissid_ta, true);
    lv_obj_align(wifissid_ta, LV_ALIGN_TOP_LEFT, 0, 40);
     lv_obj_set_width(wifissid_ta,140);
     lv_textarea_set_text(wifissid_ta, SSID.c_str());
     lv_obj_add_event_cb(wifissid_ta, ta_event_cb, LV_EVENT_ALL, kb);
     lv_obj_add_event_cb(wifissid_ta, wifissid_ta_event_cb, LV_EVENT_READY, NULL);

  lv_obj_t  * ui_label_wifi_pass = lv_label_create(wifi_settingspanel1); //создаем объект заголовок
  lv_label_set_text(ui_label_wifi_pass, "Пароль:"); //сам текст для надписи
  lv_obj_align(ui_label_wifi_pass , LV_ALIGN_TOP_LEFT, 155, 20); //положение на экране
 
  wifipass_ta = lv_textarea_create(wifi_settingspanel1);
    lv_textarea_set_one_line(wifipass_ta, true);
    lv_obj_align(wifipass_ta, LV_ALIGN_TOP_LEFT, 155, 40);
     lv_obj_set_width(wifipass_ta,150);
     lv_textarea_set_text(wifipass_ta, PASS.c_str());
     lv_obj_add_event_cb(wifipass_ta, ta_event_cb, LV_EVENT_ALL, kb);
     lv_obj_add_event_cb(wifipass_ta, wifipass_ta_event_cb, LV_EVENT_READY, NULL);    
     lv_textarea_set_password_mode(wifipass_ta, true);

  //Заголовок панели
    lv_obj_t * wifi_settingspanel2 = lv_obj_create(settab6);
    lv_obj_set_size(wifi_settingspanel2, 340,LV_SIZE_CONTENT);
    lv_obj_set_pos(wifi_settingspanel2, 0, 140);
    //Заголовок панели
    lv_obj_t  * ui_label_set_cat_wifi_inf = lv_label_create(wifi_settingspanel2); //создаем объект заголовок
    lv_label_set_text(ui_label_set_cat_wifi_inf , "Информация WiFi"); //сам текст для надписи
    lv_obj_align(ui_label_set_cat_wifi_inf , LV_ALIGN_TOP_MID, 0, 0); //положение на экране 
    wifitable = lv_table_create(wifi_settingspanel2);
    lv_obj_set_width(wifitable, 300);
    lv_obj_add_event_cb(wifitable, draw_table_part_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);
        lv_obj_set_pos(wifitable, 0, 20);
        lv_table_set_col_cnt(wifitable, 2);
        lv_table_set_col_width(wifitable, 0, 100);
        lv_table_set_col_width(wifitable, 1, 200);
        lv_table_set_cell_value(wifitable, 0, 0, "Параметр");
        lv_table_set_cell_value(wifitable, 0, 1, "Значение");
        lv_table_set_cell_value(wifitable, 1, 0, "Подключение");
        lv_table_set_cell_value(wifitable, 2, 0, "SSID");
        lv_table_set_cell_value(wifitable, 3, 0, "IP");
        lv_table_set_cell_value(wifitable, 4, 0, "MAC");
        lv_table_set_cell_value(wifitable, 5, 0, "Хост");
        lv_table_set_cell_value(wifitable, 6, 0, "Сигнал");
        lv_table_set_cell_value(wifitable, 7, 0, "Шлюз");
  
}


void setup()
{ 
  Serial.begin( 115200 ); //открытие серийного порта
      touch_init(); //иницилизация тача 
      lv_init();//инициализация LVGL
  
  //инициализация дисплея в библиотеке TFT_ESPi и изменение его ориентации на альбомную
      tft.begin();          
      tft.setRotation(1);
 
  //Инициализация файловой системы
  if(!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)){
        Serial.println("SPIFFS Mount Failed");
        Serial.println("Formatting SPIFFS...");
        bool formatted = SPIFFS.format();
        if(formatted){
        Serial.println("\n\nSuccess formatting");
        }else{
       Serial.println("\n\nError formatting");
        }
        return;
    }
    else 
      {
        Serial.println("SPIFFS ready!");
        Serial.println("Reading config file...");
        loadConfiguration(filename);
        listDir(SPIFFS, "/", 0);
        Serial.println("Loading playlist info...");
        un=playlistnumtrack(SPIFFS,"/playlist.txt");
        if (un>0) 
          {
            Serial.printf("Found %d stations \n",un);
            sn=1;
            url=playlistread(SPIFFS,"/playlist.txt",sn);
            Serial.printf("Current track: %s \n",url);
          }
          else
          {
             Serial.println("Stations not found");
            un=1;
            sn=1;
            url="http://vladfm.ru:8000/vfm*Владивосток FM";
            Serial.printf("Playing default station: %s \n",url);
          }
        }  

 // Инициализация SD карты
      pinMode(5, OUTPUT);
      digitalWrite(5, HIGH);
      SDSPI.begin(18, 19, 23); // SDSPI.begin(SCLK, MISO, MOSI);
      SDSPI.setFrequency(1000000);
      if(!SD.begin(5,SDSPI)){
        Serial.println("Card Mount Failed");
      }
      else
      {
      uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

   
  //createDir(SD, "/mydir");
  listDir(SD, "/", 0);
  if (SD.exists("/playlist.txt")) 
    {
      sdLoadConf(SD, "/playlist.txt", "/playlist.txt");
      SD.remove("/playlist.txt");
      listDir(SPIFFS, "/", 0);}
  //writeFile(SD, "/hello.txt", "Hello ");
  //appendFile(SD, "/hello.txt", "World!\n");
  //readFile(SD, "/hello.txt");
  //testFileIO(SD, "/test.txt");
  //sdSaveConf(SD, "/confbackup.txt");
  SD.end();
      }        
      
     

  //выводим интерфейс LVGL на экран        
      Serial.println("lvgl start");
      lvlg_create();
Serial.println("wifi start");
//подключение к WiFi
WiFi.disconnect(true);
delay(1000);

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  uint8_t tries = 11;
  Serial.println();
  Serial.printf("Connecting to %s ", SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);
  while (--tries && WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
  
  Serial.println("ntp start");
//запуск сервиса синхронизации времени
uint8_t numtries=10;
bool ntpstart=false;
 while (numtries>0 && ntpstart==false)
 {
ntp.setGMT(gmt);
bool ntpstart=ntp.begin();
Serial.println(ntp.status());
numtries--;
 }
 //ntp.setHost(ntpserver);
Serial.println("calendar start");
//Устанавливаем календарям актуальные данные
set_calendars_date();
getweather();
}

Serial.println("timers start");
//Устанавливаем интервалы таймеров
  reftime.setInterval(1000);//обновление времени на экране 1000 мс или 1 секунда
  refpc.setInterval(refpcinterval); //Обновление парметров ПК 3 секунды
  //refyandex.setInterval(refyandexinterval);//получение данных о доходе с рекламы 10 минут
  reflvgl.setInterval(50);//обновление экрана LVGL 50 мс
  refbright.setInterval(1000);//обновление показаний фоторезистора
  refweather.setInterval(refweatherinterval);//Погода 5 минут
  refsensor.setInterval(refsensorinterval);//обновление показаний сенсора
Serial.println("backlight start");
//Настройка подсветки экрана
  pinMode(TFT_BACKLIGHT,OUTPUT);//Переключаем пин подсветки на передачу данных
  ledcSetup(0, 250, 8); //инициализируем управление подсветкой (канал 0-15, частота шим 250 Гц, разрешение 0-256)
  ledcAttachPin(TFT_BACKLIGHT, 0); //подключаем пин подсветки к каналу 0
  ledcWrite(0, bright_level); //устанавливаем значение подсветки по умолчанию 250
Serial.println("rgb start");
//Настройка RGB светодиодов
  pinMode(RED_PIN,OUTPUT);//Переключаем пин красного на передачу данных
  pinMode(GREEN_PIN,OUTPUT);//Переключаем пин зеленого на передачу данных
  pinMode(BLUE_PIN,OUTPUT);//Переключаем пин синего на передачу данных
  //Инициализируем каналы управления
  ledcSetup(1, 2000, 8); //инициализируем управление светодиодами (канал 1, частота шим 2000 Гц, разрешение 0-256)
  ledcSetup(2, 2000, 8);//инициализируем управление светодиодами (канал 2, частота шим 2000 Гц, разрешение 0-256)
  ledcSetup(3, 2000, 8);//инициализируем управление светодиодами (канал 3, частота шим 2000 Гц, разрешение 0-256)
  //Подключаем пины
  ledcAttachPin(RED_PIN, 1); //подключаем пин красного к каналу 1
  ledcAttachPin(GREEN_PIN, 2); //подключаем пин зеленого к каналу 2
  ledcAttachPin(BLUE_PIN, 3); //подключаем пин синего к каналу 3
  ledcWrite(1, 255); //устанавливаем значение подсветки по умолчанию 250
  ledcWrite(2, 255); //устанавливаем значение подсветки по умолчанию 250
  ledcWrite(3, 255); //устанавливаем значение подсветки по умолчанию 250

//датчик температуры
  if  (usesensor)
  {
  Serial.println("i2c start");
  Wire.begin(21,22);
  bool status = bme.begin(0x77, &Wire);  
  if (!status) {
    Serial.println("Could not find a valid BME280_2 sensor, check wiring!");
    while (1);
  }
  // Set up oversampling and filter initialization
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150); // 320*C for 150 ms
  }
Serial.println("photo start");  
// настройка фоторезистора
  pinMode(PHOTO_PIN, ANALOG);

  
}


//Установка актуальной даты на календаре
  void set_calendars_date()
  {
  bool ntpsync=false;
  uint8_t tries=10;
  while (ntpsync==false && tries>0)
  {
  ntp.updateNow();
  ntpsync=ntp.synced();
  Serial.println(ntpsync);
  tries--;
  }
  lv_calendar_set_today_date(calendar, ntp.year(), ntp.month(), ntp.day());
  lv_calendar_set_showed_date(calendar, ntp.year(), ntp.month());
  lastday=ntp.day(); //присваиваем переменной lastday значение текущего дня
    static lv_calendar_date_t highlighted_days[49]; //массив дат праздников

    //Новый год
    highlighted_days[0].year = ntp.year();
    highlighted_days[0].month = 1;
    highlighted_days[0].day = 1;
    //Новогодние каникулы
    highlighted_days[1].year = ntp.year();
    highlighted_days[1].month = 01;
    highlighted_days[1].day = 2;

    highlighted_days[2].year = ntp.year();
    highlighted_days[2].month = 01;
    highlighted_days[2].day = 3;

    highlighted_days[3].year = ntp.year();
    highlighted_days[3].month = 01;
    highlighted_days[3].day = 4;

    highlighted_days[4].year = ntp.year();
    highlighted_days[4].month = 01;
    highlighted_days[4].day = 5;
    //Православный сочельник
    highlighted_days[5].year = ntp.year();
    highlighted_days[5].month = 01;
    highlighted_days[5].day = 6;

    //Рождество Христово
    highlighted_days[6].year = ntp.year();
    highlighted_days[6].month = 01;
    highlighted_days[6].day = 7;
    
    //День Генерального прокурора
    highlighted_days[7].year = ntp.year();
    highlighted_days[7].month = 01;
    highlighted_days[7].day = 12;
    
    //Старый новый год
    highlighted_days[8].year = ntp.year();
    highlighted_days[8].month = 01;
    highlighted_days[8].day = 14;
    
    //Татьянин день
    highlighted_days[9].year = ntp.year();
    highlighted_days[9].month = 01;
    highlighted_days[9].day = 25;
    
    //День Святого Валентина
    highlighted_days[10].year = ntp.year();
    highlighted_days[10].month = 02;
    highlighted_days[10].day = 14;

    //День памяти о международных обязанностях
    highlighted_days[11].year = ntp.year();
    highlighted_days[11].month = 02;
    highlighted_days[11].day = 15;
    
    //День защитника отечества
    highlighted_days[12].year = ntp.year();
    highlighted_days[12].month = 02;
    highlighted_days[12].day = 23;

    //День сил специальных операций
    highlighted_days[13].year = ntp.year();
    highlighted_days[13].month = 02;
    highlighted_days[13].day = 27;
    
    //Международный женский день
    highlighted_days[14].year = ntp.year();
    highlighted_days[14].month = 03;
    highlighted_days[14].day = 8;
    
    //Мартовское равноденствие
    highlighted_days[15].year = ntp.year();
    highlighted_days[15].month = 03;
    highlighted_days[15].day = 20;
    
    //День работников культуры
    highlighted_days[16].year = ntp.year();
    highlighted_days[16].month = 03;
    highlighted_days[16].day = 25;
    
    //Национальный день донора
    highlighted_days[17].year = ntp.year();
    highlighted_days[17].month = 04;
    highlighted_days[17].day = 22;
    

    //День памяти о радиационных авариях и катастрофах
    highlighted_days[18].year = ntp.year();
    highlighted_days[18].month = 04;
    highlighted_days[18].day = 26;
    
    //День весны и труда
    highlighted_days[19].year = ntp.year();
    highlighted_days[19].month = 05;
    highlighted_days[19].day = 1;

    //День Победы
    highlighted_days[20].year = ntp.year();
    highlighted_days[20].month = 05;
    highlighted_days[20].day = 9;

    //День пограничника
    highlighted_days[21].year = ntp.year();
    highlighted_days[21].month = 05;
    highlighted_days[21].day = 28;
    
    //День защиты детей
    highlighted_days[22].year = ntp.year();
    highlighted_days[22].month = 06;
    highlighted_days[22].day = 1;

    //День русского языка
    highlighted_days[23].year = ntp.year();
    highlighted_days[23].month = 06;
    highlighted_days[23].day = 6;

    //День социальных работников
    highlighted_days[24].year = ntp.year();
    highlighted_days[24].month = 06;
    highlighted_days[24].day = 8;
    
    //День России
    highlighted_days[25].year = ntp.year();
    highlighted_days[25].month = 06;
    highlighted_days[25].day = 12;

    //Всемирный день Донора
    highlighted_days[26].year = ntp.year();
    highlighted_days[26].month = 06;
    highlighted_days[26].day = 14;
    
    //Июньское солнцестояние
    highlighted_days[27].year = ntp.year();
    highlighted_days[27].month = 06;
    highlighted_days[27].day = 21;
    
    //День военно-морского флота
    highlighted_days[28].year = ntp.year();
    highlighted_days[28].month = 07;
    highlighted_days[28].day = 30;

    //День ВДВ
    highlighted_days[29].year = ntp.year();
    highlighted_days[29].month = 8;
    highlighted_days[29].day = 2;

    //День железнодорожника
    highlighted_days[30].year = ntp.year();
    highlighted_days[30].month = 8;
    highlighted_days[30].day = 25;
    
    //День ВВС
    highlighted_days[31].year = ntp.year();
    highlighted_days[31].month = 8;
    highlighted_days[31].day = 12;

    //День Государственного флага
    highlighted_days[32].year = ntp.year();
    highlighted_days[32].month = 8;
    highlighted_days[32].day = 22;

    //День фильмов и кино
    highlighted_days[33].year = ntp.year();
    highlighted_days[33].month = 8;
    highlighted_days[33].day = 27;
    
    //День знаний
    highlighted_days[34].year = ntp.year();
    highlighted_days[34].month = 9;
    highlighted_days[34].day = 1;

    //Сентябрьское равноденствие
    highlighted_days[35].year = ntp.year();
    highlighted_days[35].month = 9;
    highlighted_days[35].day = 23;
    
    //День машиностроителя
    highlighted_days[36].year = ntp.year();
    highlighted_days[36].month = 9;
    highlighted_days[36].day = 29;
    
    //День воздушно-космической обороны России
    highlighted_days[37].year = ntp.year();
    highlighted_days[37].month = 10;
    highlighted_days[37].day = 4;
    
    //День спецназа
    highlighted_days[38].year = ntp.year();
    highlighted_days[38].month = 10;
    highlighted_days[38].day = 24;

    //День таможенника
    highlighted_days[39].year = ntp.year();
    highlighted_days[39].month = 10;
    highlighted_days[39].day = 25;

    //День народного единства
    highlighted_days[40].year = ntp.year();
    highlighted_days[40].month = 11;
    highlighted_days[40].day = 4;
    
    //День полиции
    highlighted_days[41].year = ntp.year();
    highlighted_days[41].month = 11;
    highlighted_days[41].day = 10;

    //День ракетных войск и артиллерии
    highlighted_days[42].year = ntp.year();
    highlighted_days[42].month = 11;
    highlighted_days[42].day = 18;

    //День матери
    highlighted_days[43].year = ntp.year();
    highlighted_days[43].month = 11;
    highlighted_days[43].day = 26;
    
    //День морской пехоты
    highlighted_days[44].year = ntp.year();
    highlighted_days[44].month = 11;
    highlighted_days[44].day = 27;

    //День героев Отечества
    highlighted_days[45].year = ntp.year();
    highlighted_days[45].month = 12;
    highlighted_days[45].day = 9;
    
    //День стратегических ракетных войск
    highlighted_days[46].year = ntp.year();
    highlighted_days[46].month = 12;
    highlighted_days[46].day = 17;
    
    //Декабрьское солнцестояние
    highlighted_days[47].year = ntp.year();
    highlighted_days[47].month = 12;
    highlighted_days[47].day = 22;
    
    //Канун нового года
    highlighted_days[48].year = ntp.year();
    highlighted_days[48].month = 12;
    highlighted_days[48].day = 31;

  //Массив праздников
  char *celeb[49]={"Новый год",
  "Второй день Нового года",
  "Третий день Нового года",
  "Четвертый день Нового года",
  "Пятый день Нового года",
  "Православный сочельник",
  "Православное рождество",
  "День Генерального прокурора",
  "Старый Новый год",
  "Татьянин день",
  "День святого Валентина",
  "День памяти о международных обязанностях",
  "День защитника отечества",
  "День сил специальных операций",
  "Международный женский день",
  "Мартовское равноденствие",
  "День работников культуры",
  "Национальный день донора крови",
  "День памяти о радиационных авариях и катастрофах",
  "День весны и труда",
  "День Победы",
  "День пограничника",
  "День защиты детей",
  "День русского языка",
  "День социальных работников",
  "День России",
  "Всемирный день донора крови",
  "Июньское солнцестояние",
  "День военно-морского флота",
  "День ВДВ",
  "День работников железнодорожника",
  "День спорта",
  "День военно-воздушных сил России",
  "День государсвенного флага",
  "День фильмов и кино",
  "Сентябрьское равноденствие",
  "День машиностроителя",
  "День воздушно-космической обороны России",
  "День спецназа",
  "День таможенника",
  "День народного единства",
  "День полиции",
  "День ракетных войск и артиллерии",
  "День матери",
  "День морской пехоты",
  "День героев Отечества",
  "День стратегических ракетных войск",
  "Декабрьское солнцестояние",
  "Канун Нового года"
  };

  lv_calendar_set_highlighted_dates(calendar, highlighted_days, 46);
  for (byte i=0;i<49;i++)
            {
              if (ntp.month()==highlighted_days[i].month && ntp.day()==highlighted_days[i].day)
              {
                lv_label_set_text_fmt(statusbox, "Сегодня празднуем: %s",celeb[i]);
                break;
              }
            }
  
    
}

//События WiFi
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  IPAddress ip = WiFi.localIP();
  String ipString =String(ip[0]);
  for (byte octet = 1; octet < 4; ++octet) {
    ipString += '.' + String(ip[octet]);
  }
  lv_label_set_text_fmt(wifistatus, "%s "LV_SYMBOL_WIFI, ipString.c_str());
  Serial.println("IP address: ");
  Serial.println(ip);
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(SSID, PASS);
}

void loop()
{
  static int lastms = 0;
  ntp.tick();//внутренний таймер NTP сервиса
  audio.loop();
  //далее проверяем таймеры на срабатывание
    
    if (reflvgl.isReady())
       {
        lv_timer_handler();
        }
     if (refbright.isReady() && photosensor==true) 
        {
          int analog_value = analogRead(PHOTO_PIN); //читаем аналогове значение с фоторезистора
          //Serial.println(analog_value);
          bright_level = map(analog_value, 0, 1088, 255, 20); //преобразовываем его в значение яркости подсветки
          //Serial.println(bright_level);
          ledcWrite(0, bright_level); //устанавливаем значение подсветки 
        }
    if (refsensor.isReady() && lv_tabview_get_tab_act(tabview)==0 && usesensor==true)
    {
  
  if (!bme.endReading()) {
  Serial.println(F("Failed to complete reading :("));
  }
  else
  {
  lv_label_set_text_fmt(roomtemp,LV_SYMBOL_TEMP"%.1f°С",bme.temperature);
  lv_label_set_text_fmt(roomhumid, LV_SYMBOL_HUMIDITY"%.1f%",bme.humidity);
  lv_label_set_text_fmt(roompress,LV_SYMBOL_PRESSURE"%dмм рт. ст.",bme.pressure/133,3);
  int airquality=bme.gas_resistance / 1000;
  if (airquality<51) {lv_label_set_text_fmt(roomair,"Качество воздуха: Хорошее",airquality);if (ledindicator){ledcWrite(1, 255);ledcWrite(2, 128);ledcWrite(3, 255);}}
  if (airquality>50 && airquality<101) {lv_label_set_text_fmt(roomair,"Качество воздуха: Среднее",airquality);if (ledindicator){ledcWrite(1, 255);ledcWrite(2, 200);ledcWrite(3, 255);}} 
  if (airquality>100 && airquality<151) {lv_label_set_text_fmt(roomair,"Качество воздуха: Ниже среднего",airquality);if (ledindicator){ledcWrite(1, 225);ledcWrite(2, 225);ledcWrite(3, 255);}} 
  if (airquality>150 && airquality<201) {lv_label_set_text_fmt(roomair,"Качество воздуха: Плохое",airquality);if (ledindicator){ledcWrite(1, 225);ledcWrite(2, 165);ledcWrite(3, 255);}} 
  if (airquality>200 && airquality<301) {lv_label_set_text_fmt(roomair,"Качество воздуха: Очень плохое",airquality);if (ledindicator){ledcWrite(1, 200);ledcWrite(2, 255);ledcWrite(3, 255);}} 
  if (airquality>300) {lv_label_set_text_fmt(roomair,"Качество воздуха: Опасное",airquality);if (ledindicator){ledcWrite(1, 100);ledcWrite(2, 255);ledcWrite(3, 255);}} 
  lv_bar_set_value(roomair_bar,airquality/10, LV_ANIM_ON);
  lv_label_set_text_fmt(roomair_bar_label,"%d",airquality);
  }
    }        
  if (WiFi.status() == WL_CONNECTED)
{ 
   lv_obj_clear_flag(wifistatus, LV_OBJ_FLAG_HIDDEN);
   

     if (refvisualiser.isReady())
     {
       lv_chart_set_value_by_id(radio_visualiser, radio_visualiser_series, lv_rand(0,33), lv_rand(20, 100));
     }
    
  if (reftime.isReady()) {
        lv_label_set_text(ui_status_clock, ntp.timeString().c_str());
        if (lv_tabview_get_tab_act(tabview)==0)
        {
        //обновление времени по NTP
        lv_label_set_text(displayclock, ntp.timeString().c_str());
        if (lastday!=ntp.day())
        {
          set_calendars_date();
        }      
        }
  }
   
   if (refpc.isReady() and lv_tabview_get_tab_act(tabview)==1)
   {
        Serial.println("Reading hardware");
        if (WiFi.status() == WL_CONNECTED) hardwareMonitor(); 
  }
    if (refweather.isReady())
   {
        if (WiFi.status() == WL_CONNECTED) getweather(); 
  }
  
} else
{
 lv_obj_add_flag(wifistatus, LV_OBJ_FLAG_HIDDEN);  
}
}
