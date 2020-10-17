/* * Nome do arquivo: config.h
 *  Este arquivo armazena os principais defines
 *  
 */

#define deviceid "0"   // Nome do device é armazenado na coluna id da tabela DynamoDB

#define number_of_sample_types 7                                                           // Numero de parametros
#define $sample_types {"Tensao", "Freq", "Pot_At", "Pot_Ap", "Corrente", "FP", "Pot_Re"}   // Nome dos parametros 
#define $unidades_de_medida_parametro {"V", "Hz", "W", "VA", "A", " ", "VAr"}              // Unidades de medida
#define $precisao_decimal_parametro {1, 2, 0, 0, 2, 2, 0}                                  // Casas decimais
#define time_between_uploads 30000 //ms                        // Tempo entre uploads
#define debaunce_time 250                                      // Debaunce da chave do display
#define display_update_time 1000                               // Atualizacao do display
#define time_for_serial_prints 5000                            // Intervalo de impressão na Serial

#define serial_speed 115200        // Serial port speed
#define CLOCKIN 3579545            // Clock do ADE


#define SW_DISPLAY D0              // Chave para mudança de tela
#define SDA_PIN D1                 // SPI Pin
#define SCK_PIN D2                 // SPI Pin
#define CS_PIN D8                  // SPI Pin


//WIFI
const char* ssid = "PELI";         // WiFi network name
const char* password = "peli2018"; // WiFi network password


// Configuration for NTP
const char* ntp_primary = "pool.ntp.org";     // Servidores de fuso horário
const char* ntp_secondary = "time.nist.gov";
