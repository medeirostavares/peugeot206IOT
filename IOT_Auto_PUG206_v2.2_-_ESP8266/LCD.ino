void msgWelcome(void){
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("BEM VINDO!");
  delay(2500);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("PEUGEOT 206");
  lcd.setCursor(3, 1);
  lcd.print("IOT READY");
  delay(2800);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("GUARABOTS");
  lcd.setCursor(4, 1);
  lcd.print("POWERED");
  delay(2800);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("USE O CINTO");
  lcd.setCursor(2, 1);
  lcd.print("DE SEGURANCA");
  delay(2800);
}

void htmlComandDisplay(String comand){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Comando recebido");
  
  lcd.setCursor(2, (int)(16-comand.length())/2);
  lcd.print(comand);
  delay(3000);
  lcd.clear();
  digitalClockDisplay();
}

void networkPainel(void){
  lcd.clear();
  
  lcd.setCursor(3, 0);
  lcd.print("AP: ");
  lcd.print(ssid);
  
  lcd.setCursor(2, 1);
  lcd.print(WiFi.localIP());
}

void displayPainel(void) {
  if (botaoPrecionado == 3){
    botaoPrecionado = 0;
    if (set_on == false) {
      set_on = true;
      set = 3;
    } else {
      set++;
      if (set >= 9) {
        set = 0;
        RTC.defineDados(0, minuto, hora, auxNomeDia, dia, mes, ano);
        set_on = false;
        setMode = false;
        digitalClockDisplay();
      }
    }
    flagSet_on = false;
  }

  if(millis() - auxSetingClock > 300){
    seting();
    digitalClockDisplaySeting();
    auxSetingClock = millis(); 
  }
}

void seting(void){   
  if(botaoPrecionado == 1){
    botaoPrecionado = 0;
    switch (set){
      case 3://AJUSTA A HORA
        hora--;
        if(hora < 1) hora = 24; 
      break;
      case 4://AJUSTA MINUTOS
        minuto--;
        if(minuto <= 1) minuto = 59;
      break;
      case 5://AJUSTA DIA DA SEMANA
        auxNomeDia--;
        if(auxNomeDia <= 1)auxNomeDia = 6;
      break;
      case 6://AJUSTA DIA DO MES
        dia--;
        if (dia < 1)dia = 31;
      break;
      case 7://AJUSTA MES
        mes--;
        if (mes < 1)mes = 12;
      break;
      case 8://AJUSTA ANO
        ano--;
      break;
    } 
  }

  if (botaoPrecionado == 2) {
    botaoPrecionado = 0;
    switch (set){
      case 3://AJUSTA A HORA
        hora++;
        if(hora > 23) hora = 0; 
      break;
      case 4://AJUSTA MINUTOS
        minuto++;
        if(minuto > 59) minuto = 0;
      break;
      case 5://AJUSTA DIA DA SEMANA
        auxNomeDia++;
        if(auxNomeDia > 6)auxNomeDia = 0;
      break;
      case 6://AJUSTA DIA DO MES
        dia++;
        if (dia > 31)dia = 1;
      break;
      case 7://AJUSTA MES
        mes++;
        if (mes > 12)mes = 1;
      break;
      case 8://AJUSTA ANO
        ano++;
      break;
    } 
  }
}

void digitalClockDisplaySeting(void) {
  lcd.clear();
  Temp = floor(leitura_TTC104(A0));

  lcd.setCursor(0, 0);
  printValue(hora, (set_on && set == 3) ? true : false);
  lcd.print(":");
  printValue(minuto, (set_on && set == 4) ? true : false);
  if(set_on == 1){
    lcd.setCursor(6, 0);
    lcd.print("SET");
  }
  lcd.setCursor(12, 0);
  lcd.print(Temp);lcd.write((byte)0);lcd.print("C");

  lcd.setCursor(0, 1);
  printString(nomeDia[auxNomeDia], (set_on && set == 5) ? true : false);
  lcd.setCursor(6, 1);
  printValue(dia, (set_on && set == 6) ? true : false);lcd.print("/");
  printValue(mes, (set_on && set == 7) ? true : false);lcd.print("/");
  printValue(2000+ano, (set_on && set == 8) ? true : false);
}

void digitalClockDisplay(void) {
  getHora();
  
  lcd.clear();
  Temp = floor(leitura_TTC104(A0));

  lcd.setCursor(0, 0);
  printValue(hora, (set_on && set == 3) ? true : false);
  lcd.print(":");
  printValue(minuto, (set_on && set == 4) ? true : false);
  if(set_on == 1){
    lcd.setCursor(6, 0);
    lcd.print("SET");
  }
  lcd.setCursor(12, 0);
  lcd.print(Temp);lcd.write((byte)0);lcd.print("C");

  lcd.setCursor(0, 1);
  printString(nomeDia[auxNomeDia], (set_on && set == 5) ? true : false);
  lcd.setCursor(6, 1);
  printValue(dia, (set_on && set == 6) ? true : false);lcd.print("/");
  printValue(mes, (set_on && set == 7) ? true : false);lcd.print("/");
  printValue(ano+2000, (set_on && set == 8) ? true : false);
}

//_________________________________________________________________________

void printValue(int value, bool estate){
  if(estate == true){
    if(millis() - auxPrintValue > 150){
      if(flagPrintValue == true){   
        flagPrintValue=!flagPrintValue;
        lcd.print("  ");
        auxPrintValue = millis();
      }else{
        flagPrintValue=!flagPrintValue;
        if(value<10) lcd.print("0");
        lcd.print(value);
        auxPrintValue = millis();  
      }
    } 
  }else{
    if(value<10) lcd.print("0");
    lcd.print(value);
  }
}
void printString(String palavra, bool estate){
  if(estate == true){
    if(millis() - auxPrintValue > 150){
      if(flagPrintValue == true){   
        flagPrintValue=!flagPrintValue;
        lcd.print("   ");
        auxPrintValue = millis();
      }else{
        flagPrintValue=!flagPrintValue;
        lcd.print(palavra);
        auxPrintValue = millis();  
      }
    } 
  }else{
    lcd.print(palavra);
  }
}
