// Bluetooth Low Energy libraries and variables

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;

bool deviceConnected = false;

// UART service UUID
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

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
bool first_time = false;

String msg_dist;

// NewPing setup of pins and maximum distance.
NewPing sonar_1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE); 
NewPing sonar_2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);

// Global vars to store distance values from both HC-SR04 sensors
unsigned int distance_1 = 0;
unsigned int distance_2 = 0;


//-----------------------------------------------
//--------- BLE device connected check ----------
//-----------------------------------------------

// Check if device is connected via bluetooth
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
      //Serial.println("ESP is connected via bluetooth");
      //delay(1000);
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
      bool LED_BOOL = true;
      bool HORN_BOOL = false;
      bool STRANGER_DANGER = false;
      bool first_time = false;

    }
};


//-----------------------------------------------
//-------- BLE received msg callbacks -----------
//-----------------------------------------------

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("********* ");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

        Serial.print(" *********");
        Serial.println();

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

        Serial.println();
      }
      delay(1000);
    }
};


//-----------------------------------------------
//------ Setup function - only runs once --------
//-----------------------------------------------

void setup() 
{
  //Serial.begin(115200); // Start serial monitor at 115200
 
  //Serial.println("The ESP is ready to pair with a bluetooth device");

  // Define pins mode
  pinMode(LED, OUTPUT);
  pinMode(HORN, OUTPUT);
  
  // Create the BLE Device
  BLEDevice::init("CYCLOGARD"); // Give it a name

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic to send values from ESP to device
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  // Characteristic that allows connected device to send information to the ESP
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();

  // Debug
  //Serial.println("Waiting a client connection to notify...");
}


//-----------------------------------------------
//----------- Main function - loop --------------
//-----------------------------------------------

void loop() 
{

  // Wait 50ms between pings (about 20 pings/sec). 
  // 29ms should be the shortest delay between pings.
  delay(50);                 

  // Debug
  //Serial.println("Bool device is connected: " && deviceConnected);
  
  // Let's convert the string message to a character array:
  if (deviceConnected) {
    
    // Do actions according to the distance measured with the sensors
    distance_check();

    // Make sure the array is big enough to store all the values
    char msg_send[8];

    // Assign each msg character to a space in the array
    msg_dist.toCharArray(msg_send, 8);

    // Assign value to the correct bluetooth characteristic
    pCharacteristic->setValue(msg_send);

    // Send the value via BLE
    pCharacteristic->notify();
    
    // Debug
    //Serial.println("*** Sent Value: ");
    //Serial.print(msg_send);
    //Serial.print(" ***");
    //Serial.println();
  }

  else if (first_time) {
    turn_led_off();
  }
  //delay(1000);
}  


//-----------------------------------------------
//------- Distance checking and actions ---------
//-----------------------------------------------

// If distance < threshold, activate buzzer and LED
void distance_check()
{
  // First, get distance from ultrassound sensors
  distance_get();

  // Debug
  //Serial.println();
  //Serial.println("D1: ");
  //Serial.print(distance_1);
  //Serial.println();
  //Serial.println("D2: ");
  //Serial.print(distance_2);

  // If distance reading is between threshold, do actions
  if ((distance_1 > 40 && distance_1 < 150) || (distance_2 > 40 && distance_2 < 150))
  {
    // Boolean variable to indicate we're in danger zone
    STRANGER_DANGER = true;

    // If LED boolean is true and it's not already on, turn it on
    if (LED_BOOL && !first_time) {
      turn_led_on();
    }

    // If HORN boolean is true, turn it on
    else if (HORN_BOOL){
      turn_horn_on();
    }  

    // If LED boolean is false and LED is already on, turn it off
    else if (!LED_BOOL && first_time){
      turn_led_off();
    }       
  }

  // Else, turn stuff off - don't do any actions
  else
  {
    // Danger boolean is off
    STRANGER_DANGER = false;

    if (first_time)
    {
      turn_led_off();
    }

    // Debug
    //Serial.println(" Not in Danger");
  }
}


//-----------------------------------------------
//-------------- Distance get -------------------
//-----------------------------------------------

// Get distance with the new.ping library in centimeters and output a concatenated string
String distance_get()
{
  // Send ping, get distance in cm and print result (0 = outside set distance range)
  int distance_1_cm = sonar_1.ping_cm();
  int distance_2_cm = sonar_2.ping_cm();

  // Associate values with the global variables
  distance_1 = distance_1_cm;
  distance_2 = distance_2_cm;

  // Transform values into a string
  String msg = String(distance_1_cm) + ";" + String(distance_2_cm) + ";" + STRANGER_DANGER;
  msg_dist = msg;

  // Degub
  //Serial.println();
  //Serial.println("D1: ");
  //Serial.print(distance_1_cm);
  //Serial.print(" from distance_get");
  //Serial.println();
  //Serial.println("D2: ");
  //Serial.print(distance_2_cm);
  //Serial.print(" from distance_get");

  return msg;
};


//-----------------------------------------------
//------------ Action functions -----------------
//-----------------------------------------------

// Turning LED off
void turn_led_off() 
{
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);

  first_time = false;
};


// Turning LED on
void turn_led_on() 
{
  delay(50);
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);

  //Serial.println("SD LED ON");

  first_time = true;
};


// Turning horn on
void turn_horn_on() 
{
  delay(50);
  digitalWrite(HORN, HIGH); 
  delay(50);
  digitalWrite(HORN, LOW);
  delay(1000);

  //Serial.println("SD HORN ON");
};