/* * Nome do arquivo: funcoes_MQTT.h
 * Este arquivo inclui as principais funcoes relacionadas ao broker AWS 
 *  
 */


// Cloud iot details.
const char *thingId = deviceid;          // (device ID) 
const char *host = "a6ufkvuua7imp-ats.iot.us-east-2.amazonaws.com";   // AWS IoT Core host


// : xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(                     // Certificados de autenticação
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";
// : xxxxxxxxxx-private.pem.key)


const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)EOF";


// Amazon Trust Services(ATS) CA > "RSA 2048: Amazon Root CA 1" )
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

//utilizado para montar o payload JSON
#define pattern_begin \            
"{\"id\" : \"%s\",\n\
\"timestamp\" : \"%s\",\n"


#define pattern_end \
"\n\
}"


              
String setClock() {        // Set time via NTP, as required for x.509 validation  
  configTime(0 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
      delay(500);
      Serial.print(".");
      now = time(nullptr);
  }
  Serial.println("Done.");
  return String(now);  
}


void callback(char* topic, byte* payload, unsigned int length) {   // Define calback - utilizado para receber mensagens do tópico
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}


X509List ca(ca_str);                                    // Instalação dos certificados
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;                            // define a wi-fi session
//PubSubClient client(host, 8883, callback, wifiClient);  // Set  MQTT port number to 8883 as per //standard


void set_certifications(){                              // Seta certificações necessárias para uso dos serviços AWS
    wifiClient.setTrustAnchors(&ca);
    wifiClient.setClientRSACert(&cert, &key);
    Serial.println("Certifications and key are set");
}


void reconnect() {    
    while (!client.connected()) {                       // Loop until we're reconnected
        Serial.print("Attempting MQTT connection...");
        if (client.connect(thingId)) {                  // Attempt to connect
            Serial.println("connected");
            client.subscribe("inTopic");                // ... and resubscribe
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            char buf[256];
            wifiClient.getLastSSLError(buf,256);
            Serial.print("WiFiClientSecure SSL error: ");
            Serial.println(buf);
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

int indice_busca;
int indice_anterior;
long timestamp_base = 0;
void createMessage(char* msg_to_publish, float dados[]){     //Monta a string JSON a ser enviada
    char buff[100];
    char timestamp[25];
    const String tipo_amostra[number_of_sample_types] = $sample_types; 
    char tipo_amostra_atual[20];
    String timeString = setClock();
    strcpy(msg_to_publish, "");
    timeString.toCharArray(timestamp, 25);
    snprintf(buff, 100, pattern_begin, thingId, timestamp);
    strcat(msg_to_publish,buff);
    for (int j=0; j<number_of_sample_types; j++){
        tipo_amostra[j].toCharArray(tipo_amostra_atual, 20);
        snprintf(buff, 50, "\"%s\" : ",tipo_amostra_atual);
        strcat(msg_to_publish,buff);    
        snprintf(buff, 50, "\"%.2f\"",dados[j]);
        strcat(msg_to_publish,buff);
        snprintf(buff, 10, ",\n");
        strcat(msg_to_publish,buff);    
    } 
    if (timestamp_base == 0){
        timestamp_base = atol(timestamp);
        indice_anterior = 0;
    }
    if ( atol(timestamp) >= (timestamp_base + 7200)){ // a cada duas horas
        indice_busca = 1234;
        timestamp_base = 0;
    }
    else if ( atol(timestamp) >= (timestamp_base + 900) && indice_anterior < 123 ){ // a cada quinze minutos
        indice_busca = 123;
        indice_anterior = indice_busca;
    }
    else if ( atol(timestamp) >= (timestamp_base + 300) && indice_anterior < 12){ // a cada cinco minutos
        indice_busca = 12;
        indice_anterior = indice_busca;
    }
    else {
        indice_busca = 1;
    }   

    snprintf(buff, 30, "\"indice\" : \"%d\"", indice_busca);
    strcat(msg_to_publish,buff); 
    snprintf(buff, 10, pattern_end);
    strcat(msg_to_publish,buff);    
}

void publish_message(char* msg_to_publish){   //Publica a string no serviço da AWS
    Serial.print("Publishing message: ");
    Serial.println(msg_to_publish);
    client.beginPublish("aws/rules/Energy_meter", String(msg_to_publish).length(), false);
    client.print(msg_to_publish);
    client.endPublish();
    Serial.print("Heap: "); 
    Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
    Serial.println("");
}
