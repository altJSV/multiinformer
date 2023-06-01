
void audio_info(const char *info){
    Serial.print("info        "); Serial.println(info);
    
}

void audio_showstreamtitle(const char *info){
    lv_label_set_text_fmt(radio_playing_label, "Трек: %s",info);
}

void audio_showstation(const char *info){
    Serial.print("station     ");Serial.println(info);
}

void url_sta(int c) { // Разделение  URL и sta
  String u=String(url[c]).substring(0,String(url[c]).indexOf("*")+1);
  u.toCharArray(URL,u.length());           // URL
  Serial.println(URL);
  String s=String(url[c]).substring(String(url[c]).indexOf("//")+2);
  String n=s.substring(s.indexOf("*")+1)+1;
  n.toCharArray(sta,n.length());           // название станции
}