// Função para ler os caracteres vindos da serial e montar a "string conteudo" 
  String leStringSerial() {
  String conteudo = "";                           // apaga a variável conteúdo
  char caractere = 0;                             // apaga a variável caractere

  while (Serial.available() > 0) {                // Enquanto tiver algo na serial
    caractere = Serial.read();                    // Lê byte da serial e coloca na variável caractere
    conteudo.concat(caractere);                   // Concatena os caracteres lidos
    delay(10);                                    // Aguarda 10 ms para ler o próximo caractere
  }
  return conteudo;                                // retorna conteúdo lido
}
