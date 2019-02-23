#include <time.h>
#include <WiFiServerSecure.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiType.h>
#include <CertStoreBearSSL.h>
#include <ESP8266WiFiAP.h>
#include <WiFiClient.h>
#include <BearSSLHelpers.h>
#include <WiFiServer.h>
#include <ESP8266WiFiScan.h>
#include <WiFiServerSecureBearSSL.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiSTA.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiServerSecureAxTLS.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char ssid[] = "getskump4";  //  your network SSID (name)
char pass[] = "getskump1234";       // your network password

String line = "Olle är bäst på allt och snygg som fan" ;

void setup() {
  Serial.begin(115200);

  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();
  lcd.clear();

  lcd.print("booting...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  Serial.print("Waiting for WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(1 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for time");
  while (!time(nullptr)) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
}

void loop() { 
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  Serial.print("gettimeofday() ");
  char tmp[16];
  sprintf(tmp, "%d %02d", tv.tv_sec, tv.tv_usec / 10000);
  Serial.println(tmp);
  lcd.setCursor(0, 1);
  lcd.print(tmp);

  delay(70); 
}
