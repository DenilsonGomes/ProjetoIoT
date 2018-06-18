/*
 * Autores: Denilson Gomes Vaz da Silva, Raphael Carneiro de Freitas
 * Graduando em Engenharia da Computação
 * Projeto de Internet das Coisas (IoT) - Prof. Dr. Wendley Silva
 * Codigo para monitorar 3 ou mais variaveis
 */


#include <ESP8266WiFi.h>
#include <PubSubClient.h> // Biblioteca usada, baixe e instale se não a tiver, link abaixo
#include <DHT.h>                          //https://github.com/knolleary/pubsubclient/blob/master/examples/mqtt_esp8266/mqtt_esp8266.ino

//Define a pinagem do ESP8266

#define D0    16 //pino do led
#define D1    5 //pino do sensor de som
#define D2    4 // pino dht11
#define D3    0
#define D4    2
#define D5    14
#define D6    12
#define D7    13
#define D8    15
#define D9    3
#define D10   1

#define pinPIR D3 // Pino do sensor de presença PIR
#define pinSound D1 // Pino do sensor de som

#define DHTTYPE DHT11

DHT dht(D2, DHTTYPE, 15);
bool valorPIR;
int valorSOM = 0; //Variavel para sinal do sensor de som
 
const char* ssid = "ap201"; //Aqui o nome da sua rede local wi fi
const char* password =  "solpa050709"; // Aqui a senha da sua rede local wi fi
const char* mqttServer = "m10.cloudmqtt.com"; // Aqui o endereço do seu servidor fornecido pelo site 
const int mqttPort =12217; // Aqui mude para sua porta fornecida pelo site
const char* mqttUser = "zpwwnbka"; //  Aqui o nome de usuario fornecido pelo site
const char* mqttPassword = "vgNRAVKehHWL"; //  Aqui sua senha fornecida pelo site
//char EstadoSaida = '0';  
WiFiClient espClient;
PubSubClient client(espClient);


void mqtt_callback(char* topic, byte* payload, unsigned int length);


void setup() {
    dht.begin();
    pinMode(D0, OUTPUT);
    digitalWrite(D0, LOW); 
    pinMode(D2, INPUT);
    digitalWrite(D2, LOW);
    pinMode(pinPIR, INPUT);
    digitalWrite(pinPIR, LOW);
    //pinMode(D2, OUTPUT);
    //digitalWrite(D2, LOW);  

 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
  
 
  client.publish("LEDcor","#ffffff");
  client.subscribe("LED");
  client.subscribe("PRES");
  client.subscribe("SOM");
  client.subscribe("TEMP");
  client.subscribe("UMID");
  client.subscribe("LEDcor");
 
}
 // LENDO A TEMPERATURA E PUBLICANDO
    void leTemperatura(void){
     // digitalRead(D2);
      int t = dht.readTemperature();
      char t2[3];
      String str =String(t);
      str.toCharArray(t2,3);
       client.publish("TEMP",t2);
      // delay(20000);     
    }
    
 // LENDO A UMIDADE E PUBLICANDO
    void leUmidade(void){
     // digitalRead(D2);
      int u = dht.readHumidity();
      char u2[3];
      String str =String(u);
      str.toCharArray(u2,3);
       client.publish("UMID",u2);
      // delay(20000);
    }

// DETECTANDO PRESENÇA E PUBLICANDO
    void detectaPres(void){
     valorPIR = digitalRead(pinPIR); // Atribui o valor da leitura digital de pinPIR para valorPIR
        if(valorPIR){
          Serial.println("Presença detectada");
          client.publish("PRES","PRESENÇA DETECTADA");
       }else{
          Serial.println("Presença não detectada");
          client.publish("PRES","PRESENÇA NÃO DETECTADA");
          }
    }
 
 // DETECTANDO SOM E PUBLICANDO
    void detectaSom(void){
        valorSOM = digitalRead(pinSound); // Atribui o valor de som para valorSOM
        Serial.println(String("Som: ") + String(valorSOM));
        char som[3];
        String strsom =String(valorSOM);
        strsom.toCharArray(som,3);
        client.publish("SOM",som);
    }
    
    
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Mensagem recebida no topico: ");
  Serial.println(topic);
 
  Serial.print("Mensagem:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);

       String msg;
    
 
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
  

        
    //toma ação dependendo da string recebida:
    //verifica se deve colocar nivel alto de tensão na saída.
    //IMPORTANTE: o Led já contido na placa é acionado com lógica invertida (ou seja,
    //enviar HIGH para o output faz o Led apagar / enviar LOW faz o Led acender)
 
 
    //verifica se deve colocar nivel alto de tensão na saída se enviar L e digito, ou nivel baixo se enviar D e digito no topíco LED

    
       if (msg.equals("L0"))
    {
        digitalWrite(D0, HIGH);
        client.publish("LEDcor","#ffff00");
        
       // EstadoSaida = '0';
    }
       if (msg.equals("D0"))
    {
        digitalWrite(D0, LOW);
         client.publish("LEDcor","#ffffff");
       // EstadoSaida = '0';
    }
         if (msg.equals("l1"))
    {
       leTemperatura();
    }
         if (msg.equals("l2"))
    {
       leUmidade();
    }
         if (msg.equals("l3"))
    {
       detectaPres();
    }
        if (msg.equals("l4"))
    {
       detectaSom();
    }
    

       
    
     }
   
 
  Serial.println();
  Serial.println("-----------------------------");
 
}
 
void loop() {
  client.loop();
  
  
}
