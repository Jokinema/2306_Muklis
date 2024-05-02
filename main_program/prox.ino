void proximitySetup() {
  pinMode(A7, INPUT);
}

int getDataProx() {
  int data = analogRead(A7);
  
#ifdef DEBUG
  Serial.print("| NODE :");
  Serial.print(THIS_DEVICE_ADDRESS);
  Serial.print(" | ");
  Serial.print("[SENSOR] PROX READ :");
  Serial.print(data);
  Serial.print(" | Logic : ");
  Serial.println(data < 512 ? 1 : 0);
#endif

  return data < 512 ? 1 : 0;
}
