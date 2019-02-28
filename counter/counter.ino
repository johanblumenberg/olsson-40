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

char ssid[] = "saftkalas";
char pass[] = "8005300167";

const char line[] = "Game Over when counter reaches zero";
long long DATE = 1582588800000LL;

enum State {
  COUNTDOWN,
  PANIC,
  BLANK,
  ONEUP
};

State state(long long now) {
  if (now > DATE - 10*1000) {
    return ONEUP;
  } else if (now > DATE - 25*1000) {
    return BLANK;
  } else if (now > DATE - 60*60*1000) {
    return PANIC;
  } else {
    return COUNTDOWN;
  }
}

long long time() {
  struct timeval tv;
  gettimeofday(&tv, nullptr);
  return 1000LL * tv.tv_sec + tv.tv_usec / 1000;  
}

void updateText(long long now) {
  static int         p = 0;
  static long long   t = 0;
  static const char* str = line;
  static int         len = strlen(str);
  static const int   DT = 350;
  
  if (t == 0) {
    p = 16;
    t = now;
  } else if (now - t > DT) {
    t += DT;
    
    p--;
    if (p < -len) {
      p = 16;
    }

    char tmp[17];

    for (int i = 0; i < 16; i++) {
      if (i < p) {
        tmp[i] = ' ';
      } else if (i >= p + len) {
        tmp[i] = ' ';
      } else {
        tmp[i] = str[i - p];
      }
      tmp[16] = '\0';
    }
    
    Serial.println(tmp);
  
    lcd.setCursor(0, 1);
    lcd.print(tmp);
  }
}

void updateTime(long long now) {
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
  
  char tmp[17];
  sprintf(tmp, "%4d %02d:%02d:%02d.%02d", d, h, m, s, cs);

  Serial.println(tmp);
  
  lcd.setCursor(0, 0);
  lcd.print(tmp);
}

void updatePanic(long long now) {
  long long delta = DATE - now;

  delta /= 10;
  int cs = delta % 100;
  delta /= 100;
  int s = delta % 60;
  delta /= 60;
  int m = delta % 60;

  char c1 = (now + 800) % 1700 < 300 ? '!' : ' ';
  char c2 = (now + 400) % 2300 < 300 ? '!' : ' ';
  char c3 = (now + 900) % 1100 < 300 ? '!' : ' ';
  char c4 = (now + 200) % 1900 < 300 ? '!' : ' ';
  char c5 = (now + 600) % 1800 < 300 ? '!' : ' ';
  char c6 = (now + 100) % 2500 < 300 ? '!' : ' ';

  const char* panic = (now % 1500 > 750) ? "PANIC!" : "      ";

  char tmp[17];
  sprintf(tmp, "%c%c%c %02d:%02d.%02d %c%c%c", c1, c2, c3, m, s, cs, c4, c5, c6);

  Serial.println(tmp);
  
  lcd.setCursor(0, 0);
  lcd.print(tmp);

  sprintf(tmp, "%c%c %c %s %c %c%c", c5, c4, c6, panic, c1, c3, c2);

  lcd.setCursor(0, 1);
  lcd.print(tmp);
}

void updateBlank(long long now) {
  Serial.println("blank");

  char dot = (now % 3000 > 1500) ? '.' : ' ';

  char tmp[17];
  sprintf(tmp, " %c              ", dot);
  
  lcd.setCursor(0, 0);
  lcd.print(tmp);
  lcd.setCursor(0, 1);
  lcd.print("                ");  
}

void updateOneUp(long long now) {
  Serial.println("Extra Life!");

  char excm = (now % 1500 > 750) ? '!' : ' ';

  char tmp[17];
  sprintf(tmp, " %c    Extra   %c ", excm, excm);
  
  lcd.setCursor(0, 0);
  lcd.print(tmp);
  lcd.setCursor(0, 1);
  lcd.print("      Life!     ");  
}

void setup() {
  Serial.begin(115200);

  Wire.begin(D2, D1);
  lcd.begin();
  lcd.home();
  lcd.clear();

  lcd.print("booting..");

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

  lcd.print(".");

  configTime(1 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for time");
  while (time() < 946684800000LL) { // 2000-01-01
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  
  lcd.clear();

// For testing, close to the point when countdown reaches zero
//  delay(1000);
//  DATE = time() + 60*1000;
}

void loop() {
  long long now = time();

  switch (state(now)) {
    case COUNTDOWN:
      updateTime(now);
      updateText(now);
      break;

    case PANIC:
      updatePanic(now);
      break;

    case BLANK:
      updateBlank(now);
      break;
      
    case ONEUP:
      updateOneUp(now);
  }

  delay(70); 
}
