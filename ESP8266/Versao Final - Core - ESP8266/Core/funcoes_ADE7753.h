/* * Nome do arquivo: funcoes_ADE7753.h
 * Este arquivo armazena funcoes relacionadas a R/W de registradores do ADE7753
 * 
 */

#include <math.h>

// Lista de registradores 
#define WAVEFORM 0x01
#define AENERGY 0x02
#define RAENERGY 0x03
#define LAENERGY 0x04
#define VAENERGY 0x05
#define RVAENERGY 0x06
#define LVAENERGY 0x07
#define LVARENERGY 0x08
#define MODE 0x09
#define IRQEN 0x0A
#define STATUS 0x0B
#define RSTSTATUS 0x0C
#define CH1OS 0x0D
#define CH2OS 0x0E
#define GAIN 0x0F
#define PHCAL 0x10
#define APOS 0x11
#define WGAIN 0x12
#define WDIV 0x12
#define CFNUM 0x14
#define CFDEN 0x15
#define IRMS 0x16
#define VRMS 0x17
#define IRMSOS 0x18
#define VRMSOS 0x19
#define VAGAIN 0x1A
#define VADIV 0x1B
#define LINECYC 0x1C
#define ZXTOUT 0x1D
#define SAGCYC 0x1E
#define SAGLVL 0x1F
#define IPKLVL 0x20
#define VPKLVL 0x21
#define IPEAK 0x22
#define RSTIPEAK 0x23
#define VPEAK 0x24
#define RSTVPEAK 0x25
#define TEMP 0x26
#define PERIOD 0x27
#define TMODE 0x3D
#define CHKSUM 0x3E
#define DIEREV 0x3F


#define MODE_VALUE_16BITS 0b0010000010001100    //  < DISSAG=0 enable sag >     < CYCMODE=1 cycle energy acumulation mode >     < POAM=1 only positive active power > 0x8084
#define IRQEN_VALUE_16BITS   0b1000 //   0x131F
#define CH1OS_VALUE_8BITS 0b00000000    // 0b00010000   // OFFSET bit 7 (ultimo) precisa ser zero pois é o integrador digital 
#define CH2OS_VALUE_8BITS 0b00000000        // OFFSET
#define GAIN_VALUE_8BITS 0x00
#define PHCAL_VALUE_6BITS 0x0B      // < 00 1011 >
#define APOS_VALUE_16BITS 0x0100    // < APOS = 0x0100)  This 16-bit register allows small offsets in the active power calculation to be removed
#define WGAIN_VALUE_12BITS 0x0000
#define WDIV_VALUE_8BITS 0x00
#define IRMSOS_VALUE_12BITS 0b00000000000    //OFFSET
#define VRMSOS_VALUE_12BITS 0b01000000000   //OFFSET
#define LINECYC_VALUE_16BITS 1000  // < 2000 ciclos 8,3 de acumulacao
#define SAGLVL_VALUE_8BITS 0x42     // < 0100 0010 >  This register sets the level of the SAG detection
#define SAGCYC_VALUE_8BITS 0x65     // < 0110 0101 >  
#define VPKLVL_VALUE_8BITS 0x50     // < 0101 0000 >  This register sets the level of the voltage peak detection
#define IPKLVL_VALUE_8BITS 0x50     // < 0101 0000 >

void write8(unsigned long reg, unsigned long value);
void write16(unsigned long reg, unsigned long value);
boolean imprimir_na_serial = false;

float current[number_of_sample_types] = {  // Vetor dos valores atuais de leituras 
// V  Hz W  VA A  FP VAR
   0, 0, 0, 0, 0, 0, 0
};
 
void configADE(){
   write16(MODE, MODE_VALUE_16BITS);
   write16(IRQEN, IRQEN_VALUE_16BITS);              // 
   write16(LINECYC, LINECYC_VALUE_16BITS);
   write16(APOS, APOS_VALUE_16BITS);
   write8(CH1OS, CH1OS_VALUE_8BITS);                //ganho em milivolts 30mv utiliza só bits 0 a 5
   write8(CH2OS, CH2OS_VALUE_8BITS);                //ganho em milivolts 30mv utiliza só bits 0 a 5
   write16(IRMSOS, IRMSOS_VALUE_12BITS);            //offset canal 1 tensao
   write16(VRMSOS, VRMSOS_VALUE_12BITS);            //offset canal 2 tensao
   }


void enableChip(){
  SPI.beginTransaction (SPISettings (4000000, MSBFIRST, SPI_MODE1));
  digitalWrite(CS_PIN,LOW);
  delayMicroseconds(50);
}


void disableChip(){
  digitalWrite(CS_PIN,HIGH);  
  SPI.endTransaction();
  delayMicroseconds(50);
}


