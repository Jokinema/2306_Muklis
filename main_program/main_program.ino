#include "Kinematrix.h"
#include "index/digital_input.h"
#include "index/digital_output.h"
#include "modules/communication/wireless/lora/lora-com.h"
#include "modules/time/timer-task.h"

#include <LiquidCrystal_I2C.h>



#define DEBUG 1
// #define SEND_RANDOM 0

enum DeviceAddress {
  DEVICE_ADDRESS_TX_1 = 0x01,
  DEVICE_ADDRESS_RX = 0x02,
  DEVICE_ADDRESS_TX_2 = 0x03
};

DeviceAddress THIS_DEVICE_ADDRESS = DEVICE_ADDRESS_RX;

LoRaModule lora;
LiquidCrystal_I2C lcd(0x27, 16, 2);

TimerTask tim1(50);
TimerTask tim2(100);
TimerTask lcdTim(1000);

DigitalOut ledMerah(7);
DigitalOut ledKuning(6);
DigitalOut ledHijau(5);
DigitalOut buzzer(4);

int data, status = 1, keyFrameAnim = 0;

void setup() {

  Serial.begin(115200);

  Serial.print("Initialize...");
  lcd.init();
  lcd.backlight();

  if (THIS_DEVICE_ADDRESS != DEVICE_ADDRESS_RX) proximitySetup();
  lora.init(10, 9, 8);

  Serial.println("Done..");
}

void loop() {
  if (tim1.triggered()) lora.receive(onReceive);
  if (tim2.triggered()) {
    if (THIS_DEVICE_ADDRESS != DEVICE_ADDRESS_RX) sendData();
  }
  if (lcdTim.triggered()) lcdIdleAnimation();
  
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
