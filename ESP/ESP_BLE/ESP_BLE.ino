// Bluetooth Low Energy libraries and variables

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

bool deviceConnected = false;
const int readPin = 19;

BLECharacteristic *pCharacteristic_1;
BLECharacteristic *pCharacteristic_2;
BLECharacteristic *pCharacteristic_3;
BLECharacteristic *pCharacteristic_4;

// UART service UUID
#define SERVICE_UUID              "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

// UART characteristics UUIDs
#define CHARACTERISTIC_UUID_TX        "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX        "6E400005-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX_2      "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX_3      "6E400004-B5A3-F393-E0A9-E50E24DCCA9E"

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

// Check if device is connected via bluetooth
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic_4->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

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
        Serial.println("*********");
      }
    }
};


void setup() 
{
  Serial.begin(115200); // Start serial monitor at 115200
 
  Serial.println("The ESP is ready to pair with a bluetooth device");
  
  pinMode(LED, OUTPUT);
  pinMode(HORN, OUTPUT);
  
  // Create the BLE Device and name it
  BLEDevice::init("CYCLOGARD");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic_1 = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic_1->addDescriptor(new BLE2902());

  // Create a BLE Characteristic
  pCharacteristic_2 = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX_2,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic_2->addDescriptor(new BLE2902());

  // Create a BLE Characteristic
  pCharacteristic_3 = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX_3,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic_3->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic_4 = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic_4->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}


void loop() 
{
  // Wait 50ms between pings (about 20 pings/sec). 
  // 29ms should be the shortest delay between pings.
  delay(50);                   

  distance_check();
  
  if (deviceConnected) {
    // Let's convert the value to a char array:
    
    // make sure this is big enough to store all the values
    char distance_str[8]; 
    char distance_str_2[8];

    // float_val, min_width, digits_after_decimal, char_buffer
    //dtostrf(distance_1, 1, 2, distance_str);
    //dtostrf(distance_2, 1, 2, distance_str_2);

    //  pCharacteristic->setValue(&distance_1, 1); // To send the integer value
    //  pCharacteristic->setValue("Hello!"); // Sending a test message

    // Set the charachteristic value as the distance_str
    pCharacteristic_1->setValue(distance_1);
    pCharacteristic_2->setValue(distance_2);

    if (STRANGER_DANGER) {
      pCharacteristic_3->setValue("1");
    }

    else {
      pCharacteristic_3->setValue("0");
    }

    pCharacteristic_1->notify(); // Send the value to the app!
    pCharacteristic_2->notify();
    pCharacteristic_3->notify();

    Serial.println("*** Sent Values: ");
    Serial.println(distance_str && " " && distance_str_2 && " " && STRANGER_DANGER);
    Serial.println(" ***");
  }
  delay(300);
};  


// If distance < threshold, activate buzzer and LED
void distance_check()
{
  distance_get(distance_1, distance_2);

  if (deviceConnected)

  {
    if (distance_1 > 40 && distance_1 < 150 || distance_2 > 40 && distance_2 < 150)
    {
      STRANGER_DANGER = true;
      // If LED is active, turn it on
      if (LED_BOOL) {
        digitalWrite(LED, HIGH);
        Serial.println("SD LED ON");
      }

      // If HORN is active, turn it on
      if (HORN_BOOL){
        digitalWrite(HORN, HIGH); 
        Serial.println("SD HORN ON");
      }         
    }

    else
    {
      STRANGER_DANGER = false;
      digitalWrite(HORN, LOW);
      digitalWrite(LED, LOW);
      Serial.println(" SD, LED and HORN OFF");
    }
  } ;    
}

// Get distance with the new.ping library in centimeters
int distance_get(int distance_1, int distance_2)
{
  // Send ping, get distance in cm and print result (0 = outside set distance range)
  int distance_1_cm = sonar_1.ping_cm();
  int distance_2_cm = sonar_2.ping_cm();

  Serial.println(distance_1_cm);
  Serial.println(distance_2_cm);

  distance_1 = distance_1_cm;
  distance_2 = distance_2_cm;

  return distance_1_cm;
}

