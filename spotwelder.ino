#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "props.h"

int buttonPin = D6;
int ledPin = LED_BUILTIN; // BUILTIN LED
int ssrPin = D1;

ESP8266WebServer server(80);

String generate_index() {
  String index = R"rawliteral(<html><body>Spotwelding pattern in milliseconds<br/><FORM ACTION="/" method=POST><table><tr><td></td><td>On</td><td>Off</td><td>On</td><td>Off</td><td>On</td><td>Off</td><td>On</td><td>Off</td></tr><tr><td></td>)rawliteral";
  for(int i=0;i<WELD_PATTERN_LENGTH;++i) {
    index+="<td><input type=text name=pattern";
    index+=String(i);
    index+=" value='";
    index+=String(Props::getInstance()->getWeldPattern(i));
    index+="'></td>";
  }
  index+="</tr></table><br/><input type=submit value='Save'></form></body></html>";
  return index;
}

void handle_index() {
  Serial.println("Handling index request");
  server.send(200, "text/html", generate_index());
  Serial.println("sent");
}

void handle_post() {
  Serial.println("Handling post request");
  String pattern="pattern";
  for(int i=0;i<WELD_PATTERN_LENGTH;++i) {
    String ss=server.arg(pattern+String(i));
    if (ss.isEmpty()) {
      Props::getInstance()->setWeldPattern(i, 0);
    } else
    {
      Props::getInstance()->setWeldPattern(i, ss.toInt());
    }
  Serial.print("Pattern");
  Serial.print(i);
  Serial.print("=");
  Serial.println(Props::getInstance()->getWeldPattern(i));
  Props::getInstance()->commit();
  }
  handle_index();
}

void setup() {
  Serial.begin(115200);
  Props::getInstance()-> get_serial();
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  pinMode(ssrPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(ssrPin, HIGH);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(ssrPin, LOW);
  digitalWrite(ledPin, HIGH);
  // Connect to WiFi network
  Serial.println();
  while (wifiConnect() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  server.on("/", HTTP_GET, handle_index);
  server.on("/", HTTP_POST, handle_post);
  server.begin();
}

int toggle(int state){
  if (state==LOW)
    return HIGH;
  if (state==HIGH)
    return LOW;
  return LOW;
}

void buttonHandler() {
  if (LOW == digitalRead(buttonPin)) {
      int i=0;
      int state=HIGH;
      while(i<WELD_PATTERN_LENGTH) {
        if(0 == Props::getInstance()->getWeldPattern(i)){
          break;
        }
        state=toggle(state);
        digitalWrite(ssrPin, toggle(state));
        digitalWrite(ledPin, state);
        delay(Props::getInstance()->getWeldPattern(i));
        i++;
      }
      digitalWrite(ssrPin, LOW);
      digitalWrite(ledPin, HIGH);
      while(LOW == digitalRead(buttonPin)) {
        delay(100);
      }
  }
}

void loop() {
  server.handleClient();
  buttonHandler();
  delay(1);
}
 