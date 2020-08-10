void client_checker(void) {
  char a[INPUT_BUFFER_SIZE] = {0};

  struct _time my_time = {{0}, {0}, {0}, {0}, {0}, {0}};
  get_time(&my_time);

  char _time[20] = {0};

  strcpy(_time, "20");
  strcat(_time, my_time._year);
  strcat(_time, my_time._month);
  strcat(_time, my_time._monthDay);
  strcat(_time, " ");
  strcat(_time, my_time._hour);
  strcat(_time, ":");
  strcat(_time, my_time._minute);
  strcat(_time, ":");
  strcat(_time, my_time._second);

  static bool _stopped = false;
    char tmp_cycle_number[10];

  listener(a);

  if (*a == NULL) return;

  while (!check_sd_card());
  if (!sd.chdir()) send_to_lcd("Access to / failed", 0, 1, false);
  char tmp_year[5] = {0};

  strcat(tmp_year, "20");
  strcat(tmp_year, my_time._year);
  if (!sd.exists(tmp_year)) {
    if (!sd.mkdir(tmp_year)) send_to_lcd("Can`t make directory", 0, 1, false);
  }//must be terminated

  sd.chdir(tmp_year);

  if (!sd.exists(my_time._month)) {
    if (!sd.mkdir(my_time._month)) send_to_lcd("Can`t make directory", 0, 1, false);
  }

  sd.chdir(my_time._month);

  switch (process_text(a)) {

    case TOTAL_CYCLES:
      send_to_lcd("Cycle#", 5, 0, true);
      char cycle_number_ascii[10];
     
      memset(cycle_number_ascii, '\0', sizeof(cycle_number_ascii));
      for (unsigned char i = 57, b = 0; i < 64; i++) {
        if (isdigit(a[i])) {
          cycle_number_ascii[b++] = a[i];
          lcd.write(a[i]);
        }
      }
      cycle_number = (unsigned int)atoi(cycle_number_ascii);
      have_cycle_number = true;
      memset(tmp_cycle_number, '\0', sizeof(tmp_cycle_number));
      itoa(cycle_number, tmp_cycle_number, 10);
      strcat(tmp_cycle_number, ".txt");
      break;

    case SETTING_UP_WIFI:
      send_to_lcd("Setting UP WIFI", 0, 1, false);
      return;
      break;

    case WIFI_DISCONNECTED:
      send_to_lcd("Please Set Up WiFi", 0, 1, false);
      return;
      break;

    case GOT_IP:
      char ip_address[16];
      memset(ip_address, '\0', sizeof(ip_address));
      for (char i = 0, b = 0; i < strlen(a); i++) {
        if (isalpha(a[i]) && a[i] != '.') continue;
        ip_address[b++] = a[i];
      }

      send_to_lcd(ip_address, 0, 1, false);
      return;
      break;

    case CYCLE_COMPLETE:
      _complete = true;
    
      if (!file.open(tmp_cycle_number, O_WRONLY | O_CREAT | O_APPEND)) send_to_lcd("Can`t make file", 0, 1, false);
      file.print("\r\n");
      file.println(a);
      file.close();
      sd.remove("tmp.txt");
      my_serial_flush();
      _spi(SPI_END);
      scroll_text();
      have_cycle_number = false;
      return;
      break;

    case STOPPED:
      _complete = false;
      char cycle_number_lcd[10];
      memset(cycle_number_lcd, '\0', sizeof(cycle_number_lcd));
      itoa(cycle_number, cycle_number_lcd, 10);
      send_to_lcd("Cycle#", 0, 0, true);
      lcd.print(cycle_number_lcd);
      lcd.setCursor(0, 1);
      lcd.print("  Manually Stopped  ");
      if (!file.open(tmp_cycle_number, O_WRONLY | O_CREAT | O_APPEND)) send_to_lcd("Can`t make file", 0, 1, false);
      file.print("\r\n");
      file.println(a);
      file.close();
      my_serial_flush();
      have_cycle_number = false;
      return;
      break;

    case BEGIN:
      _complete = false;
      char date_time[21];
      memset(date_time, '\0', sizeof(date_time));
      strcat(date_time, my_time._month);
      strcat(date_time, " ");
      strcat(date_time, my_time._monthDay);
      strcat(date_time, " 20");
      strcat(date_time, my_time._year);
      strcat(date_time, "    ");
      strcat(date_time, my_time._hour);
      strcat(date_time, ":");
      strcat(date_time, my_time._minute);

      if (!file.open(tmp_cycle_number, O_WRONLY | O_CREAT | O_APPEND)) send_to_lcd("Can`t make file", 0, 1, false);
      file.println(a);
      file.print("\r\n");
      if (!m11_new) {
        file.println("------------------------");
        file.println("   Date        Time");
        file.println(date_time);
        file.println("------------------------");
      }
      file.close();
      
     send_to_lcd(blank, 0, 1, false);
     send_to_lcd(a, 0, 1, false);
      return;
      break;

    case STERILIZING:
      eeprom_address = 0;
      data_log_eeprom = true;
      break;

    default:
      break;
  }
  if (!have_cycle_number && !esp_flag) {
    send_to_lcd(blank, 0, 0, false);
    send_to_lcd(_time, 0, 0, false);
  }
       _scroll();
  //delay(1000);
//   send_to_lcd(blank, 0, 1, false);
//     send_to_lcd(a, 0, 1, false);
  

  if (!esp_flag) {
    if (!file.open(tmp_cycle_number, O_WRONLY | O_CREAT | O_APPEND)) send_to_lcd("Can`t make file", 0, 1, false);
    file.print("\r\n");
    file.println(a);
    file.close();
  }

  return;
}
