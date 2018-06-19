/*
 * Autores: Denilson Gomes Vaz da Silva, Raphael Carneiro de Freitas
 * Graduandos em Engenharia da Computação
 * Projeto de Internet das Coisas (IoT) - Prof. Dr. Wendley Silva
 * Codigo para monitorar 3 ou mais variaveis
 */


#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>                          

//Define a pinagem do ESP8266

#define D0    16 //Pino do led
#define D1    5 
#define D2    4  // Pino dht11
#define D3    0  // Pino do Sensor de Presença
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

DHT dht(D2, DHTTYPE);
bool valorPIR;
int valorSOM = 0; //Variavel para sinal do sensor de som
 
const char* ssid = "ap201"; //Aqui o nome da sua rede local wi fi
const char* password =  "solpa050709"; // Aqui a senha da sua rede local wi fi
const char* mqttServer = "m10.cloudmqtt.com"; // Aqui o endereço do seu servidor fornecido pelo site 
const int mqttPort =12217; // Aqui mude para sua porta fornecida pelo site
const char* mqttUser = "zpwwnbka"; //  Aqui o nome de usuario fornecido pelo site
const char* mqttPassword = "vgNRAVKehHWL"; //  Aqui sua senha fornecida pelo site

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
    pinMode(D8, OUTPUT);
    digitalWrite(D8, LOW);
    

  Serial.begin(115200);

 //------Conecta a rede Wi-Fi
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

 //------Conecta ao Broker MQTT
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
  
 //Topicos nos quais o dispositivo está inscrito
  client.publish("LEDcor","#ffffff");
  client.subscribe("LED");
  client.subscribe("PRES");
  client.subscribe("TEMP");
  client.subscribe("UMID");
  client.subscribe("LEDcor");
  client.subscribe("LUZ");
  client.subscribe("ACESSO");
  
 
}
 // LENDO A TEMPERATURA E PUBLICANDO
    void leTemperatura(void){
    // while(1){
      int t = dht.readTemperature();
      char t2[3];
      String str =String(t);
      str.toCharArray(t2,3);
      client.publish("TEMP",t2);
      //delay(20000);
     //}
    }
    
 // LENDO A UMIDADE E PUBLICANDO
    void leUmidade(void){
    // while(1){
      int u = dht.readHumidity();
      char u2[3];
      String str2 =String(u);
      str2.toCharArray(u2,3);
      client.publish("UMID",u2);
      //delay(20000);
     //}
    }

// DETECTANDO PRESENÇA E PUBLICANDO
    void detectaPres(void){
      //while(1){
     valorPIR = digitalRead(pinPIR); // Atribui o valor da leitura digital de pinPIR para valorPIR
        if(valorPIR){
          Serial.println("Presença detectada");
          client.publish("PRES","DETECTADA");
       }else{
          Serial.println("Presença não detectada");
          client.publish("PRES","NAO DETECTADA");
          }
        //  delay(20000);
      //}
    }
 
 // DETECTANDO LUZ E PUBLICANDO
    void detectaLuz(void){
     // while(1){
      int intensidade = analogRead(A0);   // read the input on analog pin 0
      char v2[5];
      String str3=String(intensidade);
      str3.toCharArray(v2,5);
      Serial.println(intensidade);   // print out the value you read
      client.publish("LUZ",v2);
      //delay(20000);
      //}
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
  
    //verifica as mensagens recebidas e ações a serem tomadas.
    
       if (msg.equals("L0"))// Liga o Led
    {
        digitalWrite(D0, HIGH);
        client.publish("LEDcor","#ffff00");
    }
       if (msg.equals("D0"))// Desliga o Led
    {
        digitalWrite(D0, LOW);
         client.publish("LEDcor","#ffffff");
    }

       if (msg.equals("ATIVASISTEMA"))//Ativa o Sistema
    {
        digitalWrite(D8, HIGH);    
    }
       if (msg.equals("DESLIGASISTEMA"))//Desliga o Sistema
    {
        digitalWrite(D8, LOW);
         client.publish("LEDcor","#ffffff");
    }
    
         if (msg.equals("l1"))
    {   
       leTemperatura();// Lê a Temperatura 
    }
        
         if (msg.equals("l2"))// Lê a Umidade
    {
       leUmidade();
    }
         if (msg.equals("l3"))//Lê o sensor de presença
    {
       detectaPres();
    }
        if (msg.equals("l4"))// Lê o sensor de Luz
    {
       detectaLuz();
    }
    
     }//fecha o for
 
  Serial.println();
  Serial.println("-----------------------------");
 
}
 
void loop() {
  client.loop();  
}
