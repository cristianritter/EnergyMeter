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
MIIDWjCCAkKgAwIBAgIVALIRrBK80/BSneYrYppHskrxK2hDMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTEwMjkwMTA4
MDhaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDBibFKn4bAftVvmT5i
U31D3M8Zwc6lNU41lMjdJesKteaW6Hcz9OH1GcavWzU3pvoSc7dRDzwOh7nbwgWd
5IhCm44GxTbhnipS74ok6qAB4wfH4HvUEcYLPkiZrr5ratTZhZiYnKOmb9W9ZrJa
CxiLInOMGFayCcSi8gfuqCc3yL4YYgB8gRtvzGJYJmm9wdF4nRhad6mgmGGG+06q
u5sgm0fKfEVznR3ee7FvtbziUeklZGSd+bHw/XdBHk4IIwpNeTbFpD9X9ipFoo6f
S/Sq8VbWPWs6cRkvLMxaEao94tXDIXy3sSTzLpCuVjj5nL8oYWx3gie0xrgDNJnR
2sfpAgMBAAGjYDBeMB8GA1UdIwQYMBaAFHLlKsDkqxUiWCo8zGYwSfLEjiQ0MB0G
A1UdDgQWBBRCcc5Kp8JlLQdzPu+LMyGkkgxJhDAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAnAbXUrk7raSJ2OWlXQ9+VmDw
HSvJOOt2h3lGpAQ6QavbR+wDFXxvYkIX0bMRCSOV/k9OpEm0pcXaxZa4Psa+gmfR
UJdzXHUsBN6stTiiEKlSKXFDysD9dTZtl1aTiaobWjiZlRcUzGBSc2sbqTwYoB0H
JNUGKS1Yghz1GC4m2A44gEXYNsa8tedRZessFRUWirboyn2fhZLqFz+ETeJxtlvD
69qB3Q2+u3+VAPpOKZbcN97Lebjya7j9tMfyq1Pelng05z1RMxUhMDTDQZs+IM0e
uO6IaTZDh0xMmd7h5HchWfmvFk8XyBu7E+QPXEKblqkokhq29INazLyC94rwYg==
-----END CERTIFICATE-----
)EOF";
// : xxxxxxxxxx-private.pem.key)


const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEAwYmxSp+GwH7Vb5k+YlN9Q9zPGcHOpTVONZTI3SXrCrXmluh3
M/Th9RnGr1s1N6b6EnO3UQ88Doe528IFneSIQpuOBsU24Z4qUu+KJOqgAeMHx+B7
1BHGCz5Ima6+a2rU2YWYmJyjpm/VvWayWgsYiyJzjBhWsgnEovIH7qgnN8i+GGIA
fIEbb8xiWCZpvcHReJ0YWnepoJhhhvtOqrubIJtHynxFc50d3nuxb7W84lHpJWRk
nfmx8P13QR5OCCMKTXk2xaQ/V/YqRaKOn0v0qvFW1j1rOnEZLyzMWhGqPeLVwyF8
t7Ek8y6QrlY4+Zy/KGFsd4IntMa4AzSZ0drH6QIDAQABAoIBAQC5H9xWZQGgX7IJ
baweoX3QzAXNvXTM0pBc8FSyNPL+68AF2ZcsO7FnoaBLXBMriKzruYRAJTJtM6a0
boKw5UjxbZoNH9e/OGuDqV2aslMbeu6WYkQQNnAZkRog5uV+GlI/P2sXj716Fr98
zB8Nwk9Xxzx/ZUiIRI+yzZCI2I+q6svs7sw+rPoDxRjoGxWDxXiYKVPsj8TPI3fD
cHdt2bYV/mUHliPp8Hez9cb6HeWpAns1mgCXhKPtQ3jaT4nhmwyHKc2IbgBDUIiV
g9muen2EGpevwu4AJ1JsYI48/AtTFHKyTa/fMmt0Gq7YQZZIzH80stxYYtHnhlOV
EL7WMOABAoGBAP/0QN9TZ3BUePyVURf8SwFwVolQfioGMUUUj8rx+TJf0aXY0suD
JosxR1qafnAOQ7cuMCT3kqwD4vyhUIdm1DRq+P1CwG/YeV9n3MF1CCQoPFE7BVjo
sEhorIFDAEBYgT9yPeKC6PT97SEE9NK9mdLkb2OObtNDKVA6iG/GjS7JAoGBAMGS
kxwCUPm1Af+EKAyAJSc0b+9CiyJu+8uov7FV5WpB0JQywoUL9AScSrS54lwW7gfr
WzsiqB6um9N+tTDux8lUn4DJmvykZqi8AbiS2zjtw++9nAzReFfyl9ZnZMC8WVSw
BZHruR2i5yAANwAAR6CY8ZK7FHbFJoHwQCRlicAhAoGBANP8cWpFwqsXW2kfJh3u
eBTF/PYXc5bMsCvdvsZAKLte/t0UOmoRwm+QLd/NiyC1v8HMgFVtss3DHlFMLc0n
DmQnoPbpPUalDMlG1WBo1IhVxrckYeZkOfSOtSkN2/xvEQUCUlzzPYwDb01ktHyC
Sg+P4O2cvw88VdA72lpxIOd5AoGAP5nGRhSowHLttiOY+Qp9qpT7FjTZ/DVU/AR0
/3JGj5QSUS8gZJCZ243yqdDNhA5wpR96eUKY1IPwZRwjsnEyKX9pbOpQxyPJKzWi
px9xchIFJ9c316pXjSYDJ4oPO5hnDYFMQzadOxSUNhp7978fya+W5+wZ/DAox9bF
bHAEQ6ECgYEArRsx8852744dlah+XxUoXEJ+QelgcI4qKV8S6E3yVXid36WteA1K
Z4OSgccOmXaspB1BRVwSqGoT0vuKjZz0shVl0XVtLJbho0Au/p5nFvnBMuHdqw6R
CNEYvGPGA0MyhWZq5psyM/zlrWWuXMKnNTAK6IQw4l9gcX7ADg1acEI=
-----END RSA PRIVATE KEY-----
)EOF";


// Amazon Trust Services(ATS) CA > "RSA 2048: Amazon Root CA 1" )
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
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
