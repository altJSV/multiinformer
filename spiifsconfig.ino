void loadConfiguration(const char *filename) {
  // Open file for reading
  File file = SPIFFS.open(filename);
  if(!file){
        Serial.println("Failed to open file");
        return;
        }
  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/v6/assistant to compute the capacity.
  StaticJsonDocument<1024> doc1;

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc1, file);
  if (error) {
              Serial.println(F("Failed to read file, using default configuration"));
              file.close();
              return;
              }

  // Copy values from the JsonDocument to the Config 
  const char * jspc_server_path = doc1["pcadress"] ;
  //const char * jstoken = doc1["yatoken"];
  const char * jsapi_key= doc1["weatherapi"];
  const char * jsqLocation= doc1["weathercity"]; 
  usesensor= (bool)doc1["usesensor"];
  gmt= (int8_t)doc1["gmt"];
  Serial.println(gmt);
  refpcinterval= (uint32_t)doc1["refpcinterval"];
  refweatherinterval= (uint32_t)doc1["refweatherinterval"];
  //refyandexinterval= (uint32_t)doc1["refyandexinterval"];
  refsensorinterval= (uint32_t)doc1["refsensorinterval"];
  bright_level = (uint8_t)doc1["bright_level"];
  //String ntpserverbuf=doc1["ntpserver"] | "pool.ntp.org";
  //ntpserver=&ntpserverbuf[1];
  //Serial.println(ntpserver);
  //red_level = (uint8_t)doc1["red_level"];
  //green_level = (uint8_t)doc1["green_level"];
  //blue_level = (uint8_t)doc1["blue_level"];
  photosensor= (bool)doc1["photosensor"];
  pc_server_path = jspc_server_path;
  //token = jstoken;
  api_key = jsapi_key;
  qLocation  = jsqLocation;
  // Close the file (Curiously, File's destructor doesn't close the file)
  file.close();
}

// Saves the configuration to a file
void saveConfiguration(const char *filename) {
  // Delete existing file, otherwise the configuration is appended to the file
  SPIFFS.remove(filename);

  // Open file for writing
  File file = SPIFFS.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println(F("Failed to create file"));
    return;
  }

  // Allocate a temporary JsonDocument
  // Don't forget to change the capacity to match your requirements.
  // Use arduinojson.org/assistant to compute the capacity.
  StaticJsonDocument<1024> doc1;

  // Set the values in the document
  doc1["pcadress"] = pc_server_path;
  //doc1["yatoken"] = token;
  doc1["weatherapi"] = api_key;
  doc1["weathercity"] = qLocation;
  doc1["usesensor"] = usesensor;
  doc1["gmt"] = gmt;
  Serial.println(gmt);
  doc1["refpcinterval"] = refpcinterval;
  doc1["refweatherinterval"] = refweatherinterval;
  //doc1["refyandexinterval"] = refyandexinterval;
  doc1["refsensorinterval"] = refsensorinterval;
  doc1["bright_level"] = bright_level;
  //doc1["ntpserver"]=ntpserver;
  //Serial.println(ntpserver);
  //doc1["red_level"] = red_level;
  //doc1["green_level"] = green_level;
  //doc1["blue_level"] = blue_level;
  doc1["photosensor"] = photosensor;
  // Serialize JSON to file
  if (serializeJson(doc1, file) == 0) {
    Serial.println(F("Failed to write to file"));
  }
  else
  {
    Serial.println("Config file saved!");
  }
  // Close the file
  file.close();
}