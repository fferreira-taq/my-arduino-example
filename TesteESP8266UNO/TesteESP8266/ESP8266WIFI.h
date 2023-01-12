
#ifndef ESP8266WIFI_H
#define ESP8266WIFI_H

#include "Arduino.h"
#include <SoftwareSerial.h>

#define SSTX         2
#define SSRX         3
 
#define STA          1
#define AP           2
#define STA_AP       3

#define OPEN         0
#define WEP          1
#define WPA_PSK      2
#define WPA2_PSK     3
#define WPA_WPA2_PSK 4

#define SINGLE       0
#define MULTIPLE     1
#define DEBUG        0


class ESP8266WIFI {
  private:
   
    //HardwareSerial *uartEsp;
    String FindAndFilter (String data, String inicio, String fim);
    String FindAndFilter (String data, String inicio);
    boolean  TimeOut(int timeout, String target,boolean debug);
   // String   TimeOut(int timeout, String target,boolean debug);
    void ClearSBuffer(void);
    boolean ReadSBuffer(String &parcial , String target,int timeout);
    //String ReceiveWTimeout(int time, String Msg);
  
  
  public:
    /**
    * Construtor da classe
    */
    ESP8266WIFI();
    
    
    void init(int baud = 115200);
    /**
    * Verifica se a placa está respondendo aos comandos AT
    *
    * @return true - Funcionando.
    * @return false - Sem resposta.
    */
    boolean TesteAT (void);
    /**
     * Restore ESP8266, comando "AT+RESTORE".
     *
     * @return true - Sucesso.
     * @return false - Falha.
     */
    boolean Reset (void);
    boolean setMode(int cwmode);
    /**
    * Consulta a versão dos comandos AT.
    *
    * @return String com a versao e data.
    */
    boolean QuitAP(void);
    /**
    * Cria uma AP
    *
    * @parametro ssid - SSID da rede que deseja criar.
    * @parametro pwd - Senha da rede que deseja criar.
    * @parametro channel - O canal que sera usado para a rede. Valores de 1-13.
    * @parametro sec - Tipo de encriptação (OPEN,WEP,WPA_PSK,WPA2_PSK,WPA_WPA2_PSK).
    * @nota: Metodo deve ser usado apenas no modo AP (CWMODE=2 ou 3), caso contrario retorna false.
    */
    boolean createAP(String ssid, String pwd, int chan, int sec);

    boolean enableDHCP (void);//NAO TA FUNCIONANDO
    boolean disableDHCP (void);//NAO TA FUNCIONANDO
    boolean setMux(int mux);
    boolean createUDP(int id, String remote_IP, int remote_Port, int localPort);
    boolean SendData(String data, int linkID, int timeout);
    boolean pooling();
    
    int getMode(void);
    
    String ReceiveData (int timeout);
    String Status (int timeout);
    String listClient (int timeout);
    String getIP(int timeout);
    String getVersion(int timeout);
    /**
     * Procura APs disponiveis e retorna a lista
     *
     * @return String com todas as APs ao alcance.
     * @nota: Este metodo ocupa muita memoria, pois a lista de APs disponiveis pode ser grande.
     */
    String listAP(int timeout);
    /**
    * Connect in AP.
    *
    * @parametro ssid - SSID da rede que deseja se conectar.
    * @parametro pwd - Senha da rede que deseja se conectar.
    * @return String com o ocorrido: "ERROR. CWMODE=2.", "Timeout WIFI", "Timeout DHCP";
    */
    String connectAP(String ssid, String pwd, int timeout);
    /**
    * Sai de uma rede previamente conectada.
    *
    * @return true - Sucesso.
    * @return false - Falha.
    */

};
#endif

