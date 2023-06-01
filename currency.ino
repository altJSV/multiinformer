void  cur_check()
{
String payload;
WiFiClient client;
HTTPClient https;
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
    
    //Парсинг JSON
    StaticJsonDocument<48> filter;
filter["securities"]["data"][0] = true;

StaticJsonDocument<1024> doc;

DeserializationError error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));

if (error) {
  Serial.print("deserializeJson() failed: ");
  Serial.println(error.c_str());
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
  else {
    //Данные не получены. Выводим в консоль код ошибки
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  https.end();                         // Освобождаем ресурсы и закрываем соединение
}
