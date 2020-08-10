void scroll_text(void) {
  send_to_lcd("Cycle ", 0, 0, true);
  lcd.print(cycle_number);
  send_to_lcd(" Complete",11,0,false);
  _delay_ms(1000);

  while (!Serial1.available()) {//no need to check for esp because esp is not sending any data when its done its setup
    unsigned char i = 0;
    char tmp = 0;

    do {
      _delay_ms(50);
      EEPROM.get(eeprom_address, tmp);

      if (tmp == 255) {
        eeprom_address = 0;
        break;
      }
      if (tmp != '\0' && i <= 20) {
        lcd.setCursor(i++, 1);
        lcd.write(tmp);
      }
      else if (tmp == '\0') {
        _delay_ms(1700);
        send_to_lcd(blank, 0, 1, false);
        i = 0;
      }
      eeprom_address += sizeof(char);
    } while (tmp != 255 && !Serial1.available());//no need to check for esp because esp is not sending any data when its done its setup
  }
}
