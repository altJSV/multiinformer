

void hardwareMonitor()
{
  WiFiClient client;
  HTTPClient http;
  // Отправляем запрос
  http.useHTTP10(true);
  http.begin(client, pc_server_path);
      //Здесь проверяем доступен ли воообще JSON по указанному нами адресу
      int httpCode = http.GET();
      if (httpCode > 0) {
        Serial.println(httpCode);
 /*StaticJsonDocument<400> filter;
JsonObject filter_Children_0_Children_0 = filter["Children"][0]["Children"].createNestedObject();
//filter_Children_0_Children_0["Text"] = true;
JsonObject filter_Children_0_Children_0_Children_0_Children_0 = filter_Children_0_Children_0["Children"][0]["Children"].createNestedObject();
//filter_Children_0_Children_0_Children_0_Children_0["Text"] = true;
filter_Children_0_Children_0_Children_0_Children_0["Value"] = true;
filter_Children_0_Children_0_Children_0_Children_0["Children"][0]["Value"] = true;

  // непосредственно парсинг JSON. Файл очень большой, памяти занимает много, парсинг занимает немало времени
  DeserializationError error = deserializeJson(hwm, http.getStream(), DeserializationOption::Filter(filter), DeserializationOption::NestingLimit(12));
  */
  StaticJsonDocument<224> filter;

JsonObject filter_Children_0_Children_0 = filter["Children"][0]["Children"].createNestedObject();
filter_Children_0_Children_0["Value"] = true;

JsonObject filter_Children_0_Children_0_Children_0 = filter_Children_0_Children_0["Children"].createNestedObject();
filter_Children_0_Children_0_Children_0["Value"] = true;

JsonObject filter_Children_0_Children_0_Children_0_Children_0 = filter_Children_0_Children_0_Children_0["Children"].createNestedObject();
filter_Children_0_Children_0_Children_0_Children_0["Value"] = true;
filter_Children_0_Children_0_Children_0_Children_0["Children"][0]["Value"] = true;

DynamicJsonDocument hwm(12288);
DeserializationError error = deserializeJson(hwm, http.getStream(), DeserializationOption::Filter(filter), DeserializationOption::NestingLimit(12));
  if (error) {
      String errorStr = error.c_str();
      Serial.print ("Hardware parsing: ");
      Serial.println(errorStr);
    }
    else
    {
  
   /*структура файла довольно таки большая и сложная, со множеством вложенных списков и ветвлений, но в целом разобраться можно
    ["Children"][0]["Children"][1] - разлчиные параметры процессора
    ["Children"][0]["Children"][3] - видеокарта
    ["Children"][0]["Children"][2] - память
    ["Children"][0]["Children"][0] - материнская плата
   */
   //String cpuName = hwm["Children"][0]["Children"][1]["Text"];
   String cpuTempPackage = hwm["Children"][0]["Children"][1]["Children"][1]["Children"][10]["Value"];
   String cpuLoad = hwm["Children"][0]["Children"][1]["Children"][2]["Children"][0]["Value"];
   //String gpuName = hwm["Children"][0]["Children"][3]["Text"];
   String gpuHotSpot = hwm["Children"][0]["Children"][3]["Children"][1]["Children"][0]["Value"];
   String gpuLoad = hwm["Children"][0]["Children"][3]["Children"][2]["Children"][0]["Value"];
   String usedRAM = hwm["Children"][0]["Children"][2]["Children"][1]["Children"][0]["Value"];
  String freeRAM = hwm["Children"][0]["Children"][2]["Children"][1]["Children"][1]["Value"];
  String gpuRAMused = hwm["Children"][0]["Children"][3]["Children"][5]["Children"][1]["Value"];
  String gpuRAM = hwm["Children"][0]["Children"][3]["Children"][5]["Children"][0]["Value"];
  String cpuFANpercent= hwm["Children"][0]["Children"][0]["Children"][0]["Children"][3]["Children"][0]["Value"];
  String gpuFANpercent = hwm["Children"][0]["Children"][3]["Children"][4]["Children"][0]["Value"];
  String uploadSpeed = hwm["Children"][0]["Children"][12]["Children"][2]["Children"][0]["Value"];
  String downloadSpeed = hwm["Children"][0]["Children"][12]["Children"][2]["Children"][1]["Value"];
  u_int8_t uram=usedRAM.toInt();
  u_int8_t totalram=uram+freeRAM.toInt();
  int gram=gpuRAMused.toInt();
  int gtotalram=gram+gpuRAM.toInt();
  u_int8_t cpl=cpuLoad.toInt();
  u_int8_t cpt=cpuTempPackage.toInt();
  u_int8_t cpf=cpuFANpercent.toInt();
  u_int8_t gpl=gpuLoad.toInt();
  u_int8_t gpt=gpuHotSpot.toInt();
  u_int8_t gpf=gpuFANpercent.toInt();
  String totalramtext="Исп.: "+usedRAM+" Св.:" + freeRAM;
  String totalgputext="Исп.: "+gpuRAMused+" Св.:" + gpuRAM;
  
  //отображаем полученные значения
  //загрузка процессора
  lv_anim_set_var(&cpu, cpu_indic);
  lv_anim_set_time(&cpu, 500);
  lv_anim_set_values(&cpu, prev_cpu_usage, cpl);
  lv_anim_start(&cpu);
  prev_cpu_usage=cpl; 
  //Температура процессора
  lv_anim_set_var(&cpu, cputemp_indic);
  lv_anim_set_time(&cpu, 500);
  lv_anim_set_values(&cpu, prev_cpu_temp, cpt);
  lv_anim_start(&cpu);
  prev_cpu_temp=cpt;
  //вентилятор процессора
  lv_anim_set_var(&cpu, cpufan_indic);
  lv_anim_set_time(&cpu, 500);
  lv_anim_set_values(&cpu, prev_cpu_fan, cpf);
  lv_anim_start(&cpu);
  prev_cpu_fan=cpf;
  //Загрузка видеокарты
  lv_anim_set_var(&gpu, gpu_indic);
  lv_anim_set_time(&gpu, 500);
  lv_anim_set_values(&gpu, prev_gpu_usage, gpl);
  lv_anim_start(&gpu);
  prev_gpu_usage=gpl; 
  //температура видеокарты
  lv_anim_set_var(&gpu, gputemp_indic);
  lv_anim_set_time(&gpu, 500);
  lv_anim_set_values(&gpu, prev_gpu_temp, gpt);
  lv_anim_start(&gpu);
  prev_gpu_temp=gpt;
  //вентилятор видеокарты
  lv_anim_set_var(&gpu, gpufan_indic);
  lv_anim_set_time(&gpu, 500);
  lv_anim_set_values(&gpu, prev_gpu_fan, gpf);
  lv_anim_start(&gpu);
  prev_gpu_fan=gpf;
  cpuLoad=LV_SYMBOL_CPU+cpuLoad;
  gpuLoad=LV_SYMBOL_GPU+gpuLoad;
  cpuTempPackage=LV_SYMBOL_TEMP+cpuTempPackage;
  gpuHotSpot=LV_SYMBOL_TEMP+gpuHotSpot;
  cpuFANpercent=LV_SYMBOL_FAN+cpuFANpercent;
  gpuFANpercent=LV_SYMBOL_FAN+gpuFANpercent;
  lv_label_set_text(cpu_load_label, cpuLoad.c_str());
  lv_label_set_text(cpu_temp_label, cpuTempPackage.c_str());
  lv_label_set_text(cpu_fan_label, cpuFANpercent.c_str());
  lv_label_set_text(gpu_load_label, gpuLoad.c_str());
  lv_label_set_text(gpu_temp_label, gpuHotSpot.c_str());
  lv_label_set_text(gpu_fan_label, gpuFANpercent.c_str());
  lv_anim_set_var(&ramcpu, rambar);
  lv_anim_set_var(&ramgpu, gpurambar);
  lv_anim_set_time(&ramcpu, 500);
  lv_anim_set_time(&ramgpu, 500);
  lv_anim_set_values(&ramcpu, prev_ram, uram);
  lv_anim_start(&ramcpu);
  prev_ram=uram;
  lv_anim_set_values(&ramgpu, prev_gpuram, gram);
  lv_anim_start(&ramgpu);
  prev_gpuram=gram;
  lv_label_set_text(ram_val_label, totalramtext.c_str());
  lv_label_set_text(gpuram_val_label, totalgputext.c_str());
  //Сетевые параметры
  uploadSpeed=LV_SYMBOL_UPLOAD+uploadSpeed;
  downloadSpeed=LV_SYMBOL_DOWNLOAD+downloadSpeed;
  lv_label_set_text(ui_upload_label, uploadSpeed.c_str());
  lv_label_set_text(ui_download_label, downloadSpeed.c_str());
/*
lv_meter_set_indicator_end_value(cpumeter, cpu_indic, cpuLoad.toInt());
lv_meter_set_indicator_end_value(cpumeter, cputemp_indic, cpuTempPackage.toInt());
lv_meter_set_indicator_end_value(gpumeter, gpu_indic, gpuLoad.toInt());
lv_meter_set_indicator_end_value(gpumeter, gputemp_indic, gpuHotSpot.toInt());*/
  }
      }
  else
    {
    Serial.println("http.GET() == 0");
  }
  http.end();
      
}
