/*
 * Autor: Denilson Gomes Vaz da Silva
 * Graduando em Engenharia da Computação
 * Projeto de Internet das Coisas (IoT) - Prof. Dr. Wendley Silva
 * Codigo para monitorar 3 ou mais variaveis
 */

//- Importando Bibliotecas--//
#include "DHT.h"

//--Variaveis Globais--//
#define pinDHT 2 // Pino do DHT
#define DHTTYPE DHT11 // Definindo o sensor DHT11
#define pinLDR A0 // Pino do LDR
int valorLDR = 0; //Variavel para sinal do LDR
#define pinSound A1 // Pino do sensor de som
int valorSOM = 0; //Variavel para sinal do sensor de som
#define pinPIR 3 // Pino do sensor de presença PIR
bool valorPIR;

//--Instanciando Objetos--//
DHT dht(pinDHT, DHTTYPE, 15); // Inicializando o sensor DHT

 void setup() {
  Serial.begin(9600); //Inicia comunicação serial
  dht.begin(); // Iniciando o DHT 
  pinMode(pinLDR, INPUT);
  pinMode(pinSound, INPUT);
  pinMode(pinPIR, INPUT);
}

void loop() {
  int u = dht.readHumidity(); // Lendo a umidade
  if(u == 2147483647){delay(10);return;} //Verificando se houve falha na leitura
  int t = dht.readTemperature(); // Lendo a temperatura em graus Celsius

  Serial.println(String("Humidade: ") + String(u));
  Serial.println(String("Temperatura: ") + String(t));

  valorLDR = analogRead(pinLDR); // Atribui o valor da luminosidade no LDR para valorLDR
  Serial.println(String("Luminosidade: ") + String(valorLDR));  

  valorSOM = analogRead(pinSound); // Atribui o valor de som para valorSOM
  Serial.println(String("Som: ") + String(valorSOM));

  valorPIR = digitalRead(pinPIR); // Atribui o valor da leitura digital de pinPIR para valorPIR
  if(valorPIR){
    Serial.println("Presença detectada");
  }else{
    Serial.println("Presença não detectada");
  }
  Serial.println(valorPIR);
  delay(8000);
}
