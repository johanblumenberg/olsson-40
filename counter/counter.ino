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
long long DATE = 1582588800000LL;

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
  long long now = 1000LL * tv.tv_sec + tv.tv_usec / 1000;
  long long delta = DATE - now;

  delta /= 10;
  int cs = delta % 100;
  delta /= 100;
  int s = delta % 60;
  delta /= 60;
  int m = delta % 60;
  delta /= 60;
  int h = delta % 24;
  delta /= 24;
  int d = delta;
  
  char tmp[16];
  sprintf(tmp, "%4d %02d:%02d:%02d.%02d", d, h, m, s, cs);

  Serial.println(tmp);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(tmp);

  delay(70); 
}
