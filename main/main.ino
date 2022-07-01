
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
 
#define built_inLed 2
 
 
float h ;
float t;
 

 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "esp32-module"                         //change this

// ARN  arn:aws:iot:us-east-1:537921206588:thing/esp32-module
 
const char WIFI_SSID[] = "TP-Link_esp32";               //change this
const char WIFI_PASSWORD[] = "esp32-pass";           //change this
const char AWS_IOT_ENDPOINT[] = "a3bk78mek0njqi-ats.iot.us-east-1.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
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
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUBPdEetOTjqgUytAvd3joq2NIvjUwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIyMDMyNjIxMjkz
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOXKL3w9Y2+zxeWEKIIH
18hU0yHpI/sMAG++LWLhf62wR/MHsf+SEtOpiWUTTxqjIoTQf5PQhRtWXQm1MwZo
rYCptomPvcxud+7X+uKAkPljT4cCyzx54u/j17lOXlV6yC55WciT+s0tDPXo0qZr
oc0W/2HaJ/hdR4TabDBE0nMDqAdP3sN8cMzSUGBoza+jSh3TQKQ2pYx9LgrnUHc/
SptdWeH+aIKf0xFMUr6qrNnFpJqb2RubrR+JqtQgVkgDByCLsWQiKio39q0DMsW2
d2d5NqgzgdDWTTVkr6o0p2XNbXiQBjrakqwbMhPccRDD0jNoqnMA2u15GajhRiqi
4BECAwEAAaNgMF4wHwYDVR0jBBgwFoAUYhslRfVENhnjlhZvrAgiWvp6sfkwHQYD
VR0OBBYEFLvNGEcUkrvrXU5xstdO5SQGgIFlMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBk9afBNiid84AeqtWP+p78p7kb
rFVyKIigDIkfMOtFl5+L0mbaTsxo9HX348/SA30szmrfqwiGQA61kmMOK/eIHphb
NldpxtGQEdxUFsCXQlstL1bwOKSVS41gmVwOxX+C9/NXadnzg55WDixjHCYkrV4L
yOdCg5Xtq1KTDzXGJ/VFgsrFnHP833sHsa1nb8Blzig5eTLVzLMJvNwpY8orwHC8
G/44I7/oHHRNu/LoGv4ekpCZLR9hQaP09Ng0aTCGmbpQ8rV45MR1/V+2CD+TzRbc
vx/yyCjYuSsPAQDcTL/88E6dndSeykLHnj0RNSKTxq83nYhwLdyOvFSukECz
-----END CERTIFICATE-----

 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpQIBAAKCAQEA5covfD1jb7PF5YQoggfXyFTTIekj+wwAb74tYuF/rbBH8wex
/5IS06mJZRNPGqMihNB/k9CFG1ZdCbUzBmitgKm2iY+9zG537tf64oCQ+WNPhwLL
PHni7+PXuU5eVXrILnlZyJP6zS0M9ejSpmuhzRb/Ydon+F1HhNpsMETScwOoB0/e
w3xwzNJQYGjNr6NKHdNApDaljH0uCudQdz9Km11Z4f5ogp/TEUxSvqqs2cWkmpvZ
G5utH4mq1CBWSAMHIIuxZCIqKjf2rQMyxbZ3Z3k2qDOB0NZNNWSvqjSnZc1teJAG
OtqSrBsyE9xxEMPSM2iqcwDa7XkZqOFGKqLgEQIDAQABAoIBAQCxqmyig5hRMk7t
EQx81PGKq8L+chLmtLXVr9XJoQ4lzakjh6yhLGCwFGHtq0s7DizPQaGSOOPhahnP
OrEz4FVdqFpA68lUuYizO3x4Ce2NVLKVcZyPqrHCxofxd11Ke/8wp5S6l2ePoahm
rVfJYmiceI6GKrVCu2RjnLmPbHbGgvMhH2xedIc3qLgGvlWoE9nVofHEDrogmusV
5b5NHwgrlgK0vL1t8DhtJjpE2sCQOBEXn/351KbNgCljfU58eoa/9xAAfdkb/CTw
knacypCurSYoiWRLje5Aw6KBmIbjWteKi5vOfqz43uulMsbBANo36gKLndgihncA
26uvhZONAoGBAPUeKUJgn4jcFo9jQPlm/IAQEnvD14SMYFEH/N7CZzfAVl1b3cAX
FCJv4k9VBVBR5tBrrZW/7mvRXBJmcuGuRJ+ffV8T1erHRpramvvc+V29gC3YOy4/
2ZDd6U53pKVrklWuI/flNh3PDVMh5DYP+2bRFTICgjvOoBWNxI+VvYmzAoGBAO/9
0QtackvQ/Fe/zg3/f4dxbdCiBmQh5DrPu5quMZOy3c88v0ahoE0UgpCdvn54uILq
SoX6MQDO6IhMa6P9xGPYBoQcQs1QUGaqVLzBeiVnb6kw5MkONwi1DzIQevAHEJWR
fPQtxJyD9EI4ZnWpA1UBB5bTQ12WKE12U4L80sUrAoGBANDYjPD4YJV8aGBtgeu9
+r+mcgkvuQ4/iMUh2Ct4WfefkjZjUpIbV9VrnNhZKXjg/5dvV2wzJxKIRDmNPq7Z
C7Exc+Gxhd9PQPIp9klIYtOi5XaqOOpUDqym1uU7fXZQLFOCVzUc/7nwM3s0cjw5
68sTtEpy5sHMcItMykBIpkCPAoGBAO6ARgGMb04dcMob2cFbivelqEbQ3p0geVph
G8YLG0X3Wiwb6NNeyJoRqffibG/vtR6VjF49V/m/pqPdnpOpHEVFaKReGZ1FFcIR
wIidlrAZbYStDJEqyVKj7WgJrMNhof+iSJW57kzFbjdpKZoAz0jo7wvbBHW7Rm5F
C5V/gE9lAoGAfjFOT3ocQscIMc2n0q26TXfgoqThTEP0MOB7HtCfIy49Auc3o9e1
qSCNtdrdTcnOwHeLG29kEojbyrphv9Tb466YFpq1SpD4xmts1jhtB/gR3nRyRxKe
IPE3HILkyI4Cs9rlVPMSj7J6T/EFycuMihW1MtOgCaNWQ486nb/ENg4=
-----END RSA PRIVATE KEY-----

 
 
)KEY"; 
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(callback);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe("esp32/all");
 
  Serial.println("AWS IoT Connected!");
}
 
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish("esp32/all", jsonBuffer);
//
//#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
//#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"
//  
}
 
void callback(char* topic, byte* payload, unsigned int length) {
 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------");
 
}
 
void setup()
{
  Serial.begin(115200);
  connectAWS();

}
 
void loop()
{
publishMessage();

  }
  Serial.print(F("Humidity: "));

  Serial.print(F("%  Temperature: "));

  Serial.println(F("°C "));
 
  client.publish("esp32/all", "Hello from ESP32");
  client.publish("esp32/pub", "Hello from ESP32");
  client.loop();
  delay(1000);
  
}


//https://techtutorialsx.com/2017/04/24/esp32-publishing-messages-to-mqtt-topic/  how to puclish for esp32
// https://techtutorialsx.com/2017/04/24/esp32-subscribing-to-mqtt-topic/    the call-back function
