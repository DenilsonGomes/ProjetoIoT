/*
 * Autor: Denilson Gomes Vaz da Silva
 * Graduando em Engenharia da Computação
 * Projeto de Internet das Coisas (IoT) - Prof. Dr. Wendley Silva
 * Codigo para monitorar 3 ou mais variaveis
 */

//- Importando Bibliotecas--//
#include "DHT.h"

//--Variaveis Globais--//
#define DHTPIN 2 // Pino do DHT
#define DHTTYPE DHT11 // Definindo o sensor DHT11
#define LDR A0 // Pino do LDR
int valorLDR = 0;

//--Instanciando Objetos--//
DHT dht(DHTPIN, DHTTYPE, 15); // Inicializando o sensor DHT

 void setup() {
  Serial.begin(9600); //Inicia comunicação serial
  dht.begin(); // Iniciando o DHT 

}

void loop() {
  int u = dht.readHumidity(); // Lendo a umidade
  if(u == 2147483647){delay(10);return;} //Verificando se houve falha na leitura
  
  int t = dht.readTemperature(); // Lendo a temperatura em graus Celsius

  Serial.println(String("Humidade: ") + String(u));
  Serial.println(String("Temperatura: ") + String(t));

  valorLDR = analogRead(LDR); // Atribui o valor da luminosidade no LDR1 para valorSensor1
  Serial.println(String("Luminosidade: ") + String(valorLDR));  
  
  delay(3000);
}
