 
 ************************************************************************************************************
 *************************PINAGEM****************************************************************************
  
  RFID-RC522   Wemos          Nodemcu
RST/Reset RST  D3  [1*]        D3 [1*]      GPIO-0  [1*]
SPI SS  SDA[3] D8  [2*]        D8 [2*]      GPIO-15 [2*]
SPI MOSI MOSI  D7              D7           GPIO-13
SPI MISO MISO  D6              D6           GPIO-12
SPI SCK SCK    D5              D5           GPIO-14

[1*] (1, 2) Configuracao tipica definindo como RST_PIN no sketch / programa.
[2*] (1, 2) Configuracao, tipica definindo como SS_PIN no sketch / programa.
[3*] O pino SDA pode ser rotulado como SS em algumas placas de MFRC522 / mais antigas, significam a mesma coisa.
******************************************************************************************************************
 */


#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN    D3    
#define SS_PIN     D8   
#define LED        D1

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() 
{
  Serial.begin(9600);   // Inicia a serial
  SPI.begin();      // Inicia  SPI bus
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o seu cartao do leitor...");
  Serial.println();
  //digitalWrite(D1,LOW);
  pinMode(D1, OUTPUT);
  pinMode(D0, OUTPUT); 
}

void loop() 
{
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

  //if (conteudo.substring(1) == "01 D1 FC 52" || "03 1C A0 F6") //UID 1 - Cartao
  if (conteudo.substring(1) == "BB 59 8B 89") //UID 1 - Cartao
  {
    Serial.println("Liberado!");
    Serial.println();
    digitalWrite(D1, HIGH);     // LIGA LED OU/ ativa rele, abre trava solenoide
    delay(200);              // DELAY /espera 3 segundos
    digitalWrite(D1, LOW);  // DESlIGA LED OU /desativa rele, fecha  trava solenoide
    digitalWrite(D0, HIGH);//Ativa led que indica que o sistema está funcionando 
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
    digitalWrite(D1, HIGH);     
    delay(100);              
    digitalWrite(D1, LOW);  
    delay(2000);//Espera dois segundos para ler novamente
    }
}
