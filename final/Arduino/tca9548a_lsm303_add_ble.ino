/*  set up for BlueTooth */

#include <ArduinoBLE.h>
BLEService sensorService("9262a3a9-bb7f-479d-bc28-298ee8dc593b"); // create service for MKR_Sensor
BLEIntCharacteristic Left_hand_3_Characteristic("b314d5c1-f2e2-4ee8-9820-d507474ecd30", BLERead | BLENotify);
BLEIntCharacteristic Right_hand_7_Characteristic("b314d5c2-f2e2-4ee8-9820-d507474ecd30", BLERead | BLENotify);
BLEIntCharacteristic Back_1_Characteristic("b314d5c3-f2e2-4ee8-9820-d507474ecd30", BLERead | BLENotify);

/*  set up for TCAADDR */
#include <Wire.h>

#define TCAADDR 0x70

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(333);

Adafruit_LSM303_Accel_Unified accel2 = Adafruit_LSM303_Accel_Unified(777);

Adafruit_LSM303_Accel_Unified accel3 = Adafruit_LSM303_Accel_Unified(111);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}


void setup()
{
  //  while (!Serial);  // open the Serial monitor if necessary
  delay(1000);

  //begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set the local name peripheral advertises
  BLE.setLocalName("Sensor For Wearable");

  // set the UUID for the service this peripheral advertises:
  BLE.setAdvertisedService(sensorService);

  // add the characteristics to the service
  sensorService.addCharacteristic(Left_hand_3_Characteristic);
  sensorService.addCharacteristic(Right_hand_7_Characteristic);
  sensorService.addCharacteristic(Back_1_Characteristic);

  // add the service
  BLE.addService(sensorService);

  //set init value
  Left_hand_3_Characteristic.writeValue(0);
  Right_hand_7_Characteristic.writeValue(0);
  Back_1_Characteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");

  /* init the TCAADDR */
  Wire.begin();

  //left hand connects to No.2
  tcaselect(3);
  if (!accel.begin())
  {
    Serial.println("Ooops, no LSM303 on No.3 detected ... Check your wiring!");
    while (1);
  }

  //Right hand connects to No.4
  tcaselect(7);
  if (!accel2.begin())
  {
    Serial.println("Ooops, no LSM303 on No.7 detected ... Check your wiring!");
    while (1);
  }

  tcaselect(1);
  if (!accel3.begin())
  {
    Serial.println("Ooops, no LSM303 on No.1 detected ... Check your wiring!");
    while (1);
  }

  /* Display some basic information on this sensor */
  //  displaySensorDetails();

}

void loop()
{
  /* poll for BLE events*/
  BLE.poll();

  /*start getting the value from the sensor*/
  tcaselect(3);
  sensors_event_t event_1;
  accel.getEvent(&event_1);

  int left_hand_value = constrain(event_1.acceleration.y, -10 , 10);
  left_hand_value = map (left_hand_value, -10 , 10 , 0, 255);
  Serial.print("Left_hand_3:");
  /* Display the results (acceleration is measured in m/s^2) */
  //  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(left_hand_value); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.print("m/s^2 ");



  tcaselect(7);
  sensors_event_t event_2;
  accel2.getEvent(&event_2);

  int right_hand_value = constrain(event_2.acceleration.y, -9 , 9);
  right_hand_value = map (right_hand_value, -9 , 9 , 0, 255);
  Serial.print("Right_hand_7:");
  /* Display the results (acceleration is measured in m/s^2) */
  //  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(right_hand_value); Serial.print("  ");
  //  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.print("m/s^2 ");

  tcaselect(1);
  sensors_event_t event_3;
  accel3.getEvent(&event_3);

  int back_value = abs(event_3.acceleration.y);
  back_value = constrain(back_value, 0 , 4);
  back_value = map (back_value, 0 , 4 , 0, 255);
  Serial.print("Back_1:");
  /* Display the results (acceleration is measured in m/s^2) */
  //  Serial.print("X: "); Serial.print(back_value); Serial.println("  ");
  Serial.print("Y: "); Serial.print(back_value); Serial.println("  ");
  //  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  "); Serial.println("m/s^2 ");



  /* Delay before the next sample */
//  delay(500);

  /*write the value to p5 sketch*/
  Left_hand_3_Characteristic.writeValue(left_hand_value);
  Right_hand_7_Characteristic.writeValue(right_hand_value);
  Back_1_Characteristic.writeValue(back_value);


}
