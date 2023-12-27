#include <ESP8266WiFi.h>

char* ssid=new char[23];
const char* password = "SpotWelder1234";

void fillSSIDTemplate() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  sprintf(ssid, "SpotWelder-%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);  
}

int wifiConnect() {
  switch (Props::getInstance()->get_wifi_mode()) {
    case WIFIAP: return wifiAPConnect();break;
    case WIFISTA: return wifiSTAConnect();break;
    case WIFIDISABLED: log_i("Wifi disabled.\n");return WL_DISCONNECTED;break; 
  }
  log_e("Invalid mode: '%d'\n",Props::getInstance()->get_wifi_mode());
  return WL_NO_SHIELD;
}

int wifiSTAConnect() {
  log_d("Connecting to '%s' with password '%s'\n",Props::getInstance()->get_wifi_sta_ssid(), Props::getInstance()->get_wifi_sta_password());
  WiFi.begin(Props::getInstance()->get_wifi_sta_ssid(), Props::getInstance()->get_wifi_sta_password());
  delay(500);
  return WiFi.waitForConnectResult();
}

bool wifiSTADisconnect() {
  return WiFi.disconnect(true);
}

int wifiAPConnect() {
  fillSSIDTemplate();
  IPAddress local_ip(192,168,55,1);
  IPAddress gateway(192,168,55,1);
  IPAddress subnet(255,255,255,0);
  log_i("SSID: '%s' password: '%s'\n",ssid, password);
  bool res = WiFi.softAP(ssid, password);
  if (res) {
    log_i("Soft AP OK");
  }
  else { log_e("Soft AP FAILED"); }
  WiFi.softAPConfig(local_ip, gateway, subnet);
  return WL_CONNECTED;
}

bool wifiAPDisconnect() {
  return WiFi.softAPdisconnect(true);
}
