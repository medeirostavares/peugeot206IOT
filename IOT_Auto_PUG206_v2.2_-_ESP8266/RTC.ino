void getHora(){
  RTC.atualiza();
  
  dia = RTC.getDiaDoMes();
  mes = RTC.getMes();
  ano = RTC.getAno();
  hora = RTC.getHoras();
  minuto = RTC.getMinutos();
  auxNomeDia = RTC.getNDiaDaSemana(); 
  
  #ifdef DEBUG
  Serial.print("Data: ");
  Serial.print(RTC.getDiaDoMes());
  Serial.print("/");
  Serial.print(RTC.getMes());
  Serial.print("/");
  Serial.print(RTC.getAno());
  Serial.print(" ");
  Serial.print("Hora : ");
  Serial.print(RTC.getHoras());
  Serial.print(":");
  Serial.print(RTC.getMinutos());
  Serial.print(":");
  Serial.print(RTC.getSegundos());
  Serial.print(", ");
  Serial.println(RTC.getDiaDaSemana());
  #endif
}

void sycTimeNTP(bool upd){
  static unsigned int auxUpdate=0;
  if((minuto - auxUpdate > 29 || upd == true) && WiFi.status() == WL_CONNECTED){
    RTC.defineDados(ntp.getSeconds(), ntp.getMinutes(), ntp.getHours(), auxNomeDia, ntp.getDay(), mes, ano); 
    auxUpdate = minuto;
    Serial.println("update with NTP!");
  }
}

