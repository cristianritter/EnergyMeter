/* * Nome do arquivo: funcoes_outras.h
 *  Este arquivo possui as principais funcoes do programa
 *
 */

#include "funcoes_OLED.h"

Pinger pinger;
extern "C"
{
  #include <lwip/icmp.h> // needed for icmp packet definitions}
}


boolean pingTest() {                                        //verifica conexao com internet
    Serial.println("Verificando conexão com a internet.");
    while(pinger.Ping("www.google.com") == false){
        delay(1000);
        Serial.print(".");
    }
    pinger.OnReceive([](const PingerResponse& response) {
        boolean ping_sucess = false;
        if (response.ReceivedResponse) {
            Serial.printf(
            "Reply from %s: bytes=%d time=%lums TTL=%d\n",
            response.DestIPAddress.toString().c_str(),
            response.EchoMessageSize - sizeof(struct icmp_echo_hdr),
            response.ResponseTime,
            response.TimeToLive);
            ping_sucess = true;
            Serial.println("Verificação de ping finalizada.");
        }
        else{
            Serial.printf("Request timed out.\n");
        }
        return !ping_sucess;   // Return true to continue the ping sequence. False to exit
    });
    display_msg("Conectado!");
}


boolean wifiConnect() {                                               //conexão à rede wifi / internet
    display_msg("Conectando...");
    WiFi.begin(ssid, password);
    Serial.println();
    Serial.print("Conexão Wifi: ");
    Serial.println(ssid);
    Serial.print("Tentando conexão..");  
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("Success!");
    Serial.print("IP Address is: ");
    Serial.println(WiFi.localIP());
    pingTest(); 
}


boolean threadTo(unsigned long* last_time, unsigned long default_time) {  //cria rotinas que rodam a cada ciclo de tempo especifico
    long now = millis();
    if (now - *last_time > default_time) {
        *last_time = now;
        return true;
    }  
    else 
        return false;  
}
