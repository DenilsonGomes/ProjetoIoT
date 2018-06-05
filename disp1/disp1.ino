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

//--Instanciando Objetos--//
DHT dht(DHTPIN, DHTTYPE, 15); // Inicializando o sensor DHT

 void setup() {
  Serial.begin(9600);
  // Iniciando o DHT 
  dht.begin();

}

void loop() {
  // Lendo a umidade
  int u = dht.readHumidity();

  //Verificando se houve falha na leitura
  if(u == 2147483647){
    delay(10);
    return;
  }
  
  // Lendo a temperatura em graus Celsius
  int t = dht.readTemperature();

  Serial.println(String("Humidade: ") + String(u));
  Serial.println(String("Temperatura: ") + String(t));

  delay(3000);
}
