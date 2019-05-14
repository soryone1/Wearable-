/* 

The code for connecting the mkr1010 on the wearable and the the mkr1010 on the dmx lights by BLE
By: Jasper Wang   

*/

const serviceUuid_Sensor = "9262a3a9-bb7f-479d-bc28-298ee8dc593b"; //set the uuid for the mkr_sensor
const serviceUuid_Receiver = "5ce555b5-5730-42d5-85c8-fc1c6bccb968"; //set the uuid for the mkr_receiver

//set the characteristicsUUID for mkr_wearbale
const characteristicsUUID_Sensor = {
  left_hand: "b314d5c1-f2e2-4ee8-9820-d507474ecd30",
  right_hand: "b314d5c2-f2e2-4ee8-9820-d507474ecd30",
  back: "b314d5c3-f2e2-4ee8-9820-d507474ecd30"
};

//set the characteristicsUUID for mkr_DMX
const characteristicsUUID_Recevier = {
  light_red: "3fecff01-25da-44bb-9fc1-d1734cec8d1c",
  light_blue: "3fecff02-25da-44bb-9fc1-d1734cec8d1c",
  light_white: "3fecff03-25da-44bb-9fc1-d1734cec8d1c"
};

//set the Characteristic for mkr_wearable
let left_hand_Characteristic;
let right_hand_Characteristic;
let back_Characteristic;

//set the value for mkr_wearbale
let left_hand_Value = 0;
let right_hand_Value = 0;
let back_Value = 0;

//set the Characteristic for mkr_DMX
let light_red_Characteristic;
let light_blue_Characteristic;
let light_white_Characteristic;

//set the value for mkr_DMX
let light_red_Value = 0;
let light_blue_Value = 0;
let light_white_Value = 0;

//let ble
let myBLE;
let myBLE_Rece;

function setup() {
  // Create a p5ble class
  myBLE = new p5ble();
  myBLE_Rece = new p5ble();

  // Create a 'Connect and Start Notifications' button for MkR_Sensor
  const connectButton = createButton("Connect and Start Notifications");
  connectButton.mousePressed(connectAndStartNotify);

  // Create a 'Connect and Start Notifications' button for MkR_Receiver
  const connectButton_Receive = createButton("Connect and Start Receive");
  connectButton_Receive.mousePressed(connectAndStartReceive);
}

function connectAndStartNotify() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid_Sensor, gotCharacteristics_Sensor);
}

function connectAndStartReceive() {
  // Connect to a device by passing the service UUID
  myBLE_Rece.connect(serviceUuid_Receiver, gotCharacteristics_Receiver);
}

// A function that will be called once got characteristics
function gotCharacteristics_Sensor(error, characteristics) {
  if (error) console.log("error: ", error);
  //   console.log(characteristics[0].uuid);
  for (let i = 0; i < characteristics.length; i++) {
    if (characteristics[i].uuid == characteristicsUUID_Sensor.left_hand) {
      left_hand_Characteristic = characteristics[i];
      myBLE.startNotifications(left_hand_Characteristic, handleLeftHand);
    } else if (
      characteristics[i].uuid == characteristicsUUID_Sensor.right_hand
    ) {
      right_hand_Characteristic = characteristics[i];
      myBLE.startNotifications(right_hand_Characteristic, handleRightHand);
    } else if (characteristics[i].uuid == characteristicsUUID_Sensor.back) {
      back_Characteristic = characteristics[i];
      myBLE.startNotifications(back_Characteristic, handleBack);
    } else {
      console.log("nothing");
    }
  }
}

function gotCharacteristics_Receiver(error, characteristics) {
  if (error) console.log("error: ", error);
  // console.log("characteristics: ", characteristics);
  for (let i = 0; i < characteristics.length; i++) {
    if (characteristics[i].uuid == characteristicsUUID_Recevier.light_red) {
      light_red_Characteristic = characteristics[i];
    } else if (
      characteristics[i].uuid == characteristicsUUID_Recevier.light_blue
    ) {
      light_blue_Characteristic = characteristics[i];
    } else if (
      characteristics[i].uuid == characteristicsUUID_Recevier.light_white
    ) {
      light_white_Characteristic = characteristics[i];
    } else {
      console.log("nothing");
    }
  }
}

// A function that will be called once got characteristics
function handleLeftHand(data) {
  // console.log("data: ", data);
  light_red_Value = Number(data);
  //   console.log(light_red_Value);
  myBLE_Rece.write(light_red_Characteristic, light_red_Value);
}

function handleRightHand(data) {
  // console.log("data: ", data);
  light_blue_Value = Number(data);
  //   console.log(light_blue_Value);
  myBLE_Rece.write(light_blue_Characteristic, light_blue_Value);
}

function handleBack(data) {
  // console.log("data: ", data);
  light_white_Value = Number(data);
  //   console.log(light_white_Value);
  myBLE_Rece.write(light_white_Characteristic, light_white_Value);
}
