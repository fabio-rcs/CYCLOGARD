// Wifi libraries
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

// Ultrasound sensor improved library
#include <NewPing.h>

// Vars and pins definition
#define TRIGGER_PIN  19
#define ECHO_PIN     23
#define MAX_DISTANCE 400 // Maximum distance we want to measure (in centimeters).
#define BUZZER       21
#define LED          33

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int distance = 0; // var to store distance 

void setup() 
{
  Serial.begin(9600); // Connection velocity
  
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  
  Serial.println("My first ESP32 program"); // Debug
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() 
{
  delay(50);                    // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

  //int distance = sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)

  distance_check();
  
  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println("cm");
}  

void distance_check()
{
  distance = distance_get();
  
  if (distance <= 50)
    {
      digitalWrite(LED, HIGH); // LED on  
      digitalWrite(BUZZER, HIGH);    
    }
  else
    {
      digitalWrite(LED, LOW); // LED off
      digitalWrite(BUZZER, LOW);
    }      
}


int distance_get()
{
  int distancecm = sonar.ping_cm(); // Send ping, get distance in cm and print result (0 = outside set distance range)

  return distancecm;
}

void initWiFi() 
{
  // Replace with your network credentials
  const char* ssid = "I'm under your bed";
  const char* password = "7MaravilhasNaturais?!";
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("CONNECTED: ");
    delay(1000);
  }
  
  Serial.println(WiFi.localIP());
}