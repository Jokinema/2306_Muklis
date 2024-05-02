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

  lcd.clear();

  lcd.setCursor(3, 0);
  lcd.print(" ADA KERETA ");

  lcd.setCursor(0, 1);
  lcd.print(" -= Dari     =- ");

  if (address == DEVICE_ADDRESS_TX_1) {
    lcd.setCursor(0, 0);
    lcd.print("<<");
    lcd.setCursor(9, 1);
    lcd.print("TX1");
  }
  if (address == DEVICE_ADDRESS_TX_2) {
    lcd.setCursor(14, 0);
    lcd.print(">>");
    lcd.setCursor(9, 1);
    lcd.print("TX2");
  }

  delay(3000);
  lcd.clear();
}

void lcdIdleAnimation() {
  switch (keyFrameAnim) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("    TIDAK ADA   ");
      lcd.setCursor(0, 1);
      lcd.print("     KERETA     ");
      break;

    case 1:
      lcd.setCursor(0, 0);
      lcd.print("    TIDAK ADA   ");
      lcd.setCursor(0, 1);
      lcd.print("  -  KERETA     ");
      break;
      
    case 2:
      lcd.setCursor(0, 0);
      lcd.print("    TIDAK ADA   ");
      lcd.setCursor(0, 1);
      lcd.print("  -= KERETA     ");
      break;
    case 3:
      lcd.setCursor(0, 0);
      lcd.print("    TIDAK ADA   ");
      lcd.setCursor(0, 1);
      lcd.print("  -= KERETA =   ");
      break;
    case 4:
      lcd.setCursor(0, 0);
      lcd.print("    TIDAK ADA   ");
      lcd.setCursor(0, 1);
      lcd.print("  -= KERETA =-  ");
      keyFrameAnim = -1;
      break;
  }
  keyFrameAnim = keyFrameAnim + 1;
}
