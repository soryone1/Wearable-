/* set the BLE*/
#include <ArduinoBLE.h>

BLEService ReceiverService("5ce555b5-5730-42d5-85c8-fc1c6bccb968"); // BLE LED Service

BLEByteCharacteristic Red_light_Characteristic("3fecff01-25da-44bb-9fc1-d1734cec8d1c", BLERead | BLEWrite);
BLEByteCharacteristic Blue_light_Characteristic("3fecff02-25da-44bb-9fc1-d1734cec8d1c", BLERead | BLEWrite);
BLEByteCharacteristic White_light_Characteristic("3fecff03-25da-44bb-9fc1-d1734cec8d1c", BLERead | BLEWrite);

/* set the DMX*/
#include <ArduinoRS485.h>
#include <ArduinoDMX.h>

/* set the properties of the DMX*/
const int universeSize = 10;
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
      if (Red_light_Characteristic.written() ||  Blue_light_Characteristic.written() ||  White_light_Characteristic.written()) {
        if (Red_light_Characteristic.value() || Blue_light_Characteristic.value() || White_light_Characteristic.value()) {
          red = Red_light_Characteristic.value();
          blue = Blue_light_Characteristic.value();
          white = White_light_Characteristic.value();

          //          if (red != pre_red  ) {
          //                    Serial.println(red);
          //          Serial.println(blue);
          DMX.beginTransmission();

          //run the red light base on left hand
          if (red > 80) {
            DMX.write(1, red);
          } else {
            DMX.write(1, 0);
          }

          //run the blue light base on right hand
          if (blue > 80 ) {
            DMX.write(6, blue);
          } else {
            DMX.write(6, 0);
          }

          //run the white light base on the turn
          
          if (white > 150) {
            DMX.write(7, 255);
            DMX.write(8, 255);
            DMX.write(9, 255);
          } else {
            DMX.write(7, 0);
            DMX.write(8, 0);
            DMX.write(9, 0);
          }

          DMX.endTransmission();
          //          delay(1);
          //          }
          //          pre_red = red;
        }
      }
    }
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}
