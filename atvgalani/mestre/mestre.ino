/* Esse código é uma adaptação do código original desenvolvido por Rafael - Delpitec*/

#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 9);

byte Pacote[10];
// declaração de variáveis a serem escritas no supervisório
  int AI0 = 0;                                     // Variável para armazenar conversão do canal analógico A0
  int AI1 = 0;                                     // Variável para armazenar conversão do canal analógico A1
  char DI1 = 0;                                    // Variável que contém status da porta 2
  char DI2 = 0;                                    // Variável que contém status da porta 3
  char DI3 = 0;                                    // Variável que contém status da porta 4
  char DI4 = 0;                                    // Variável que contém status da porta 5
  char sEscritas[10];                              // Vetor de escrita com 10 posições (caracteres que serão transmitidos ao supervisório)
  
// declaração de variáveis a serem lidas do supervisório
  int DO1 = 0;                                     // Variável que contém comando para porta 8
  int DO2 = 0;                                     // Variável que contém comando para porta 9
  int DO3 = 0;                                     // Variável que contém comando para porta 12
  int DO4 = 0;                                     // Variável que contém comando para porta 13
  int PWM1c;                                       // variável para armazenar dígito de centena do valor pwm1 lido
  int PWM1d;                                       // variável para armazenar dígito de dezena do valor pwm1 lido
  int PWM1u;                                       // variável para armazenar dígito de unidade do valor pwm1 lido
  int PWM2c;                                       // variável para armazenar dígito de centena do valor pwm2 lido
  int PWM2d;                                       // variável para armazenar dígito de dezena do valor pwm2 lido
  int PWM2u;                                       // variável para armazenar dígito de unidade do valor pwm2 lido
  int Saida1PWM;                                   // variável do valor pwm1
  int Saida2PWM;                                   // variável do valor pwm2
  String recebido = "";                            // apaga o string "recebido"

void setup() { // configurações iniciais

  Serial.begin(38400);                             // método para inicializar objeto serial com 19200 bits por segundo
                                                   // velocidades usuais 9600, 19200 e 38400, 
  mySerial.begin(9600); //Serial virtual arduino
}

void loop() { // loop principal
 SerialVirtual();
// leitura das variáveis para escrita no supervisório
//    AI0 = analogRead(A0);                          // Le canal A0 e armazena em AI0
//    AI1 = analogRead(A1);                          // Le canal A1 e armazena em AI1
//    DI1 = digitalRead(2);                          // Leitura da porta 2
//    DI2 = digitalRead(3);                          // Leitura da porta 3
//    DI3 = digitalRead(4);                          // Leitura da porta 4
//    DI4 = digitalRead(5);                          // Leitura da porta 5

    if (Serial.available() > 0) {                  // Se receber algo pela serial
      recebido = leStringSerial();                 // Lê a string recebida: função que está na outra aba do IDE "LerString"

// leitura dos comandos vindos do SSC (faz a conversão desses caracteres para inteiro > '0')
      DO1 = recebido.charAt(0) - '0';              // leitura do comando (primeiro caracter) para porta 8
      DO2 = recebido.charAt(1) - '0';              // leitura do comando (segundo caracter)  para porta 9
      DO3 = recebido.charAt(2) - '0';              // leitura do comando (terceiro caracter) para porta 12
      DO4 = recebido.charAt(3) - '0';              // leitura do comando (quarto caracter)   para porta 13
      PWM1c =  recebido.charAt(4) - '0';           // leitura do digito de centena para o PWM1, (quinto caracter)
      PWM1d =  recebido.charAt(5) - '0';           // leitura do digito de dezena  para o PWM1, (sexto caracter)
      PWM1u =  recebido.charAt(6) - '0';           // leitura do digito de unidade para o PWM1, (sétimo caracter)
      PWM2c =  recebido.charAt(7) - '0';           // leitura do digito de centena para o PWM2, (oitavo caracter)
      PWM2d =  recebido.charAt(8) - '0';           // leitura do digito de dezena  para o PWM2, (nono caracter)
      PWM2u =  recebido.charAt(9) - '0';           // leitura do digito de unidade para o PWM2, (décimo caracter)

AI0 = variavelAD0;
int pwmEnvia = PWM1c + PWM1d + PWM1u;

// montagem e escrita do vetor para o supervisório
      sprintf (sEscritas, "%04u%04u%d%d%d%d\r", AI0, AI1, DI1, DI2, DI3, DI4); // Organiza os 12 caracteres no vetor string sEscritas
      mySerial.write(pwmEnvia);
      Serial.write(sEscritas);                     // Escrita na linha TX em formato binário
    }
  }
  
  void SerialVirtual(){
      unsigned long currentMillis = millis();// método para não utilizar delay. A função millis retorna o tempo desde que ele foi energizado
  if (currentMillis - previousMillis >= interval) { // verifica se já passou o intervalo configurado de 2 segundos.
    previousMillis = currentMillis;                                                 // estamos utilizando threads do arduino
    for (int i = 0; i < 10; i++)// laço "for" para escrever 10 bytes no escravo
    {
      Pacote[i] = 0; // apaga as posições do vetor
      mySerial.write(Pacote[i]);// transmite um pacote de 10 bytes para o escravo
    }
    delay(100);// aguarda 100ms para dar tempo ao escravo

    // se prepara para receber a resposta do escravo

    if (mySerial.available() == 10)  { //verifica se chegou um pacote de 10 bytes do escravo
      for (int i = 0; i < 10; i++)
      {
        Pacote[i] = mySerial.read();  // Faz a leitura dos bytes que vem do escravo
        delay(1); // Delay de 1ms para voltar a ler a serial
      }
      // "printar" na serial do mestre (apenas para monitorar):
           mySerial.write(pwmEnvia);
     int variavelAD0 = (Pacote[0] * 256 + Pacote[1]);
     mySerial.print ("Variavel analoca do escravo: ");
     mySerial.println (variavel1);
//      variavel2 = (Pacote[2] * 256 + Pacote[3]);
//      Serial.print ("Variavel 2 do escravo");
//      Serial.println (variavel2);
//      variavel3 = (Pacote[4] * 256 + Pacote[5]);
//      Serial.print ("Variavel 3 do escravo");
//      Serial.println (variavel3);
//      variavel4 = (Pacote[6] + Pacote[7]);
//      Serial.print ("Variavel 4 do escravo");
//      Serial.println (variavel4);
//      variavel5 = (Pacote[8] + Pacote[9]);
//      Serial.print ("Variavel 5 do escravo");
//      Serial.println (variavel5);

      
      }
    }
  }
