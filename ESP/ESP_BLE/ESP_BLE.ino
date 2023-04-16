// Bluetooth Low Energy libraries and variables

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
const int readPin = 19;
//const int LED = 33;

// UART service UUID
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Ultrasound sensor improved library
#include <NewPing.h>

// Vars and pins definition
#define TRIGGER_PIN  19
#define ECHO_PIN     23
#define MAX_DISTANCE 400 // Maximum distance we want to measure (in centimeters).
#define BUZZER       21
#define LED          33

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

 // var to store distance 
unsigned int distance = 0;

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
      std::string rxValue = pCharacteristic->getValue();

      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");

        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }

        Serial.println();

        // Do stuff based on the command received from the app
        if (rxValue.find("A") != -1) { 
          Serial.println("Turning ON!");
          digitalWrite(LED, HIGH);
        }
        else if (rxValue.find("B") != -1) {
          Serial.println("Turning OFF!");
          digitalWrite(LED, LOW);
        }

        Serial.println();
        Serial.println("*********");
      }
    }
};


void setup() 
{
  Serial.begin(115200); // Start serial monitor at 9600
 
  Serial.println("The ESP is ready to pair with a bluetooth device");
  pinMode(LED, OUTPUT);
  //pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Create the BLE Device
  BLEDevice::init("CYCLOGARD"); // Give it a name
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

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
    
    // float_val, min_width, digits_after_decimal, char_buffer
    dtostrf(distance, 1, 2, distance_str);
    
//  pCharacteristic->setValue(&distance, 1); // To send the integer value
//  pCharacteristic->setValue("Hello!"); // Sending a test message
    pCharacteristic->setValue(distance_str);
    
    pCharacteristic->notify(); // Send the value to the app!
    Serial.print("*** Sent Value: ");
    Serial.print(distance_str);
    Serial.println(" ***");
  }
  delay(300);
}  


// If distance < threshold, activate buzzer and LED
void distance_check()
{
  distance = distance_get();
  if (deviceConnected)
  {
    if (distance <= 50)
    {
      //  digitalWrite(LED, HIGH); // LED on  
      digitalWrite(BUZZER, HIGH);    
    }
    else
    {
      // digitalWrite(LED, LOW); // LED off
      digitalWrite(BUZZER, LOW);
    }
  }      
}

// Get distance with the new.ping library in centimeters
int distance_get()
{
  // Send ping, get distance in cm and print result (0 = outside set distance range)
  int distancecm = sonar.ping_cm();

  return distancecm;
}

