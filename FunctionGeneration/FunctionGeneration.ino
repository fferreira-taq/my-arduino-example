//INSTOR - Projetos e Robótica//
//Rogério Ballestrin
//rogerio@instor.com.br

//********** Bibliotecas ******************************** //
#include "Waveforms.h"

//********** Variáveis Globais ************************** //
#define oneHzSample 1000000/maxSamplesNum  // Variavel sample calculada para frequencia de saida de 1Hz
int wave0 = 0, wave1 = 0;
int i = 0;
int sample;
int freq;

//********** Setups ************************************* //
void setup() {
  analogWriteResolution(12);  // Configura resolucao de saida analogica para 12bits (4096 pontos)
  Serial.begin(9600);
  imprimeComandos();
}
//********** Loop Infinito ****************************** //
void loop() {
  analogWrite(DAC0, waveformsTable[wave0][i]);  // Escreve a forma de onda em DAC0
  analogWrite(DAC1, waveformsTable[wave1][i]);  // Escreve a forma de onda em DAC1
  i++;
  if (i == maxSamplesNum) // Reseta o contador para comecar a escrever a onda novamente
    i = 0;
  delayMicroseconds(sample);  // Aguarda, conforme freq determinada, proximo periodo de alteracao da onda.
                              //TODO: Alterar para interrupcao de timer. 
}
//********** Imprime Menu ****************************** //
void imprimeComandos() {
  Serial.println("Comandos:");
  Serial.println("  1) Alterar Frequencia");
  Serial.println("  2) Alterar Tipo de Onda em DAC0");
  Serial.println("  3) Alterar Tipo de Onda em DAC1");
  Serial.println("     0 - Senoide");
  Serial.println("     1 - Triangular");
  Serial.println("     2 - Dente de Serra");
  Serial.println("     3 - Quadrada");
}
//********** Int Serial ****************************** //
void serialEvent() {
  int inByte;
  if (Serial.available() > 0) {
    inByte = Serial.parseInt();
    switch (inByte) {
      case 1:
        Serial.println("Digite a frequencia entre 0 e 4095");
        while (Serial.available() == 0) {}
        freq = Serial.parseInt();
        sample = map(freq, 0, 4095, 0, oneHzSample); //Altera o valor de freq para 0 a oneHzSample em vez de 0 a 4095
        sample = constrain(sample, 0, oneHzSample); //limita o valor de sample entre 0 e oneHzSample
        Serial.println("Frequencia alterada.");
        imprimeComandos();
        break;
      case 2:
        Serial.println("Digite o tipo de onda em DAC0");
        while (Serial.available() == 0) {}
        wave0 = Serial.parseInt();
        Serial.println("Tipo de onda alterada.");
        imprimeComandos();
        break;
      case 3:
        Serial.println("Digite o tipo de onda em DAC1");
        while (Serial.available() == 0) {}
        wave1 = Serial.parseInt();
        Serial.println("Tipo de onda alterada.");
        imprimeComandos();
        break;
      default:
        break;
    }
  }
}
