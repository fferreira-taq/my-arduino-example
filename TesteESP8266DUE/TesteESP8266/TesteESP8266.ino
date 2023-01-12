//INSTOR - Projetos e Robótica//
//Rogério Ballestrin
//rogerio@instor.com.br

//********** Bibliotecas ******************************** //
#include "ESP8266WIFI.h"                                  // Inclui biblioteca ESP8266WIFI

//********** Variáveis Globais ************************** //
int i = 0;
boolean intSerial = 0;
String retorno;
//********** Nomeação dos Periféricos ******************* //
ESP8266WIFI wifi;
//********** Interrupção do Timer *********************** //

//********** Setups ************************************* //
void setup()
{
  Serial.begin(115200);
  Serial.println("Configurando...");
  int configurado;
  int start0 = millis();
  configurado = configuraWifi();
  int start1 = millis();
  Serial.print("Tempo configurando:");
  int temp = start1 - start0;
  Serial.println(temp);
}

void loop() {
  intSerial = wifi.pooling();
  if (intSerial) {
    i++;
    retorno = wifi.ReceiveData();
    wifi.SendData (retorno, 0, 2000);
    intSerial = 0;
  }
}

boolean configuraWifi() {
  boolean at;
  int i = 0;
  wifi.init(&Serial3, 115200, 53);
  
  do {
    i++;
    Serial.print("Reset:");
    Serial.println(i);
    at = wifi.Reset();
  } while ((at ==0) && (i < 5));
  i = 0;
  do {
    i++;
    Serial.print("Set Mode:");
    Serial.println(i);
    at = wifi.setMode(STA_AP);
  } while ((at ==0) && (i < 5));
  i = 0;
  do {
    i++;
    Serial.print("Criando AP:");
    Serial.println(i);
    at = wifi.createAP("Eletrospinning INSTOR", "12345678", 5, WPA_PSK);
  } while ((at ==0) && (i < 5));
  i = 0;
  do {
    i++;
    at = wifi.setMux(MULTIPLE);
    Serial.print("Set Mux:");
    Serial.println(i);
  } while ((at ==0) && (i < 5));
  i = 0;
  do {
    i++;
    Serial.print("Criando UDP:");
    Serial.println(i);
    at = wifi.createUDP(0, "192.168.4.255", 8888, 9999);
  } while ((at ==0) && (i < 5));
  i = 0;
  do {
    i++;
    Serial.print("E la vamos nos:");
    Serial.println(i);
    at = wifi.SendData ("E la vamos nos", 0, 2000);
  } while ((at ==0) && (i < 5));

}
