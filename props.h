#ifndef __PROPS__INCLUDE__H
#define __PROPS__INCLUDE__H
#include <Arduino.h>

#define WIFILEN 16
#define WELD_PATTERN_LENGTH 8

enum WifiMode { WIFIAP, WIFISTA, WIFIDISABLED };

class Props {
  public:
    static Props *getInstance();
    int get_serial() { return _prop_serial; }
    void reset_serial() { _prop_serial = 0; }
    
    WifiMode get_wifi_mode() {return _wifi_mode;}
    void set_wifi_mode(WifiMode wm) { _wifi_mode = wm; }
  
    char* get_wifi_sta_ssid() { return _wifi_sta_ssid; }
    void set_wifi_sta_ssid(const char* ssid) { strncpy(_wifi_sta_ssid, ssid, WIFILEN); }

    char* get_wifi_sta_password() { return _wifi_sta_password; }
    void set_wifi_sta_password(const char* password) { strncpy(_wifi_sta_password, password, WIFILEN); }

    uint8_t get_debug_bits() { return _debug_bits; }
    void set_debug_bits(uint8_t bits) { _debug_bits = bits; }

    u_int getWeldPattern(int index) { return _weldPattern[index]; }
    void setWeldPattern(int index, u_int value) { _weldPattern[index] = value; }

    void commit();
    void reset();
    int sprintf_status(char *buffer);
    
  private:
    static Props *instance;  
    Props();
    int _prop_serial;
    WifiMode _wifi_mode;
    uint8_t _debug_bits;
    char _wifi_sta_ssid[WIFILEN];
    char _wifi_sta_password[WIFILEN];
    u_int _weldPattern[WELD_PATTERN_LENGTH];
};

#define log_i Serial.printf
#define log_e Serial.printf
#define log_d Serial.printf

#endif //__PROPS__INCLUDE__H
