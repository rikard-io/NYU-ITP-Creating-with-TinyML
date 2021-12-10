#include <ArduinoBLE.h>

// BLE Counter Service
BLEService counterService("00001234-0000-1000-8000-000000000001");

// BLE Counter Characteristic
BLEUnsignedCharCharacteristic counterChar("00001234-0000-1000-8000-000000000002",
                                          BLERead | BLENotify);

// Our counter
unsigned char counter = 0;
long previousMillis = 0;

void setup() {
  Serial.begin(9600); // initialize serial communication

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1)
      ;
  }

  BLE.setLocalName("WebBLE Test");
  BLE.setAdvertisedService(counterService); // add the service UUID
  counterService.addCharacteristic(
      counterChar);                // add the counter characteristic
  BLE.addService(counterService);  // Add the counter service
  counterChar.writeValue(counter); // set initial value for this characteristic

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

bool isConnected = false;
void loop() {
  // wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    if(!isConnected){
      isConnected = true;
      Serial.print("Connected to central: ");
      // print the central's BT address:
      Serial.println(central.address());
      // turn on the LED to indicate the connection:
      digitalWrite(LED_BUILTIN, HIGH);
    }
      
    if (central.connected()) {
      long currentMillis = millis();
      // if 300ms have passed, increase the counter value
      if (currentMillis - previousMillis >= 300) {
        previousMillis = currentMillis;
        
        counter++;
        Serial.print("Counter % is now: ");
        Serial.println(counter);
        counterChar.writeValue(counter);
        
      }
    } else {
      if(isConnected){
        isConnected = false;
        // Led on when connected
        digitalWrite(LED_BUILTIN, LOW);
        Serial.print("Disconnected from central: ");
        Serial.println(central.address());
      }
    }
  }
}
