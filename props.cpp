#include <stdio.h>
#include <EEPROM.h>            // read and write from flash memory
#define PROPS_SERIAL 20214
#include "props.h"
Props *Props::instance = 0;

int Props::sprintf_status(char *buf) {
  char* p = buf;
  p+=sprintf(p,"\"serial:\": %u,", _prop_serial);
  p+=sprintf(p,"\"debug_bits:\": %u,", _debug_bits);
  p+=sprintf(p,"\"wifi_mode:\": %u,", _wifi_mode);
  p+=sprintf(p,"\"wifi_sta_ssid:\": \"%s\",", _wifi_sta_ssid);
  p+=sprintf(p,"\"wifi_sta_password:\": \"%s\",", _wifi_sta_password);
  for(int i=0;i<WELD_PATTERN_LENGTH;++i) {
    p+=sprintf(p,"\"weldpattern[%d]:\": \"%d\"", i, _weldPattern[i]);
  }
  return (p-buf);
}

Props *Props::getInstance() {
  if (!instance) {
    instance = new Props();
    log_d("Comparing serial %d with const %d\n",instance->_prop_serial,PROPS_SERIAL);
    if (instance->_prop_serial != PROPS_SERIAL) {
      log_d("Old Props found, reseting.....\n");
      instance->reset();
    }
  }
  return instance;
}

Props::Props():_prop_serial(0),             
               _wifi_mode(WIFIAP),_debug_bits(0) {
  memset(_weldPattern, 0, sizeof(u_int) * WELD_PATTERN_LENGTH);
  log_d("Reading EEPROM Props");
  EEPROM.begin(sizeof(*this));
  EEPROM.get(0,*this);
  EEPROM.end();
}

void Props::reset() {
  _prop_serial = PROPS_SERIAL;
  _wifi_mode=WIFIAP;
  _debug_bits = 0;
  memset(_weldPattern, 0, sizeof(u_int) * WELD_PATTERN_LENGTH);
  memset(_wifi_sta_ssid, 0, WIFILEN);
  memset(_wifi_sta_password, 0, WIFILEN);
  commit();
}

void Props::commit() {
  EEPROM.begin(sizeof(*this));
  EEPROM.put(0,*this);
  EEPROM.commit();
  EEPROM.end();
}

