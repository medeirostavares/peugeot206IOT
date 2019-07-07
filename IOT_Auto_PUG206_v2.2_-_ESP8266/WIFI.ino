void verificaWifi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(" ");
    Serial.print("Tentando reconectar");
    WiFi.begin(ssid, pass);

    static unsigned char tentativas;
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      tentativas++;
      delay(500);
      if (tentativas == 10) {
        tentativas = 0;
        break;
      }
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi conectado!");
      Serial.print("Endereço IP: ");
      Serial.println(WiFi.localIP());
      server.begin();
      Serial.println("Peugeot 206 OnLine");
      Serial.println("Aguardando instruções...");

      digitalWrite(D3, LOW);
      digitalWrite(D4, HIGH);
    } else {
      Serial.println("Sem conexão com WiFi!");
      Serial.println("");

      digitalWrite(D3, HIGH);
      digitalWrite(D4, LOW);
    }
  }
}
