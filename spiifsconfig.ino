void loadConfiguration(const char *filename) {
  // Open file for reading
  File file = LittleFS.open(filename);
  if(!file){
        Serial.println("Failed to open file");
        return;
        }
  //выделяем память под json
  StaticJsonDocument<1024> doc1;

  // читаем json
  DeserializationError error = deserializeJson(doc1, file);
  if (error) {
              Serial.println(F("Failed to read file, using default configuration"));
              file.close();
              return;
              }

  // копируем значения 
  const char * jspc_server_path = doc1["pcadress"] ;
  //const char * jstoken = doc1["yatoken"];
  const char * jsapi_key= doc1["weatherapi"];
  const char * jsqLocation= doc1["weathercity"];
  const char * jsSSID=doc1["ssid"];
  const char * jsPASS=doc1["pass"];
  usesensor= (bool)doc1["usesensor"];
  gmt= (int8_t)doc1["gmt"];
  refpcinterval= (uint32_t)doc1["refpcinterval"];
  refweatherinterval= (uint32_t)doc1["refweatherinterval"];
  refsensorinterval= (uint32_t)doc1["refsensorinterval"];
  bright_level = (uint8_t)doc1["bright_level"];
  daybegin=(uint8_t)doc1["day_begin"];
  dayend=(uint8_t)doc1["day_end"];
  photosensor= (bool)doc1["photosensor"];
  pc_server_path = jspc_server_path;
  ledindicator = (bool)doc1["ledindicator"];
  api_key = jsapi_key;
  qLocation  = jsqLocation;
  SSID=jsSSID;
  PASS=jsPASS;
  // Закрываем файл
  file.close();
}

// сохраняем настройки в файл
void saveConfiguration(const char *filename) {
  // Delete existing file, otherwise the configuration is appended to the file
  LittleFS.remove(filename);

  // Открываем файл для чтения
  File file = LittleFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  //Выделяем память под JSON
  StaticJsonDocument<1024> doc1;

  // Set the values in the document
  doc1["pcadress"] = pc_server_path; //адрес сервера пк монитора
  doc1["refpcinterval"] = refpcinterval; //период обновления пк монитора
  doc1["weatherapi"] = api_key;//api ключ погоды
  doc1["weathercity"] = qLocation; //Местоположение
  doc1["refweatherinterval"] = refweatherinterval;//период обновления погоды
  doc1["gmt"] = gmt; //часовой пояс
  doc1["ledindicator"] = ledindicator;//состояние led индикатора
  doc1["day_begin"] = daybegin;//начало дневного времени
  doc1["day_end"] = dayend;//конец дневного времени
  doc1["usesensor"] = usesensor; //использование сенсора bme
  doc1["refsensorinterval"] = refsensorinterval; //период обновления сенсора
  doc1["bright_level"] = bright_level;// яркость подсветки экрана
  doc1["ssid"] = SSID; //часовой пояс
  doc1["pass"] = PASS;//состояние led индикатора
  doc1["photosensor"] = photosensor; //автоматическая регулировка яркости подсветки
  // Serialize JSON to file
  if (serializeJson(doc1, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  else
  {
    Serial.println("Config file saved!");
  }
  // Закрываем файл
  file.close();
}

//Чтение строки из плейлиста
String playlistread(fs::FS &fs, const char * path, uint16_t line) {
  File file = fs.open(path, "r");
  if (!file)  return F("Failed to open file for reading");

  while (file.available() && line > 1) {//Пропускаем лишние строки в количестве line-1.
    line -= file.read() == '\n';
  }; 
  
  String read_text;
  char last_simbol;
  while (file.available() && (last_simbol != '\n')) {//Читаем строку
    last_simbol = file.read();
    read_text += last_simbol;
  };
  file.close();
  return read_text;
}

//Определение колличества строк в плейлисте
uint8_t playlistnumtrack(fs::FS &fs, const char * path) {
  File file = fs.open(path, "r");
  if (!file)  return 0;
  uint8_t count=0;
  while (file.available()) {//Пропускаем лишние строки в количестве line-1.
    count += file.read() == '\n';
  };
  file.close();
  return count;
}

//сохранение конфигурации радио
void saveRadioConf(const char *filename) {
  LittleFS.remove(filename);
  // Открываем файл для чтения
  File file = LittleFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }
  StaticJsonDocument<128> doc1;
  // записываем значения в json
  doc1["station"] = sn; //номер станции
  doc1["vol"] = vol; //громкость
  
  // сохраняем на диск
  if (serializeJson(doc1, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  else
  {
    Serial.println("Radio Config file saved!");
  }
  // Закрываем файл
  file.close();
}

//Загрузка конфигурации радио
void loadRadioConf(const char *filename) {
  // Открываем файл для чтения
  File file = LittleFS.open(filename);
  if(!file){
        Serial.println("Failed to open file");
        return;
        }
  //читаем json
  StaticJsonDocument<128> doc1;
  DeserializationError error = deserializeJson(doc1, file);
  if (error) {
              Serial.println(F("Failed to read file, using default configuration"));
              file.close();
              return;
              }
  // копируем значения из json
  sn=(uint8_t)doc1["station"]; //номер станции
  vol=(uint8_t)doc1["vol"]; //громкость
  // закрываем файл
  Serial.println("Radio Config file loaded!");
  file.close();
}