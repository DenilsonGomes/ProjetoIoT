# Sistema de Controle e Monitoramento Remoto
## Autor: Denilson Gomes Vaz da Silva
# Sumário 
 1. [Introdução](#chap1)
 2. [Visão Geral](#chap2)
 3. [Artigo](#chap3)

*******

<div id='chap1'/>  

1.Introdução
==========

<p>Esse projeto tem a intenção de monitorar e controlar algumas variavéis de forma remota.
Neste exemplo optamos por monitorar a temperatura, a umidade, presença e luminosidade.
Foi utilizado sensores para monitorar essas grandezas fisicas. Cada sensor pode ser controlado,
ligado ou desligado, também remotamente.<p/>

---
<div id='chap2'/>

2.Visão Geral
==========
<p></p>
<p>O sistema desenvolvido consiste de 2 esp8266 que se comunicam com um servidor broker MQTT.
Um esp8266 está conectado aos sensores e envia os dados dos mesmo para o servidor broker MQTT.
O outro esp8266 faz a leitura de RFID e quando passa uma tag cadastrada ele ativa o sistema.
Dessa forma, o sistema so liga os sensores quando uma tag RFID cadastrada deseja monitorar as
variaveis sensoriadas.
O servidor broker MQTT escolhido foi o CloudMQTT. Após a publicação no broker MQTT pode-se
inscrever no mesmo atraves de outros dispositivos. Neste projeto fizemos o uso do MQTT Dash,
disponivel para Andoid e IOS.

---

<div id='chap3'/>

3.Artigo
==========
<p></p>
<p>Foi desenvolvido um artigo para aproveitamento deste projeto na disciplina de Internet das Coisas, do curso de Engenharia de Computação da UFC do campus Sobral. O artigo desenvolvido pode ser visto no link a seguir.

[Artigo](https://drive.google.com/open?id=1mk14w89wjMI3-fXUGNsRDA7_hJkSCFa0)
