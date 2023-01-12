#include "ESP8266WIFI.h"
SoftwareSerial esp8266(SSTX,SSRX);

ESP8266WIFI::ESP8266WIFI(void){}

void ESP8266WIFI::init(int baud)
{
  esp8266.begin(115200);
  esp8266.write("AT+RST\r\n");
  esp8266.write("AT+CIOBAUD=19200\r\n");
  esp8266.begin(19200);
  esp8266.write("AT+CIOBAUD=");
  esp8266.write(baud);
  esp8266.write("\r\n");
  ClearSBuffer();
  return;
}

void ESP8266WIFI::ClearSBuffer(void){
  while (esp8266.available()){esp8266.read();}}

boolean ESP8266WIFI::ReadSBuffer(String &parcial, String target ,int timeout)
{
  
  int start0 = millis();
  while(!esp8266.available() && (millis() - start0 < timeout));
  while ((parcial.indexOf(target) == -1) && (millis() - start0 < timeout)) {
    parcial = parcial + esp8266.readString();
  }
  if (DEBUG)Serial.println(parcial);
  if (millis() - start0 > timeout)
    return 0;
  return 1;
}



boolean ESP8266WIFI::TimeOut(int timeout, String target, boolean debug )
{
  String response,parcial = "";

  boolean flag = false;
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
     response = response + esp8266.readString(); // read the next character.
     flag=true;
     }
      }
   if(flag)
   {
     if (debug) {
       Serial.print("%"); 
       Serial.print(response);
       Serial.println("%"); }
     if (response.indexOf(target) == -1)   return 0;
      return 1;
  }
  return 0; 
}

boolean ESP8266WIFI::TesteAT (void) {
  String parcial = "";
  esp8266.print("AT\r\n");
  return ReadSBuffer(parcial,"OK", 10);
}



boolean ESP8266WIFI::Reset (void) {
  String parcial="";
  esp8266.print("AT+RST\r\n");
  return ReadSBuffer(parcial,"ready", 3000);
}


boolean ESP8266WIFI::setMode(int cwmode)
{
  String parcial="";
  esp8266.print("AT+CWMODE=");  
  esp8266.print(cwmode);
  esp8266.print("\r\n");
  return ReadSBuffer(parcial,"OK", 3000);
}

boolean ESP8266WIFI::setMux(int mux)
{
  String parcial="";
  esp8266.print("AT+CIPMUX=");
  esp8266.print(mux);
  esp8266.print("\r\n");
  return ReadSBuffer(parcial,"OK", 2000);
}

boolean ESP8266WIFI::createUDP(int id, String remote_IP, int remote_Port, int localPort) {
  String parcial="";
  esp8266.print("AT+CIPSTART=");
  esp8266.print(id);
  esp8266.print(",\"UDP\",\"");
  esp8266.print(remote_IP);
  esp8266.print("\",");
  esp8266.print(remote_Port);
  esp8266.print(",");
  esp8266.print(localPort);
  esp8266.print(",");
  esp8266.print(0);
  esp8266.print("\r\n");
  return ReadSBuffer(parcial,"OK", 2000);
}

boolean ESP8266WIFI::createAP(String ssid, String pwd, int ch, int sec)
{
  if (getMode() == 1)
    return false;
  String parcial="";
  esp8266.print("AT+CWSAP=\"");
  esp8266.print(ssid);
  esp8266.print("\",\"");
  esp8266.print(pwd);
  esp8266.print("\"");
  esp8266.print(",");
  esp8266.print(ch);
  esp8266.print(",");
  esp8266.println(sec);
  esp8266.print("\r\n");
  return ReadSBuffer(parcial,"OK", 2000);
}

boolean ESP8266WIFI::QuitAP (void) {
  String parcial;
  esp8266.println("AT+CWQAP\r\n");
  return ReadSBuffer(parcial,"OK", 100);
}

boolean ESP8266WIFI::enableDHCP (void) {//NAO TA FUNCIONANDO
  int modo = getMode();
  String parcial;
  esp8266.print("AT+CWDHCP=\"");
  esp8266.print(modo);
  esp8266.print("\",\"");
  esp8266.print(",");
  esp8266.print(",");
  esp8266.print(",");
  return ReadSBuffer(parcial,"OK", 100);
}

boolean ESP8266WIFI::disableDHCP (void) { //NAO TA FUNCIONANDO
  int modo = getMode();
  String parcial;
  esp8266.print("AT+CWDHCP=\"");
  esp8266.print(modo);
  esp8266.print("\",\"");
  esp8266.print(1);
  esp8266.print("\"");

  return ReadSBuffer(parcial,"OK", 100);
}


