/* * Nome do arquivo: funcoes_OLED.h
 *  Este arquivo possui as funcoes relacionadas ao display oled
 *  
 */

SSD1306Wire  display(0x3c, SDA_PIN, SCK_PIN);


void displayInit(){                 //Inicializa o display
  display.init();
  display.flipScreenVertically();
  display.clear();
}


void display_msg(String texto){     // Envia mensagens diversas para o display
  display.clear();
  display.setColor(WHITE);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString((display.getWidth()/2), (display.getHeight()/2), texto);
  display.display();
}


void displayUpdate(float* current_params, int current_view){                                      // Atualiza dados do display  
  const String nomes_parametros[number_of_sample_types] = $sample_types;                          // Nome dos parametros [tensao, freq, etc] 
  const String unidade_medida_parametro[number_of_sample_types] = $unidades_de_medida_parametro;  // Unidades de medidas  {V Hz W VA A FP VAR}
  const int precisao_decimal[number_of_sample_types] = $precisao_decimal_parametro;               // Casas apos o ponto decimal
  display.clear();                                                                                
  display.setTextAlignment(TEXT_ALIGN_CENTER);                                                    
  display.setFont(ArialMT_Plain_16);                                                              
 
  display.drawRect(0, 0, display.getWidth(), display.getHeight());               // Retangulo principal borda branca
  display.fillRect(0, 0, display.getWidth(), (display.getHeight()/2)-8);         // Retangulo interno preenchimento branco
  display.setColor(BLACK);                                                       
  display.drawString((display.getWidth()/2),5,nomes_parametros[current_view]);   // Imprime o nome do parametro
  display.setColor(WHITE);                                                       
  
  char unidade_medida[10];
  unidade_medida_parametro[current_view].toCharArray(unidade_medida, 20);        // Casting string to char
  
  char valor_do_parametro[10];
  
  switch(precisao_decimal[current_view]){                                        // Configura a quantidade de pontos decimais na exibicao
  case 0:
    snprintf(valor_do_parametro, 10, "%.0f", current_params[current_view]);
    break;
  case 1:
    snprintf(valor_do_parametro, 10, "%.1f", current_params[current_view]);
    break;
  case 2:
    snprintf(valor_do_parametro, 10, "%.2f", current_params[current_view]);
    break;
  default:
    snprintf(valor_do_parametro, 10, "%.0f", current_params[current_view]);
  }
  
  char texto_final[20];
  snprintf(texto_final, 20, "%s %s",valor_do_parametro, unidade_medida);           // String contendo o valor formatado
  display.drawString((display.getWidth()/2),(display.getHeight()/2), texto_final); // Imprime o valor com a unidade de medida
  display.display();
}
