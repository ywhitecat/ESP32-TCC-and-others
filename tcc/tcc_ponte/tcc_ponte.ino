#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

//#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "WhiteCat";
const char* password = "lima1710";

// Assign output variables to GPIO pins
const int Yplus  = 12;
const int Yminus = 14;
const int Xplus  = 27;
const int Xminus = 26;
const int Zplus  = 25;
const int Zminus = 23;
const int eletro = 32;

WiFiServer server(80);

// Variable to store the HTTP request
String header;

// other variables



// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 200;


void conecta(){

  Serial.print("Conectando a rede local SSID: ");
  Serial.println(ssid);

  
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");   
  }
  Serial.println(" ");
  Serial.println("WiFi Conectado.");
  Serial.println("IP : ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void setup() {
  pinMode(Yplus, OUTPUT);
  pinMode(Yminus, OUTPUT);
  pinMode(Xplus, OUTPUT);
  pinMode(Xminus, OUTPUT);
  pinMode(Zplus, OUTPUT);
  pinMode(Zminus, OUTPUT);
  pinMode(eletro, OUTPUT);

Serial.begin(115200);
conecta();
server.begin();
  
}

void httpSerial (){
WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
  Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
                 // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /esp32/ponte/Xneg/on") >= 0) {
              Serial.println("X- on");
              digitalWrite(Xminus, HIGH);
            } else if (header.indexOf("GET /esp32/ponte/Xneg/off") >= 0){
               Serial.println("X- off");
              digitalWrite(Xminus, LOW);
            }
            if (header.indexOf("GET /esp32/ponte/Xplus/on") >= 0) {
                digitalWrite(Xplus, HIGH);
              Serial.println("X+ on");
            } else if (header.indexOf("GET /esp32/ponte/Xplus/off") >= 0){
                digitalWrite(Xplus, LOW);
              Serial.println("X+ off");
  
            }
            if (header.indexOf("GET /esp32/ponte/Yneg/on") >= 0) {
                digitalWrite(Yminus, HIGH);
              Serial.println("Y- on");


            } else if (header.indexOf("GET /esp32/ponte/Yneg/off") >= 0){
                digitalWrite(Yminus, LOW);
              Serial.println("Y- off");

            }
            if (header.indexOf("GET /esp32/ponte/Yplus/on") >= 0) {
              Serial.println("Y+ on");
              digitalWrite(Yplus, HIGH);
            } else if (header.indexOf("GET /esp32/ponte/Yplus/off") >= 0){
              Serial.println("Y+ off");             
              digitalWrite(Yplus, LOW);
            }
            if (header.indexOf("GET /esp32/ponte/Zneg/on") >= 0) {
              Serial.println("Z- on");
              digitalWrite(Zminus, HIGH);
            } else if (header.indexOf("GET /esp32/ponte/Zneg/off") >= 0){
              Serial.println("Z- off");             
              digitalWrite(Zminus, LOW);
            }
            if (header.indexOf("GET /esp32/ponte/Zplus/on") >= 0) {
                digitalWrite(Zplus, HIGH);
                Serial.println("Z+ on");
            } else if (header.indexOf("GET /esp32/ponte/Zplus/off") >= 0){
                digitalWrite(Zplus, LOW);
              Serial.println("Z+ off");             
            }
            if (header.indexOf("GET /esp32/ponte/eletro/on") >=0){
                digitalWrite(eletro, HIGH);
                Serial.println("eletro ima ON");
            }
            else if (header.indexOf("GET /esp32/ponte/eletro/off") >= 0)
            {
                digitalWrite(eletro, LOW);
                Serial.println("eletro ima OFF");
            }
      
        
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}


void loop() {
  httpSerial();
  char receive_msg = 0;

  if (Serial.available() > 0){
    receive_msg = Serial.read();
    if (receive_msg = 'info' ){
      Serial.println ("----------------------------------------");
      Serial.println ("TCC FATEC SBC");
      Serial.println ("INFORMACOES DO PROGRAMA");
      if (WiFi.status() == WL_CONNECTED){
 
    Serial.print("Conectado ao WiFi local de SSID: ");
    Serial.println(ssid);   
    Serial.print ("Endereco de IP: ");
    Serial.println(WiFi.localIP());
  }
      else
      {
          Serial.println("WIFI NAO CONECTADO ");
          Serial.println("FAVOR VERIFICAR ESPECIFICACOES DE SSID E SENHA");
      }
      Serial.println("Grupo: Andre, Eduardo, Gustavo e Peter");
      Serial.println("professor Orientador: Marco Antonio");
      Serial.println ("Programacao por Andre - 2022");
      Serial.println ("----------------------------------------");
    }
  }
}
