
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
    
}

void audio_showstreamtitle(const char *info){
    lv_label_set_text_fmt(radio_playing_label, "Трек: %s",info);
}

void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}

void url_sta(uint8_t c) { // Разделение  URL и sta
  url=playlistread(LittleFS,"/playlist.txt",c);
  if (url=="Failed to open file for reading") url="http://vladfm.ru:8000/vfm*Владивосток FM";
  String u=url.substring(0,url.indexOf("*")+1);
  u.toCharArray(URL,u.length());           // URL
  Serial.println(URL);
  String s=url.substring(url.indexOf("//")+2);
  String n=s.substring(s.indexOf("*")+1)+1;
  n.toCharArray(sta,n.length());           // название станции
}