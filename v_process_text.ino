unsigned char process_text(char a[]) {


  if (strstr(a, "-.COUNT:"))
    return TOTAL_CYCLES;

  else if (strstr(a, "WiFi Disconnected"))
    return WIFI_DISCONNECTED;

  else if (strstr(a, "Setting up WiFi"))
    return SETTING_UP_WIFI;

  else if (strstr(a, "IPa"))
    return GOT_IP;

  else if (strstr(a, "Midmark M11 - v1.0.0")) { // must be checked with new midmark
    m11_new = true;
    return BEGIN;
  }

  else if (strstr(a, "SERIAL"))
    return BEGIN;

  else if (strstr(a, "USER CANCEL"))
    return STOPPED;

  else if (strstr(a, "CYCLE COMPLETE:"))
    return CYCLE_COMPLETE;

  else if (strstr(a, "STER-START") || strstr(a, "STER_START"))
    return STERILIZING;

  else return NOTHING_USEFUL;
}
