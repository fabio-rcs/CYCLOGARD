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
#define TRIGGER_PIN_1  12
#define ECHO_PIN_1     13
#define TRIGGER_PIN_2  27
#define ECHO_PIN_2     14
#define MAX_DISTANCE   400 // Maximum distance we want to measure (in centimeters).
#define HORN           26
#define SIGNAL         25
#define LED            33

bool LED_BOOL = true;
bool HORN_BOOL = false;
bool STRANGER_DANGER = false;

// NewPing setup of pins and maximum distance.
NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE); 
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);

// Global vars to store distance values from both HC-SR04 sensors
unsigned int distance_1 = 0;
unsigned int distance_2 = 0;

void setup() 
{
  Serial.begin(115200); // Start serial monitor at 9600
  SerialBT.begin("CYCLOGUARD"); // Name of this bluetooth device
  Serial.println("The ESP is ready to pair with a bluetooth device"); // Debug
  pinMode(LED, OUTPUT);
  pinMode(HORN, OUTPUT);
}

void loop() 
{
  // Wait 50ms between pings (about 20 pings/sec)
  // 29ms should be the shortest delay between pings
  delay(50);

  distance_check();
  bluetooth_com();
  
  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println("cm");
}  

void distance_check()
{
  Distance result = distance_get();
  distance_1 = result.distance_1_cm;
  distance_2 = result.distance_2_cm;
  
  Serial.println(distance_1>20);
  Serial.println(distance_1<150);
  Serial.println(distance_2>20);
  Serial.println(distance_2<150);
  
  if ((distance_1 > 20 && distance_1 < 150) || (distance_2 > 20 && distance_2 < 150))
    {
      STRANGER_DANGER = true;
      // If LED is active, turn it on
      if (LED_BOOL) {
        digitalWrite(LED, HIGH);
        Serial.println("SD LED ON");
      }

      // If HORN is active, turn it on
      else if (HORN_BOOL){
        digitalWrite(HORN, HIGH); 
        Serial.println("SD HORN ON");
      }         
    }

    else
    {
      STRANGER_DANGER = false;
      digitalWrite(HORN, LOW);
      digitalWrite(LED, LOW);
      Serial.println(" SD OFF");
    }
  }    
}

struct Distance {
    int distance_1_cm;
    int distance_2_cm;
};

Distance distance_get()
{
  Distance.result;
  // Send ping, get distance in cm and print result (0 = outside set distance range)
  result.distance_1_cm = sonar_1.ping_cm();
  result.distance_2_cm = sonar_2.ping_cm();

 // distance_1 = distance_1_cm;
 // distance_2 = distance_2_cm;

  Serial.println();
  Serial.println("D1: ");
  Serial.print(distance_1_cm);
  Serial.print(" from distance_get");
  Serial.println();
  Serial.println("D2: ");
  Serial.print(distance_2_cm);
  Serial.print(" from distance_get");

  return result;
}

void bluetooth_com()
{
  // make sure this is big enough to store all the values
  String msg = String(distance_1) + String(distance_2);
  //String distance_str = String(distance_1);
  //String distance_str_2 = String(distance_2);

  //char msg[8];
  //msg = "";
  //msg += distance_str;
  //msg += distance_str_2;

  SerialBT.println(msg);

  if (SerialBT.available()) {
    String rxValue = SerialBT.readString();
    
    // Do stuff based on the command received from the app
    if (rxValue.find("A") != -1) { 
      Serial.println("LED is available!");
      LED_BOOL = true;
    }

    else if (rxValue.find("B") != -1) {
      Serial.println("LED is not available!");
      LED_BOOL = false;
    }

    else if (rxValue.find("C") != -1) {
      Serial.println("Horn is available!");
      HORN_BOOL = true;
    }

    else if (rxValue.find("D") != -1) {
      Serial.println("Horn is not available!");
      HORN_BOOL = false;
    }
  }
}