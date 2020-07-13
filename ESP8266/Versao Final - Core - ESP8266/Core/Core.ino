/* * Nome do arquivo: Core.h
 * Este é o arquivo main() que possui o setup e o loop principal do programa
 * 
 * Feito por Cristian Fernando Ritter
 * Para o trabalho de conclusão de curso de Engenharia Mecatrônica IfSC - Florianópolis
 * Todos os direitos reservados
 */


#include "Config.h"           // Arquivo com diversas definicoes do sketch
#include <Pinger.h>           // Biblioteca de suporte a ping
#include <ESP8266WiFi.h>      // Bibliote de suporte a wifi do módulo ESP8266
#include <SPI.h>              // Biblioteca de suporte a comunicação SPI
#include "SSD1306Wire.h"      // Bibliote de suporte ao Display OLED
//#include <PubSubClient.h>     // Biblioteca para publicacao MQTT na AWS
#include <time.h>             // Biblioteca com funções relacionadas a fuso horário
#include "funcoes_MQTT.h"     // Funcoes do autor relacionadas ao MQTT
#include "funcoes_outras.h"   // Funcoes diversas desenvolvidas pelo autor
#include "funcoes_ADE7753.h"  // funcões dos registradores ADE7753


void setup()
{
  Serial.begin(serial_speed);            // Inicia comunicação Serial.
  Serial.setDebugOutput(true);
  displayInit();                         // Inicializa displayOLED
  wifiConnect();                         // conecta a rede Wifi
  set_certifications();                  // certificações relacionadas a conta na aws
  setClock();                            // atualiza hora do sistema (para a autenticar o certificado e gerar o timestamp)
  client.setCallback(callback);          // Permite a recepção de msg pelo Tópico MQTT
  pinMode(SW_DISPLAY, INPUT);            
  pinMode(CS_PIN,OUTPUT);                
  digitalWrite(CS_PIN, HIGH);            // habilita comunicação no ADE7753 - disabled by default
  SPI.begin();                           // Inicia comunicação SPI
  configADE();                           // Configura registradores do ADE7753
}


unsigned long last_upload_time = 0;         // Uso interno nos threads
unsigned long last_debaunce_time = 0;         
unsigned long last_display_update_time = 0;   
unsigned long last_serial_print_time = 0;     

char msg_to_publish[1000];                 // Armazena a string a ser enviada para o broker
unsigned long status_register = 0;         // Armazena o valor do registrador de status do ADE
int display_current_view = 4;              // Armazena a informacao da tela apresentada no display
        

void loop() {
    if (!client.connected()) {             // Se desconectar do broker tenta reconexão
      reconnect();
    }
    client.loop();

  
    while(!statusRESET) {                  // Aguarda que o ADE esteja pronto para operar
      // this breaks when reset ends
    }

    
    retStatusRegister(&status_register);     // Atualiza status global do ADE a cada loop main

    
    if (statusWSMP(&status_register)){       //new data on wavefor
        current[0] = retV();            //volt
        current[1] = retHz();           //freq
        current[2] = retW();            //pot ativa
        current[3] = retVA();     //potencia aparente
        current[4] = retI();            //current
        current[5] = retFP();           //fat de pot  
        current[6] = retVAr();      //potencia reativa
    }

    
    if (!digitalRead(SW_DISPLAY) && threadTo(&last_debaunce_time, debaunce_time)) {      
        display_current_view++;       
        if (display_current_view == (number_of_sample_types)) { 
            display_current_view=0; 
        }
        displayUpdate(current,display_current_view);                 // Muda tela do display
    }

    
    if(threadTo(&last_display_update_time, display_update_time)) {   // Atualiza o display com as informações mais atuais
        displayUpdate(current,display_current_view);
        Serial.println(read8(CH1OS));
    }


    if(threadTo(&last_serial_print_time, time_for_serial_prints)) {     // Atualiza o display com as informações mais atuais
        imprimir_na_serial = true;
    }
  
  
    if (threadTo(&last_upload_time, time_between_uploads)) {         // Faz upload das informações mais recentes
        createMessage(msg_to_publish, current);
        publish_message(msg_to_publish); 
    }
}
