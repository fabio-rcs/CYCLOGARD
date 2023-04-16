#include <BTAddress.h>
#include <BTAdvertisedDevice.h>
#include <BTScan.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

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
  Serial.begin(9600); // Start serial monitor at 9600
  SerialBT.begin("CYCLOGUARD"); // Name of this bluetooth device
  Serial.println("My first ESP32 program"); // Debug
  Serial.println("The device started, now you can pair it with bluetooth");
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void loop() 
{
  delay(50);                    // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.

  distance_check();
  bluetooth_com();
  
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

void bluetooth_com()
{
  if (Serial.available()) {
    SerialBT.println(distance);
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
}  