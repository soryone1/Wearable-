const serviceUuid_Sensor = "19b10010-e8f2-537e-4f6c-d104768a1214"; //set the uuid for the mkr_sensor
const serviceUuid_Receiver = "5725fe87-a47a-4992-8b55-416347f9e9ca"; //set the uuid for the mkr_receiver

const characteristicsUUID_Sensor = {
  button: "19b10013-e8f2-537e-4f6c-d104768a1214",
  sensor: "19b10012-e8f2-537e-4f6c-d104768a1214"
};

const characteristicsUUID_Recevier = {
  light_red: "19b10014-e8f2-537e-4f6c-d104768a1214",
  light_blue: "19b10015-e8f2-537e-4f6c-d104768a1214"
};

let buttonCharacteristic;
let sensorCharacteristic;
let buttonValue = 0;
let sensorValue = 0;
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

  createCanvas(600, 400);
  colorMode(HSB, 255);
  background("#FFF");

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
    if (characteristics[i].uuid == characteristicsUUID_Sensor.button) {
      buttonCharacteristic = characteristics[i];
      myBLE.startNotifications(buttonCharacteristic, handleButton);
    } else if (characteristics[i].uuid == characteristicsUUID_Sensor.sensor) {
      sensorCharacteristic = characteristics[i];
      myBLE.startNotifications(sensorCharacteristic, handleSensor);
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
function handleButton(data) {
  // console.log("data: ", data);
  buttonValue = Number(data);
}

function handleSensor(data) {
  // console.log("data: ", data);
  sensorValue = Number(data);
  light_red_Value = sensorValue;
  myBLE_Rece.write(light_red_Characteristic, light_red_Value);
}

function draw() {
  noStroke();

  if (buttonValue > 0) {
    fill(color(200, 200, 200));
  } else {
    fill(color(100, 200, 200));
  }

  ellipse(sensorValue, 100, 20, 20);
}
