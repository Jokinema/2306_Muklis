
void sendData() {
  data = getDataProx();
  if (data == 1 && THIS_DEVICE_ADDRESS != DEVICE_ADDRESS_RX) {
    lora.clearData();
    lora.addData(THIS_DEVICE_ADDRESS);
    lora.addData(data);
    lora.addData(status);
    for (int i = 0; i < 20; i++) lora.sendDataAsyncCb(1000, sendDataCallback);
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

      buzzer.on();
      buzzer.offDelay(1000);

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