boolean ESP8266WIFI::SendData (String data, int linkID, int timeout) {
  
  
  String parcial = "";
  int len = data.length();
  
  int start0 = millis();
  while((millis() - start0 < timeout)){
      
      
      esp8266.print("AT+CIPSEND=");
      esp8266.print(linkID);
      esp8266.print(",");
      esp8266.print(len);
      esp8266.print("\r\n");
  
      if(ReadSBuffer(parcial,">", timeout))
      { 
       ClearSBuffer(); 
        esp8266.print(data);
      //  esp8266.print("\r\n");
        if(ReadSBuffer(parcial,"SEND OK", timeout))
        {return 1;
           }
      }return 0;
  }return 0;
 }

boolean ESP8266WIFI::pooling() {
 if(esp8266.available() > 0)
    return 1;
  else return 0;
}

String ESP8266WIFI::Status(int timeout)
{
  String parcial = "";
  esp8266.print("AT+CIPSTATUS\r\n");
  if(!ReadSBuffer(parcial,"OK", timeout))return "Timeout";
  else return FindAndFilter(parcial, "STATUS");
}

String ESP8266WIFI::getVersion(int timeout) {
  String parcial = "";
    esp8266.print("AT+GMR\r\n");
  if(!ReadSBuffer(parcial,"OK", timeout)) return "Timeout";
  else return FindAndFilter(parcial, "+GMR");
}


int ESP8266WIFI::getMode(void) {
  String parcial;
  esp8266.print("AT+CWMODE?\r\n");
  if(!ReadSBuffer(parcial, "OK",  2000))return 0;
  else {parcial = FindAndFilter(parcial, "+CWMODE:", "\nOK");
  return parcial.toInt();}
}

String ESP8266WIFI::getIP(int timeout) {
  String parcial;
  esp8266.print("AT+CIFSR\r\n");
  if(!ReadSBuffer(parcial,"OK", timeout)) return "Timeout";
  else return FindAndFilter(parcial, "+CIFSR:", "\nOK");
}

String ESP8266WIFI::listClient(int timeout) {
  String parcial;
  esp8266.print("AT+CWLIF\r\n");
  if(!ReadSBuffer(parcial,"OK", timeout))return "Timeout";
  else return FindAndFilter(parcial, "+CWLIF:", "\nOK");
}


String ESP8266WIFI::listAP(int timeout)
{
  String parcial = "";
  if (getMode() == 2)
    return "ERROR. CWMODE=2.";
  esp8266.print("AT+CWLAP\r\n");
  if(!ReadSBuffer(parcial,"OK", timeout))return "Timeout";
  else return FindAndFilter(parcial, "+CWLAP:", "\nOK");
  
}


String ESP8266WIFI::connectAP(String ssid, String pwd, int timeout)
{
  if (getMode() == 2) 
    return "ERROR. CWMODE=2.";
  
    esp8266.print("AT+CWJAP=\"");
    esp8266.print(ssid);
    esp8266.print("\",\"");
    esp8266.print(pwd);
    esp8266.print("\"");
    esp8266.print("\r\n");
    
    String parcial ="";
    if(!ReadSBuffer(parcial,"WIFI CONNECTED", timeout))return "Timeout WIFI";
        
    if(!ReadSBuffer(parcial,"\nOK", timeout))return "Timeout DHCP";
    else return FindAndFilter(parcial, "\nWIFI", "\nOK");;
     
}

String ESP8266WIFI::ReceiveData (int timeout) {
  String retorno = "";
  while(esp8266.available()){
  retorno = retorno + esp8266.readString();}
  return retorno;
}


String ESP8266WIFI::FindAndFilter (String data, String inicio, String fim) {

  String data_tmp;
  int32_t index1 = data.indexOf(inicio);
  int32_t index2 = data.indexOf(fim);
  if (index1 != -1 && index2 != -1) {
    index1 += inicio.length();
    data_tmp = data.substring(index1, index2);
    return data_tmp;
  }
  return "ERRO";
}
String ESP8266WIFI::FindAndFilter (String data, String inicio) {

  String data_tmp;
  int32_t index1 = data.indexOf(inicio);
  int32_t index2 = data.length();
  if (index1 != -1 && index2 != -1) {
    index1 += inicio.length();
    data_tmp = data.substring(index1, index2);
    return data_tmp;
  }
  return "ERRO";
}

