const serviceUuid_Sensor = "6e400001-b5a3-f393-e0a9-e50e24dcca9e"; //set the uuid for the mkr_sensor
const serviceUuid_Receiver = "5725fe87-a47a-4992-8b55-416347f9e9ca"; //set the uuid for the mkr_receiver

const characteristicsUUID_Sensor = {
  txCharacteristic: "6e400002-b5a3-f393-e0a9-e50e24dcca9e",
  rxCharacteristic: "6e400003-b5a3-f393-e0a9-e50e24dcca9e"
};

const characteristicsUUID_Recevier = {
  light_red: "19b10014-e8f2-537e-4f6c-d104768a1214",
  light_blue: "19b10015-e8f2-537e-4f6c-d104768a1214"
};

let myCharacteristic;
let myValue = 0;
let myBLE;

let light_red_Characteristic;
let light_blue_Characteristic;
let light_red_Value = 0;
let light_blue_Value = 0;
let myBLE_Rece;

function setup() {
  // Create a p5ble class
  myBLE = new p5ble();
  myBLE_Rece = new p5ble();

  createCanvas(100, 100);
  textSize(20);
  textAlign(CENTER, CENTER);

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
  // console.log(characteristics[1].uuid);
  for (let i = 0; i < characteristics.length; i++) {
    if (
      characteristics[i].uuid == characteristicsUUID_Sensor.rxCharacteristic
    ) {
      myCharacteristic = characteristics[i];
      myBLE.startNotifications(myCharacteristic, handleNotifications);
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
    } else {
      console.log("nothing");
    }
  }
}

// A function that will be called once got characteristics
function handleNotifications(data) {
  // console.log("data: ", data);
  myValue = Number(data);
  console.log(myValue);
  light_red_Value = myValue;
  myBLE_Rece.write(light_red_Characteristic, light_red_Value);
}

function draw() {
  background(250);
  // Write value on the canvas
  text(light_red_Value, width / 2, height / 2);
  // console.log(light_red_Value);
}
