#include <WiFi.h>

WiFiClient client;

WiFiServer server(80);
const char* ssid = "WhiteCat";

const char* password = "lima1710";

String  data =""; 
int leftMotorForward = 2;  /* GPIO2(D4) -> IN3   */

int rightMotorForward = 15;   /* GPIO15(D8) -> IN1  */

int leftMotorBackward = 0;    /* GPIO0(D3) -> IN4   */

int rightMotorBackward = 13;  /* GPIO13(D7) -> IN2  */

void setup(){
  pinMode(leftMotorForward, OUTPUT);

  pinMode(rightMotorForward, OUTPUT);

   pinMode(leftMotorBackward, OUTPUT);

    pinMode(rightMotorBackward, OUTPUT);

  server.begin();

}
void loop()

{

  client = server.available();

  if (!client) return;

  data = checkClient ();

  if (data == "26/on"){
    digitalWrite(2, HIGH);
  }

} 


String checkClient (void)

{

while(!client.available()) delay(1); 

String request = client.readStringUntil('\r');

request.remove(0, 5);

request.remove(request.length()-9,9);

return request;

}
