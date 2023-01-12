#include "ESP8266WIFI.h"

ESP8266WIFI::ESP8266WIFI(void)
{
}

void ESP8266WIFI::init(HardwareSerial  *uart, int baud, int pinCH)
{
  pinMode(pinCH, OUTPUT);
  digitalWrite(pinCH, HIGH);
  delay(350);
  uartEsp = uart;
  uartEsp->begin(baud);
}


boolean ESP8266WIFI::TimeOut(int timeout, String target)
{
  String data = "";
  uartEsp->setTimeout(timeout);
  data = uartEsp->readString();
  if (data.indexOf(target) == -1) {
    return 0;
  }
  return 1;

}

boolean ESP8266WIFI::TesteAT (void) {
  uartEsp->println("AT");
  return TimeOut(15, "OK");
}
boolean ESP8266WIFI::Reset (void) {
  uartEsp->println("AT+RESTORE");
  return TimeOut(2000, "ready");
}

String ESP8266WIFI::getVersion(void) {
  String retorno;
  uartEsp->println("AT+GMR");
  delay(100);
  String ret = FindAndFilter(uartEsp->readString(), "AT version:", "\nSDK");
  return ret;
}

int ESP8266WIFI::getMode(void) {
  String retorno;
  uartEsp->println("AT+CWMODE?");
  delay(10);
  String ret = FindAndFilter(uartEsp->readString(), "+CWMODE:", "\nOK");
  return ret.toInt();
}

boolean ESP8266WIFI::setMode(int cwmode)
{
  uartEsp->print("AT+CWMODE=");
  uartEsp->println(cwmode);
  return TimeOut(1000, "OK");
}
String ESP8266WIFI::listAP(int timeout)
{
  String parcial = "";
  if (getMode() == 2)
    return "ERROR. CWMODE=2.";
  uartEsp->println("AT+CWLAP");
  int start0 = millis();
  while ((parcial.indexOf("OK") == -1) && (millis() - start0 < timeout)) {
    parcial = parcial + uartEsp->readString();
  }
  if (millis() - start0 > timeout)
    return "Timeout";
  return parcial;
}

boolean ESP8266WIFI::setMux(int mux)
{
  uartEsp->print("AT+CIPMUX=");
  uartEsp->println(mux);
  return TimeOut(1000, "OK");
}

boolean ESP8266WIFI::createUDP(int id, String remote_IP, int remote_Port, int localPort) {
  uartEsp->print("AT+CIPSTART=");
  uartEsp->print(id);
  uartEsp->print(",\"UDP\",\"");
  uartEsp->print(remote_IP);
  uartEsp->print("\",");
  uartEsp->print(remote_Port);
  uartEsp->print(",");
  uartEsp->print(localPort);
  uartEsp->print(",");
  uartEsp->println(0);
  return TimeOut(3000, "OK");
}

String ESP8266WIFI::connectAP(String ssid, String pwd, int timeout)
{
  if (getMode() == 2) {
    return "ERROR. CWMODE=2.";
  } else {
    uartEsp->print("AT+CWJAP=\"");
    uartEsp->print(ssid);
    uartEsp->print("\",\"");
    uartEsp->print(pwd);
    uartEsp->println("\"");

    int start0 = millis();
    String parcial = "";
    while ((parcial.indexOf("WIFI CONNECTED") == -1) && (millis() - start0 < timeout)) {
      parcial = parcial + uartEsp->readString();
    }
    if (millis() - start0 > timeout)
      return "Timeout WIFI";

    start0 = millis();
    while ((parcial.indexOf("\nOK") == -1) && (millis() - start0 < timeout)) {
      parcial = parcial + uartEsp->readString();
    }
    if (millis() - start0 > timeout)
      return "Timeout DHCP";
    return parcial;
  }
}

boolean ESP8266WIFI::createAP(String ssid, String pwd, int channel, int sec)
{
  if (getMode() == 1)
    return false;

  uartEsp->print("AT+CWSAP=\"");
  uartEsp->print(ssid);
  uartEsp->print("\",\"");
  uartEsp->print(pwd);
  uartEsp->print("\"");
  uartEsp->print(",");
  uartEsp->print(channel);
  uartEsp->print(",");
  uartEsp->println(sec);
  return TimeOut(3000, "OK");
}

boolean ESP8266WIFI::QuitAP (void) {
  uartEsp->println("AT+CWQAP");
  return TimeOut(100, "OK");
}

boolean ESP8266WIFI::enableDHCP (void) {//NAO TA FUNCIONANDO
  int modo = getMode();
  uartEsp->print("AT+CWDHCP=\"");
  uartEsp->print(modo);
  uartEsp->print("\",\"");
  uartEsp->print(0);
  uartEsp->print("\"");
  return TimeOut(100, "OK");
}

boolean ESP8266WIFI::disableDHCP (void) { //NAO TA FUNCIONANDO
  int modo = getMode();
  uartEsp->print("AT+CWDHCP=\"");
  uartEsp->print(modo);
  uartEsp->print("\",\"");
  uartEsp->print(1);
  uartEsp->print("\"");
  return TimeOut(100, "OK");
}


boolean ESP8266WIFI::SendData (String data, int linkID, int timeout) {
  String parcial = "";
  int len = data.length();
  //Serial.print(len);
  uartEsp->print("AT+CIPSEND=");
  uartEsp->print(linkID);
  uartEsp->print(",");
  uartEsp->println(len);
  if(TimeOut(timeout, ">"))
    uartEsp->println(data);
  return TimeOut(timeout, "SEND OK");
}

String ESP8266WIFI::ReceiveData () {
  String retorno = "";
  retorno = uartEsp->readString();
  return FindAndFilter(retorno, ":");
}
boolean ESP8266WIFI::pooling() {
  if (uartEsp->available() > 0)
    return 1;
  else return 0;
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
