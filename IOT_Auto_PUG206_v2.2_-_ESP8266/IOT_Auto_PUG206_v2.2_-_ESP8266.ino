#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUDP.h>
#include <LiquidCrystal_I2C.h>
#include <GuaraTeca_Hardware.h>

#define DEBUG 

//Pinos dos reles no expansor I2C.
#define TRAVAS  0
#define SETAS   1
#define BUZINA  2
#define FAROIS  3
#define PARTIDA 4
#define IGNICAO 5
#define MEIALUZ 6
#define P7      7

//const char* ssid = "GUARABOTS";
//const char* pass = "megapower";
const char* ssid = "Joaquim";
const char* pass = "joaquimAP";

//Variaveis de auxilio aos Millis().
unsigned long int millisWifi       = 0,
                  auxPrintValue    = 0,
                  auxSetingClock   = 0,
                  auxUpdateDisplay = 0;

String nomeDia[7] = {{"DOM"},{"SEG"},{"TER"},{"QUA"},{"QUI"},{"SEX"},{"SAB"}};
unsigned int dia, mes, ano, hora, minuto, auxNomeDia;
unsigned int botaoPrecionado;
unsigned int set = 0;
int Temp = 60;

bool setModeOperation = true;

bool tela           = true ,
     set_on         = false,
     setMode        = false,
     flagMode1      = false,
     flagMode2      = false,
     flagMode3      = false,
     flagSet_on     = false,
     flagSetMode    = false,
     flagPrintValue = true ;          

//Array simbolo grau
byte grau[8] = { B00001100,
                 B00010010,
                 B00010010,
                 B00001100,
                 B00000000,
                 B00000000,
                 B00000000,
                 B00000000
               };

void sycTimeNTP(bool upd = false);

DS1307 RTC(0x68);
PCF8574 reles(0x3B);
WiFiServer server(80);
WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);//Cria um objeto "NTP" com as configurações.
LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup(){
  #ifdef DEBUG
  Serial.begin(115200);
  #endif

  lcd.begin (16, 2);
  
  lcd.createChar(0, grau);
  lcd.backlight();
  lcd.clear();
  
  auxNomeDia=1, dia=17, mes=4, ano=13;
  hora=23, minuto=59;
  RTC.defineDados(40, minuto, hora, auxNomeDia, dia, mes, ano);
  
  //OBS: PINOS D1 E D2 USADOS NA INTERFACE SERIAL I2C
  pinMode(A0, INPUT ); //TEMPERATURA
  pinMode(D0, OUTPUT); //LED ONBOARD/TESTES
  pinMode(D3, INPUT ); //EMPTY
  pinMode(D4, INPUT ); //Verifica ignição fisica
  pinMode(D5, OUTPUT); //LED OFLINE
  pinMode(D6, OUTPUT); //LED ONLINE
  pinMode(D7, INPUT ); //SET 1
  pinMode(D8, INPUT ); //SET 2

  for(int i=0; i < 8; i++){
    reles.estadoPino(i,HIGH);
  }
  
  verificaWifi();

  ntp.begin();//Inicia o NTP.
  ntp.forceUpdate();//Força o Update.
  sycTimeNTP(true);
  
  controlSystem("STAND_BY");
}

/*
 * Aguarda uma conexão com o usuario
 * Esse modo será utilizado apenas quando o carro estiver desligado, deixando o esp
 * consumindo apenas o minimo.
 * Ele deverá manter o WIFI ativo aguardando um acesso ao IP do servidor!
 * todo o resto deverá ser desligado!
 */
void mode_standBy(){
  static bool alterBrilho = false;
  static bool iginicaoFisica = false;
  
  if(millis() - millisWifi > 1000){
    verificaWifi();
    networkPainel();
    if(server.available() > 0){
      lcd.backlight();
      sycTimeNTP(true);
      setModeOperation = false;
    }
    millisWifi = millis();
  }

  if(digitalRead(D8) == true){
    alterBrilho = !alterBrilho;
    alterBrilho == true ? lcd.backlight() : lcd.noBacklight();
    delay(300);
  }

  /*if(digitalRead(D4) == true){
    controlSystem("startIgnition");
  }*/
}

