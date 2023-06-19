void  cur_check()
{
String payload;
WiFiClient client;
HTTPClient https;
  ledcWrite(1, 255); //устанавливаем значение подсветки по умолчанию 250
  ledcWrite(2, 255); //устанавливаем значение подсветки по умолчанию 250
  ledcWrite(3, 255); //устанавливаем значение подсветки по умолчанию 250
lv_table_set_cell_value(cur_table, 0, 0, "Валюта");
lv_table_set_cell_value(cur_table, 0, 1, "Курс");
//https.useHTTP10(true);
https.begin(client,"http://iss.moex.com/iss/statistics/engines/futures/markets/indicativerates/securities.json");
int httpResponseCode = https.GET();  // Запрашиваем информацию у сервера
payload = https.getString();
    Serial.println(payload);
//Проверяем получили ли мы ответ от сервера
if (httpResponseCode > 0) {
    //Ошибок не обнаружено. Получаем строку с курсами валют для дальнейшей работы
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    //Зажигаем зеленый светодиод при удачном выполнении запроса
   if (ledindicator && ntp.hour()>=daybegin && ntp.hour()<=dayend)
      {
        ledcWrite(1, 255);
        ledcWrite(2, 200);
        ledcWrite(3, 255);
      }
    //Парсинг JSON
    StaticJsonDocument<48> filter;
filter["securities"]["data"][0] = true;

StaticJsonDocument<1024> doc;

DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
  //Зажигаем красный светодиод при ошибке
  if (ledindicator && ntp.hour()>=daybegin && ntp.hour()<=dayend)
  {
  ledcWrite(1, 200);
  ledcWrite(2, 255);
  ledcWrite(3, 255);
  }
}
else
{ 
  //Зажигаем синий светодиод при удачном парсинге JSON
  if (ledindicator && ntp.hour()>=daybegin && ntp.hour()<=dayend)
  {
  ledcWrite(1, 255);
  ledcWrite(2, 255);
  ledcWrite(3, 200);
  }
JsonArray securities_data = doc["securities"]["data"];
for (byte i=0;i<9;i++)
{
JsonArray curdata = securities_data[i];
String name = curdata[2]; // "CAD/RUB"
String val = curdata[3];
lv_table_set_cell_value(cur_table, i+1, 0, name.c_str());
lv_table_set_cell_value(cur_table, i+1, 1, val.c_str());
}
}
    }
  else {
    //Данные не получены. Выводим в консоль код ошибки
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
    //Зажигаем красный светодиод при ошибке
    if (ledindicator && ntp.hour()>=daybegin && ntp.hour()<=dayend)
    {
    ledcWrite(1, 200);
    ledcWrite(2, 255);
    ledcWrite(3, 255);
    }
  }
  https.end();                         // Освобождаем ресурсы и закрываем соединение
}
