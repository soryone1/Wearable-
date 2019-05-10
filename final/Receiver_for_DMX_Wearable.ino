/* set the BLE*/
#include <ArduinoBLE.h>

BLEService ReceiverService("5725fe87-a47a-4992-8b55-416347f9e9ca"); // BLE LED Service

BLEByteCharacteristic Red_light_Characteristic("19b10015-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);
BLEByteCharacteristic Blue_light_Characteristic("19b10016-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);
BLEByteCharacteristic White_light_Characteristic("19b10017-e8f2-537e-4f6c-d104768a1214", BLERead | BLEWrite);

/* set the DMX*/
#include <ArduinoRS485.h>
#include <ArduinoDMX.h>

/* set the properties of the DMX*/
const int universeSize = 150;
int red = 0;
int blue = 0;
int white = 0;
int pre_red;
int pre_blue;
int pre_white;

void setup() {
  Serial.begin(9600);
  //  while (!Serial);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  if (!DMX.begin(universeSize)) {
    Serial.println("Failed to initialize DMX!");
    while (1); // wait for ever
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("Receiver For DMX Lights");
  BLE.setAdvertisedService(ReceiverService);

  // add the characteristic to the service
  ReceiverService.addCharacteristic(Red_light_Characteristic);
  ReceiverService.addCharacteristic(Blue_light_Characteristic);
  ReceiverService.addCharacteristic(White_light_Characteristic);

  // add service
  BLE.addService(ReceiverService);

  // set the initial value for the characeristic:
  Red_light_Characteristic.writeValue(0);
  Blue_light_Characteristic.writeValue(0);
  White_light_Characteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  // Serial.println("DMX Peripheral");
}

void loop() {

  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    while (central.connected()) {
      if (Red_light_Characteristic.written()) {
        if (Red_light_Characteristic.value()) {
          red = Red_light_Characteristic.value();
          if (red != pre_red  ) {
            Serial.println(red);
            DMX.beginTransmission();
            DMX.write(1, red);
            DMX.endTransmission();
            delay(5);
          }
          pre_red = red;
        }
      }
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