/*
 * Inicializa o sistema.
 * Nessa função serao ativados todos os demais componentes do esp e funções de controle do carro! 
 */
void mode_ignition(){
  readButtons();
  
  if(millis() - millisWifi > 500){
    verificaWifi();
    webServer(); 
    sycTimeNTP();
    millisWifi = millis();
  }
  
  if(setMode == true){
    displayPainel();
    Serial.println("SET");
  } else {
    if (botaoPrecionado == 3){
      flagSetMode = false;
      setMode = true;
    } else if(botaoPrecionado == 1 || botaoPrecionado == 2){
      botaoPrecionado = 0;
      tela=!tela;
      if(tela == true){
        digitalClockDisplay();
      }else{
        networkPainel();
      }
    }
    
    if(millis() - auxUpdateDisplay > 5000){
      digitalClockDisplay();
      auxUpdateDisplay = millis(); 
    } 
  }
}

void controlSystem(String comand){
  htmlComandDisplay(comand);
  
  if(comand == "STAND_BY"){
     networkPainel();
     lcd.noBacklight();
     setModeOperation = true;
  }
  
  if(comand == "startIgnition"){
    reles.estadoPino(IGNICAO, LOW);
  }else if(comand == "stopIgnition"){
    reles.estadoPino(IGNICAO, HIGH);
  }

  if(comand == "startEngine"){
    reles.estadoPino(PARTIDA, LOW);
    delay(750);
    reles.estadoPino(PARTIDA, HIGH);
  }

  if(comand == "lightsOn"){
    reles.estadoPino(FAROIS, LOW);
  }else if(comand == "lightsOff"){
    reles.estadoPino(FAROIS, HIGH);
  }

  if(comand == "meiaLuzOn"){
    reles.estadoPino(MEIALUZ, LOW);
  }else if(comand == "meiaLuzOff"){
    reles.estadoPino(MEIALUZ, HIGH);
  }

  if(comand == "alterSetas"){
    reles.estadoPino(SETAS, LOW);
    delay(300);
    reles.estadoPino(SETAS, HIGH);
  }

  if(comand == "hornStart"){
    reles.estadoPino(BUZINA, LOW);
    delay(300);
    reles.estadoPino(BUZINA, HIGH);
  }

  if(comand == "lockStart"){
    reles.estadoPino(TRAVAS, LOW);
    delay(300);
    reles.estadoPino(TRAVAS, HIGH);
  }
}

void loop(){    
  if(setModeOperation == true){
    mode_standBy();
  }else{
    mode_ignition();
  }
}

void readButtons(void) {
  if(digitalRead(D7) == true && digitalRead(D8) == false && flagMode1 == false && flagMode3 == false){
    flagMode1 = true;
  }else if(digitalRead(D7) == false && digitalRead(D8) == true && flagMode2 == false && flagMode3 == false){
    flagMode2 = true;
  }else if(digitalRead(D7) == true && digitalRead(D8) == true && flagMode3 == false){
    flagMode3 = true;
  }
  
  if(digitalRead(D7) == false && digitalRead(D8) == false && (flagMode1 == true || flagMode2 == true || flagMode3 == true)){
    if(flagMode3 == true){
      flagMode1 = flagMode2 = flagMode3 = false;
      botaoPrecionado = 3;
    }else if(flagMode2 == true){
      flagMode1 = flagMode2 = flagMode3 = false;
      botaoPrecionado = 2;
    }else if(flagMode1 == true){
      flagMode1 = flagMode2 = flagMode3 = false;
      botaoPrecionado = 1;
    }
  }
}

void versao() {
  #ifdef DEBUG
  Serial.println("Peugeot 2016 IOT System 1.0");
  Serial.println("Desenvolvimento:");
  Serial.println("Instituto Federal do Tocantins");
  Serial.println("Dianópolis - Tocantins");
  Serial.println("Equipe GuaraBots");
  Serial.println("Prof. Fabiano Medeiros");
  Serial.println("Aluno Joaquim Flavio");
  Serial.println("Versão do jQuery: 3.3.1");
  Serial.println("Versão do Bootstrap: 3.3.7");
  Serial.println("");
  #endif
}
