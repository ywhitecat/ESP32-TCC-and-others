//define o tamanho do vetor de transmissão, no exemplo 3 canais

byte PacoteRX[10]; // vetor_pacote de recepção de 5 variáveis, pois cada variável possui 10 bits (0 a 1023)
byte PacoteTX[10]; // vetor_pacote de transmissão de 5 variáveis, pois cada variável possui 10 bits (0 a 1023)
int valorAD0;
int pwmLed;
int pinoLed = 8;

void setup() {
  Serial.begin(9600);     //  inicia a serial principal com 9600 bps (essa serial recebe a solicitação do mestre)
  pinMode(10, OUTPUT);     //  define a porta 4 como saída
  digitalWrite(10, HIGH);  //  leva a porta 4 para nível lógico 1 quando liga
}

void loop() { // Este é o loop infinito do Arduino. Enquando estiver ligado vai executar as instruções que estão neste loop.
  // Verifica se existem 10 bytes no buffer da serial
  //if (Serial.available() == 10)   // verifica se recebeu pacote de 10 bytes da solicitação do mestre
if (Serial.available()>=0 )
  {
    digitalWrite(10, HIGH);
    // Este "for" faz a leitura do buffer da serial e zera pacote de transmissão
    for (int i = 0; i < 10; i++) // PacoteTX[#] é preenchido com zero e PacoteRX[#] recebe os bytes do buffer
    {
      PacoteRX[i] = Serial.read();  // Faz a leitura dos bytes que estão no buffer da serial;
      if (PacoteRX[0] => 0 ){
        int valor = Serial.read();
        pwmLed = analogWrite(pinoLed, valor );
      }
      PacoteTX[i] = 0; // Zera o pacote de transmissão
      delay(1); // Delay de 1ms para voltar a ler a serial
    }

    valorAD0 = analogRead(A0);// leitura do canal 0
//    pwmLed = analogWrite();

    // Escreve no pacote as informações de temperatura
    PacoteTX[0] = (byte) (valorAD0 / 256); // Valor inteiro no byte 0, conversão de 16 bits em 2 bytes
    PacoteTX[1] = (byte) (valorAD0 % 256); // resto da divisão no byte 1, conversão de 16 bits em 2 bytes
    PacoteTX[2] = (byte) (0); // Valor inteiro no byte 2, conversão de 16 bits em 2 bytes
    PacoteTX[3] = (byte) (0); // Resto da divisão no byte 3, conversão de 16 bits em 2 bytes
    PacoteTX[4] = (byte) (0); // Valor inteiro no byte 4, conversão de 16 bits em 2 bytes
    PacoteTX[5] = (byte) (0); // Resto da divisão no byte 5, conversão de 16 bits em 2 bytes
    PacoteTX[6] = (byte) (0); // Valor teste
    PacoteTX[7] = (byte) (0); // Valor teste
    PacoteTX[8] = (byte) (0); // Valor teste
    PacoteTX[9] = (byte) (0); // Valor teste

    // Transmissão do pacote para o mestre
    for (int i = 0; i < 10; i++)
    {
      Serial.write(PacoteTX[i]);
    }
    digitalWrite(10, LOW); // apaga o led
  }
}
