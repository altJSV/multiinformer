void getweather(){
  //Настройки API для получения погоды
  const String api_1 = "http://api.openweathermap.org/data/2.5/weather?q=";
const String api_2 = "&units=metric&lang=ru"; //температура в градусах цельсия и русский язык
const String api_3 = "&APPID=";


  WiFiClient client;
  HTTPClient http;
  http.begin(client,api_1 + qLocation + api_2 + api_3 + api_key);
  int httpCode = http.GET();

  if (httpCode > 0) { 

    String response = http.getString();
    Serial.println(response);
    DynamicJsonDocument owm(2048);
        // Deserialize the JSON document
    DeserializationError error = deserializeJson(owm, response);

    // Test if parsing succeeds.
    if (error) {
      String errorStr = error.c_str();
      Serial.print ("weather parsing: ");
      Serial.println(errorStr);
    } else 
    {
      //парсинг значений и вывод на экран
      Serial.println("start parsing");
float coord_lon = owm["coord"]["lon"]; // 37.5458
float coord_lat = owm["coord"]["lat"]; // 51.2817

JsonObject weather_0 = owm["weather"][0];
int weather_0_id = weather_0["id"]; // 500
String weather_0_main = weather_0["main"]; // "Rain"
const char* weather_0_description = weather_0["description"]; // "light rain"
const char* base = owm["base"]; // "stations"

JsonObject main = owm["main"];
int main_temp = main["temp"]; // 7.12
int main_feels_like = main["feels_like"]; // 4.73
int main_temp_min = main["temp_min"]; // 7.12
int main_temp_max = main["temp_max"]; // 7.12
int main_pressure = main["pressure"]; // 1014
int main_pressure_mm = main_pressure/1.333;
int main_humidity = main["humidity"]; // 90
const char* main_sea_level = main["sea_level"]; // 1014
const char* main_grnd_level = main["grnd_level"]; // 994

int visibility = owm["visibility"]; // 10000

JsonObject wind = owm["wind"];
float wind_speed = wind["speed"]; // 3.51
int wind_deg = wind["deg"]; // 331
float wind_gust =wind["gust"]; // 7

float rain_1h = owm["rain"]["1h"]; // 0.3

int clouds_all = owm["clouds"]["all"]; // 100

long dt = owm["dt"]; // 1682943106

JsonObject sys = owm["sys"];
const char* sys_country = sys["country"]; // "RU"
long sys_sunrise = sys["sunrise"]; // 1682906591
long sys_sunset = sys["sunset"]; // 1682959834

const char* name = owm["name"]; // "Gubkin"

//вывод на экран
lv_label_set_text(ui_weatherdesc, weather_0_description);
lv_label_set_text(weathercity, name);
lv_label_set_text_fmt(ui_visiblity,LV_SYMBOL_EYE_OPEN"%d",visibility);
lv_label_set_text_fmt(ui_weathertemp,LV_SYMBOL_TEMP"%d°С",main_temp);
lv_label_set_text_fmt(ui_avgtemp, "Мин:%d°С Макс:%d°С",main_temp_min, main_temp_max);
lv_label_set_text_fmt(ui_feeltemp, "Ощущается как: %d°С",main_feels_like);
lv_label_set_text_fmt(ui_windspeed, "%.2fм/с",wind_speed);
lv_label_set_text_fmt(ui_windgust, "Порывы: %.2fм/сек",wind_gust);
lv_label_set_text_fmt(ui_humid, LV_SYMBOL_HUMIDITY"%d%",main_humidity);
lv_label_set_text_fmt(ui_pressure,LV_SYMBOL_PRESSURE"%dмм",main_pressure_mm);
lv_label_set_text_fmt(ui_sunset,LV_SYMBOL_SUNSET"%s", getDateTime(sys_sunset));
lv_label_set_text_fmt(ui_sunrise,LV_SYMBOL_SUNRISE"%s", getDateTime(sys_sunrise));
lv_chart_set_next_value(ui_pressurechart, ui_prser, main_pressure_mm);
lv_chart_set_next_value(ui_tempchart, ui_tempser, main_temp);
lv_chart_set_next_value(ui_humidchart, ui_huser, main_humidity);
lv_meter_set_indicator_end_value(windmeter, wind_indic, wind_deg/10);

if (usesensor==false)
  {
    lv_label_set_text_fmt(roomtemp,"OWM:"LV_SYMBOL_TEMP"%d°С",main_temp);
    lv_label_set_text_fmt(roomhumid,"OWM:" LV_SYMBOL_HUMIDITY"%d%",main_humidity);
    lv_label_set_text_fmt(roompress,"OWM:"LV_SYMBOL_PRESSURE"%dмм",main_pressure_mm);
  }

if (weather_0_main=="Clouds")
    {
      lv_img_set_src(ui_weatherimage, &clouds100);
      lv_img_set_src(ui_weatherimage_main, &clouds100);
    }
if (weather_0_main=="Rain")
    {
      lv_img_set_src(ui_weatherimage, &rain100);
      lv_img_set_src(ui_weatherimage_main, &rain100);
    }
if (weather_0_main=="Thunderstorm")
    {
      lv_img_set_src(ui_weatherimage, &storm100);
      lv_img_set_src(ui_weatherimage_main, &storm100);
    }
if (weather_0_main=="Clear")
    {
      lv_img_set_src(ui_weatherimage, &sun100);
      lv_img_set_src(ui_weatherimage_main, &sun100);
    }
if (weather_0_main=="Drizzle")
    {
      lv_img_set_src(ui_weatherimage, &drizzle100);
      lv_img_set_src(ui_weatherimage_main, &drizzle100);
    }
if (weather_0_main=="Snow")
    {
      lv_img_set_src(ui_weatherimage, &snow100);
      lv_img_set_src(ui_weatherimage_main, &snow100);
    }
if (weather_0_main=="Rain")
    {
      lv_img_set_src(ui_weatherimage, &rain100);
      lv_img_set_src(ui_weatherimage_main, &rain100);
    }            


    }
    }
    else
    {
    Serial.println("http.GET() == 0");
  }
  
  http.end();   //Close connection
}

String getDateTime(uint32_t t) {
        // http://howardhinnant.github.io/date_algorithms.html#civil_from_days
        int8_t gmt=3; //часовой пояс
        t += gmt * 3600ul;
        t /= 60ul;
        int8_t minute = t % 60ul;
        t /= 60ul;
        int8_t hour = t % 24ul;
        if (minute>9) {return(String(hour)+":"+String(minute));} else {return(String(hour)+":0"+String(minute));}
        return(String(hour)+":"+String(minute));
}
