#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

//#include <Arduino.h>
#include <WiFi.h>


WiFiClient client;
Servo myservo;


const char* ssid = "WhiteCat";
const char* password = "lima1710";

// Assign output variables to GPIO pins
int servoPin = 13;
const int moveForward   = 12;
const int moveLeft      = 14;
const int moveRight     = 27;
const int moveBackward  = 26;

WiFiServer server(80);

// Variable to store the HTTP request
String header;

// other variables



// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void conecta() {

  Serial.print("Conectando a rede local SSID: ");
  Serial.println(ssid);


  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
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
  pinMode(moveForward, OUTPUT);
  pinMode(moveLeft, OUTPUT);
  pinMode(moveRight, OUTPUT);
  pinMode(moveBackward, OUTPUT);
  myservo.setPeriodHertz(50);
  myservo.attach(servoPin);
  myservo.write(90);
  Serial.begin(115200);
  conecta();
  server.begin();

}

void httpSerial () {
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
            if (header.indexOf("GET /esp32/up/on") >= 0) {
              Serial.println("Indo para Frente");
              digitalWrite(moveForward, HIGH);
            } else if (header.indexOf("GET /esp32/up/off") >= 0) {
              digitalWrite(moveForward, LOW);
            }
            if (header.indexOf("GET /esp32/right/on") >= 0) {
              Serial.println("Indo para direita");
              myservo.write(40);
            } else if (header.indexOf("GET /esp32/right/off") >= 0) {
              myservo.write(90);
            }
            if (header.indexOf("GET /esp32/left/on") >= 0) {
              Serial.println("Indo para esquerda");
              myservo.write(130);
            } else if (header.indexOf("GET /esp32/left/off") >= 0) {
              myservo.write(85);
            }
            if (header.indexOf("GET /esp32/reverse/on") >= 0) {
              Serial.println("Indo para tras");
              digitalWrite(moveBackward, HIGH);
            } else if (header.indexOf("GET /esp32/reverse/off") >= 0) {
              digitalWrite(moveBackward , LOW);
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
    delay (1);
  }
}


void loop() {
  httpSerial();
  String receive_msg = "";

  if (Serial.available() > 0) {
    receive_msg = Serial.read();
    if (receive_msg = "info" ) {
      Serial.println ("----------------------------------------");
      Serial.println ("TCC FATEC SBC");
      Serial.println ("INFORMACOES DO PROGRAMA");
      if (WiFi.status() == WL_CONNECTED) {

        Serial.print("Conectado ao WiFi local de SSID: ");
        Serial.println(ssid);
        Serial.print ("Endereco de IP: ");
        Serial.println(WiFi.localIP());
      }
      Serial.println ("Programacao por Andre - 2022");
      Serial.println ("----------------------------------------");
    }
  }
}