unsigned long valorMedio( int qtde_amostras, unsigned long (*function)(char), char reg ){
  unsigned long media = 0;
  for (int x=0; x<qtde_amostras; x++)
    {
      media = media + (*function)(reg);
    }  
    media = media / qtde_amostras;
    return media;
}
 
 
unsigned long read24(char reg){  //Le um registrador de 24bits
  enableChip();
  unsigned char b2,b1,b0;  //tres variaveis de 8 bits
  SPI.transfer(reg);       //inicia a comunicacao
  delayMicroseconds(5);
  b2=SPI.transfer(0x81);   //envia um dummy e recebe 8 bits por vez
  delayMicroseconds(5);
  b1=SPI.transfer(0x81);
  delayMicroseconds(5);
  b0=SPI.transfer(0x81);
  delayMicroseconds(150);
  disableChip();             
  return ((unsigned long)b2<<16 | (unsigned long)b1<<8 | (unsigned long)b0);             
}

 
unsigned long read16(char reg){
    enableChip();
    unsigned char b1,b0;
    SPI.transfer(reg);
    delayMicroseconds(5);
    b1=SPI.transfer(0x81);
    delayMicroseconds(5);
    b0=SPI.transfer(0x81);
    delayMicroseconds(150);
    disableChip();
    return ((unsigned long)b1<<8 | (unsigned long)b0);            
}


unsigned long read8(char reg){
  enableChip();
    unsigned char b1,b0;
    SPI.transfer(reg);
    delayMicroseconds(5);
    b0=SPI.transfer(0x81);
    delayMicroseconds(150);
    disableChip();
    return (unsigned long)b0;            
}


void printoutData(String reg, long regData, float calculado)
{
   Serial.print(reg);
   Serial.print("  Decimal: ");
   Serial.print(regData, DEC);
   Serial.print("  Hex: ");
   Serial.print(regData, HEX);
   Serial.print("  Calculado: ");
   Serial.println(calculado);
   Serial.println();
}


void write8(unsigned long reg, unsigned long value){ // value 8bits
   enableChip();
   SPI.transfer( (reg | 0b10000000) );  // utilizado para colocar 1 no MSB indicando gravacao no registrador
   delayMicroseconds(20);
   SPI.transfer(value&0xFF); //envia os 8 bits menos significativos
   delayMicroseconds(100);
   disableChip();  
}


void write16(unsigned long reg, unsigned long value){ // value 16bits
   enableChip();
   SPI.transfer( (reg | 0b10000000) );  //colocar 1 no MSB indica gravacao no registrador
   delayMicroseconds(20);
   SPI.transfer(value>>8); //envia os 8 bits mais significativos
   delayMicroseconds(20);
   SPI.transfer(value&0xFF); //envia os 8 bits menos significativos
   delayMicroseconds(100);
   disableChip();  
}


float retV() {
    const String reg = "VRMS";
    unsigned long measure;
    float calculado;
    measure = (valorMedio(10, read24, VRMS));
    calculado = (measure*0.500)/1561400;
    calculado = calculado*424;
    if (imprimir_na_serial){
        printoutData(reg, measure, calculado);
    }
    return calculado;
}


float retHz() {  //le periodo e retorna frequencia
    const String reg = "PERIOD";
    unsigned long measure;
    float calculado;
    measure = (valorMedio(10, read16, PERIOD));
    calculado = CLOCKIN/(8.00*measure);
    if (imprimir_na_serial){
        printoutData(reg, measure, calculado);
    }
    return calculado;
}


float retI() {  //le corrente
    const String reg = "IRMS";
    unsigned long measure;
    float calculado;
    measure = (valorMedio(10, read24, IRMS));
    calculado = (measure*10.00)/1868467;   //10 amperes correspondem ao max de 0.5V no circuito
    calculado -= calculado*0.321; 
    if (imprimir_na_serial){
        printoutData(reg, measure, calculado);
    }
    return calculado;
}


unsigned long retW(){                         //le potencia ativa
    const String reg = "POT.AT";
    float calculado;
    calculado = current[3]*current[5];  //  pot aparente * cos(teta)
    if (imprimir_na_serial){
        printoutData(reg, 0, calculado);
    }
    return calculado;
}


float retVA(){
   float calculado = current[0]*current[4];  //tensao * corrente
   return calculado; 
}


float retVAr(){
   float calculado = current[3]* sin(acos(current[5]) );   //pot aparente * sen(teta)
   return calculado; 
}


float retFP(){
    const String reg = "FP";
    float calculado = ( read24(AENERGY) / read24(VAENERGY) );
    if (imprimir_na_serial){
        printoutData(reg, 0, calculado);
    }
    imprimir_na_serial = false;
    return(calculado);
}


boolean retStatusRegister(unsigned long* status_register){
    if (read16(STATUS) & B1000000){
        *status_register = *status_register & 0xFFDF;       //desabilita a flag de RESET no status_register em caso de reboot do chip durante operacao
    }
    *status_register = *status_register | read16(RSTSTATUS);
}


boolean statusWSMP(unsigned long* status_register) {  // new data on waveform
    if (*status_register & B1000) {
        *status_register = *status_register & 0xFFF7;        //desabilita a flag apos leitura
        return true;
    }
    else
        return false;
}


boolean statusRESET(unsigned long* status_register) {  // ready
    if (*status_register & B1000000) {
        *status_register = *status_register & 0xFFBF;        //desabilita a flag apos leitura
        return true;
    }
    else
        return false;
}
