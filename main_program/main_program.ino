#include "Kinematrix.h"
#include "index/digital_input.h"
#include "index/digital_output.h"
#include "modules/communication/wireless/lora/lora-com.h"
#include "modules/time/timer-task.h"

#define DEBUG 1
// #define SEND_RANDOM 0

enum DeviceAddress {
  DEVICE_ADDRESS_TX_1 = 0x01,
  DEVICE_ADDRESS_RX = 0x02,
  DEVICE_ADDRESS_TX_2 = 0x03
};

DeviceAddress THIS_DEVICE_ADDRESS = DEVICE_ADDRESS_TX_1;

LoRaModule lora;

TimerTask tim1(50);
TimerTask tim2(100);

// DigitalIn prox(4);

DigitalOut ledMerah(7);
DigitalOut ledKuning(6);
DigitalOut ledHijau(5);
DigitalOut buzzer(4);

int data, status = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("Initialize");

  if (THIS_DEVICE_ADDRESS != DEVICE_ADDRESS_RX) proximitySetup();
  lora.init(10, 9, 8);
}

void loop() {
  if (tim1.triggered()) lora.receive(onReceive);
  if (tim2.triggered()) {
    if (THIS_DEVICE_ADDRESS != DEVICE_ADDRESS_RX) sendData();
  }

  ledMerah.update();
  ledKuning.update();
  ledHijau.update();
  buzzer.update();

#ifdef DEBUG
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil("\n");
    data = sendDataFromSerial(input);
    lora.clearData();
    lora.addData(THIS_DEVICE_ADDRESS);
    lora.addData(data);
    lora.addData(status);
    lora.sendDataAsyncCb(1000, sendDataCallback);
  }
#endif
}

void sendData() {
  data = getDataProx();
  if (data == 1 && THIS_DEVICE_ADDRESS != DEVICE_ADDRESS_RX) {
    lora.clearData();
    lora.addData(THIS_DEVICE_ADDRESS);
    lora.addData(data);
    lora.addData(status);
    lora.sendDataAsyncCb(1000, sendDataCallback);
  }
}

void sendDataCallback() {
  ledKuning.on();
  ledKuning.offDelay(200);

#ifdef DEBUG
  Serial.print("| NODE :");
  Serial.print(THIS_DEVICE_ADDRESS);
  Serial.print("| SEND: ");
  Serial.print(data);
  Serial.print("| ");
  Serial.print(status);
  Serial.println();
#endif
}

void proximitySetup() {
  pinMode(A7, INPUT);
}

int getDataProx() {
  int data = analogRead(A7);

#ifdef DEBUG
  Serial.print("| NODE :");
  Serial.print(THIS_DEVICE_ADDRESS);
  Serial.print("[SENSOR] PROX READ :");
  Serial.println(data);
#endif

  return data < 512 ? 1 : 0;
}

int sendDataFromSerial(String input) {
  input.trim();
  return input.toInt();
}
void onReceive(String datas) {
  int addressRecv = (int)lora.getData(datas, 0, ";");
  int dataRecv = lora.getData(datas, 1, ";");
  bool statusRecv = lora.getData(datas, 2, ";");

  if (THIS_DEVICE_ADDRESS == DEVICE_ADDRESS_TX_1 || THIS_DEVICE_ADDRESS == DEVICE_ADDRESS_TX_2) {
    if (addressRecv == DEVICE_ADDRESS_RX) {
      ledHijau.on();
      ledHijau.offDelay(200);
      printReceivedData(addressRecv, dataRecv, statusRecv);
    }
  } else {
    if (addressRecv == DEVICE_ADDRESS_TX_1) {
      ledMerah.on();
      ledMerah.offDelay(200);
      printReceivedData(addressRecv, dataRecv, statusRecv);
    }
    if (addressRecv == DEVICE_ADDRESS_TX_2) {
      ledHijau.on();
      ledHijau.offDelay(200);
      printReceivedData(addressRecv, dataRecv, statusRecv);
    }
  }
}

void printReceivedData(int address, int data, bool status) {
#ifdef DEBUG
  Serial.print("| NODE ");
  Serial.print(THIS_DEVICE_ADDRESS == DEVICE_ADDRESS_RX ? "RX " : "TR ");
  Serial.print("FROM :");
  Serial.print(address);
  Serial.print("| RECV: ");
  Serial.print(data);
  Serial.print("| STAT: ");
  Serial.println(status);
#endif
}
