/*
 * Autores: Denilson Gomes Vaz da Silva, Raphael Carneiro de Freitas
 * Graduandos em Engenharia da Computação
 * Projeto de Internet das Coisas (IoT) - Prof. Dr. Wendley Silva

 ************************************************************************************************************
 *************************PINAGEM****************************************************************************
  
  RFID-RC522          Nodemcu
RST/Reset RST          D3 [1*]      GPIO-0  [1*]
SPI SS  SDA[3]         D8 [2*]      GPIO-15 [2*]
SPI MOSI MOSI          D7           GPIO-13
SPI MISO MISO          D6           GPIO-12
SPI SCK SCK            D5           GPIO-14

[1*] (1, 2) Configuracao tipica definindo como RST_PIN no sketch / programa.
[2*] (1, 2) Configuracao, tipica definindo como SS_PIN no sketch / programa.
[3*] O pino SDA pode ser rotulado como SS em algumas placas de MFRC522 / mais antigas, significam a mesma coisa.
******************************************************************************************************************
 */


#include <SPI.h>
#include <MFRC522.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define RST_PIN    D3    
#define SS_PIN     D8   
#define LED        D1

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


MFRC522 mfrc522(SS_PIN, RST_PIN); // Cria uma instancia MFRC522

void setup() 
{
  Serial.begin(9600);   // Inicia a serial

//-----------Conecta a rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");

//----------Conecta ao Broker MQTT
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT..."); 
    if (client.connect("ESP8266Client2", mqttUser, mqttPassword )) {
      Serial.println("connected");  
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  pinMode(D1, OUTPUT);
  pinMode(D0, OUTPUT); 

//Topicos nos quais o dispositivo está inscrito
  client.subscribe("ACESSO");
  client.subscribe("LED");
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
  }
}

void loop() 
{
  client.loop();
  // Procura por cartao RFID
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Seleciona o cartao RFID
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Mostra UID na serial
  Serial.print("UID da tag :");
  String conteudo= "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();

  if (conteudo.substring(1) == "BB 59 8B 89") // Cartao cujo UID será reonhecido
  {
    Serial.println("Liberado!");
    Serial.println();
    digitalWrite(D1, HIGH);     // Liga o buzzer que indica cartão reconhecido
    delay(200);                 // espera 200 milissegundos
    digitalWrite(D1, LOW);      // Desliga o buzzer - (1 bip)
    digitalWrite(D0, HIGH);     // Liga o led que indica usuário autenticado 
    client.publish("ACESSO","ATIVASISTEMA"); //Envia uma mensagem ao broker para ligar o sistema
    delay(2000);//Espera dois segundos para ler novamente
    
  } else{
    Serial.println("Acesso Negado!");
    digitalWrite(D1, HIGH);     
    delay(100);              
    digitalWrite(D1, LOW);  
    delay(100);
    digitalWrite(D1, HIGH);     
    delay(100);              
    digitalWrite(D1, LOW);  
    delay(100);
    digitalWrite(D1, HIGH); // Três bips rapidos para informar que o cartão não foi reconhecido    
    delay(100);              
    digitalWrite(D1, LOW);
    digitalWrite(D0, LOW);//Desativa led que indica que o sistema está funcionando  
    client.publish("ACESSO","DESLIGASISTEMA"); // Publica a mensagem no broker para desligar o sistema
    delay(2000);//Espera dois segundos para ler novamente
    }
}// fim do loop
