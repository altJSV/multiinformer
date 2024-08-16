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