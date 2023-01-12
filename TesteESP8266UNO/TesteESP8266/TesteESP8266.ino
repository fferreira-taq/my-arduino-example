
//********** Bibliotecas ******************************** //
#include "ESP8266WIFI.h"                                  // Inclui biblioteca ESP8266WIFI
#include <SoftwareSerial.h>
//********** Variáveis Globais ************************** //
int i = 0;
boolean intSerial = 0;
String retorno;
const int BaudRate = 115200;
//********** Nomeação dos Periféricos ******************* //
ESP8266WIFI wifi;

//********** Interrupção do Timer *********************** //

//********** Setups ************************************* //
void setup()
{
  Serial.begin(9600);
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
  boolean at=0;
   at =wifi.SendData ("E la vamos nos", 0, 3000);

  

    if (wifi.pooling()) {
    i++;
    retorno="";
    retorno = wifi.ReceiveData(2000);
    Serial.print("$");
    Serial.print(retorno);
    Serial.println("$");
    intSerial = 0;
    }
    
 }
  
boolean configuraWifi() {
  boolean at;
  int i = 0;
  String  Status; 
  wifi.init(BaudRate);
    
  do {
    i++;
    Serial.print("Reset:");
    Serial.println(i);
    at = wifi.Reset();
    Serial.println(at ? "Sucess":"Fail");
  } while ((at ==0) && (i < 5));
  i = 0;

  Serial.print("Verificando Version:");
  retorno = wifi.getVersion(3000);
  Serial.println(retorno);
  
  do {
    i++;
    Serial.print("Set Mode:");
    Serial.println(i);
    at = wifi.setMode(AP);
    Serial.println(at ? "Sucess":"Fail");
  } while ((at ==0) && (i < 5));
  i = 0;
  
  do {
    i++;
    Serial.print("Criando AP:");
    Serial.println(i);
    at = wifi.createAP("TESTE00", "12345678", 5, WPA_PSK);
    Serial.println(at ? "Sucess":"Fail");
  } while ((at ==0) && (i < 5));
  i = 0;
  
  Serial.print("Verificando Mode:");
  i = wifi.getMode();
  Serial.println(i);
    
  Serial.print("Listando AP:");
  retorno = wifi.listAP(5000);
  Serial.println(retorno);
  i = 0;
/*  do {
   i++;
   Serial.print("Conectando AP:");
   Serial.println(i);
   Status = wifi.connectAP("TESTE00", "12345678", 5000);
   if((Status == "Timeout WIFI") || (Status == "Timeout DHCP"))
     at = 0;
   Serial.println(Status);
   Serial.println(wifi.getIP(3000));
 } while ((at ==0) && (i < 5));
 */i=0;
    do {
    i++;
    Serial.print("Set Mux:");
    Serial.println(i);
    at = wifi.setMux(MULTIPLE);
    Serial.println(at ? "Sucess":"Fail");
  } while ((at ==0) && (i < 5));
  i = 0;
  do {
    i++;
    Serial.print("Criando UDP:");
    Serial.println(i);
    at = wifi.createUDP(0,"192.168.4.255", 8080, 9999);
    Serial.println(at ? "Sucess":"Fail");
  } while ((at ==0) && (i < 5));
  i = 0;
  
//  Serial.print("Verificando IP:");
//  retorno = wifi.getIP(3000);
//  Serial.println(retorno);
  
  Serial.print("Verificando Status:");
  retorno = wifi.Status(2000);
  Serial.println(retorno);
  
    Serial.print("Listando Clientes:");
  retorno = wifi.listClient(3000);
  Serial.println(retorno);
  
  do {
    i++;
    Serial.print("E la vamos nos:");
    Serial.println(i);
    at = wifi.SendData ("E la vamos nos", 0, 5000);
    Serial.println(at ? "Sucess":"Fail");
  } while ((at ==0) && (i < 5));
  

  

  

}

