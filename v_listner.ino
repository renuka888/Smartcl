void listener(char a[]) {

  unsigned char  i = 0;
  unsigned long _time = 0;
  bool flag = false;
  const unsigned int timeout =500;//from 1ms must be adjusted

  _time = millis();

  do{
    if (Serial1.available()) {
      esp_flag=false;
      a[i] =  Serial1.read();
      //Serial.print(a[i]);
      if (a[i] >= 32 && a[i] <= 127) {
        flag = true;
        i == (INPUT_BUFFER_SIZE - 1) ? i-- : i++;
    
      }
    }
  } while ((millis() - _time) <= timeout);//

  if (flag) a[--i] = '\0';
  Serial.print(a);
  if (!esp_flag) {
    if (strstr(a, "STER-END"))
    {
      EEPROM.put(eeprom_address, 255);
      eeprom_address = 0;
      data_log_eeprom = false;
    }

    if (data_log_eeprom && *a) { //a[0]

      if (strlen(a) > 20) {
        char b[strlen(a) + 1]={0};

        char *p = a;

        if (*p == ' ') {
          while (*p++ == ' ');
          --p;
        }

        for (char *c = b; p < a + (strlen(a)); ) {
          if (*p == ' ' && *(p + 1) == ' ') p++;
          else *c++ = *p++;
        }

        for (char *i = b; i <= b + strlen(b); i++) {
          EEPROM.put(eeprom_address, *i);
          eeprom_address += sizeof(char);
        }
      }

      else {
        for (char *i = a; i <= a + strlen(a); i++) {
          EEPROM.put(eeprom_address, *i);
          eeprom_address += sizeof(char);
        }
      }
    }
  }
}
