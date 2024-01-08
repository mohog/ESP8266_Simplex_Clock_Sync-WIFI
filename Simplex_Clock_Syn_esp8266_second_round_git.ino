#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "network-SSID";
const char* password = "password";
const char* NTP_SERVER = "pool.ntp.org";
const char* TZ_INFO    =  "CST6CDT5,M3.2.0,M11.1.0"; //"CET-1CEST-2,M3.5.0/02:00:00,M10.5.0/03:00:00";  // enter your time zone (https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv)  (https://remotemonitoringsystems.ca/time-zone-abbreviations.php)
tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;

int ledPin = D1;

void setup() {
  
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(9600);
  Serial.println();
  Serial.println("\n\nNTP Time Test\n");
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid, password);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    if (++counter > 100) ESP.restart();
    Serial.print ( "." );
  }
  Serial.println("\n\nWiFi connected\n\n");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP() );

  configTime(0, 0, NTP_SERVER);
  // See https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv for Timezone codes for your region
  setenv("TZ", TZ_INFO, 1);
  Serial.println("\nWaiting for Internet time");

  if (getNTPtime(10)) {  // wait up to 10 sec to sync
  } else {
    Serial.println("Time not set");
    ESP.restart();
  }
  showTime(timeinfo);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
}

void loop() {
  getNTPtime(10);
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
   
  // Hourly correction all hours except 05:00 and 17:00 relay turns on at 57 minutes and 54 seconds

  if( (p_tm->tm_hour != 5) && (p_tm->tm_hour != 17) && (p_tm->tm_min == 57) && (p_tm->tm_sec == 55)){
      digitalWrite(ledPin,HIGH);
  }
  
    // Hourly correction all hours except 05:00 and 17:00 relay turns off after 8 seconds
  if( (p_tm->tm_hour != 5) && (p_tm->tm_hour != 17) && (p_tm->tm_min == 58) && (p_tm->tm_sec == 3)){
      digitalWrite(ledPin,LOW);
  }
  
    // 12 hour correction at 05:57 relay turns on at 57 minutes and 54 seconds

  if( (p_tm->tm_hour == 5) && (p_tm->tm_min == 57) && (p_tm->tm_sec == 53)){
      digitalWrite(ledPin,HIGH);
  }
  
      // 12 hour correction at 05:57 relay off after 14 seconds

  if( (p_tm->tm_hour == 5) && (p_tm->tm_min == 58) && (p_tm->tm_sec == 9)){
      digitalWrite(ledPin,LOW);
  }
  
      // 12 hour correction at 17:57 relay turns on at 57 minutes and 54 seconds

  if( (p_tm->tm_hour == 17) && (p_tm->tm_min == 57) && (p_tm->tm_sec == 53)){
      digitalWrite(ledPin,HIGH);
  }
  
      // 12 hour correction at 17:57 relay off after 14 seconds

  if( (p_tm->tm_hour == 17) && (p_tm->tm_min == 58) && (p_tm->tm_sec == 9)){
      digitalWrite(ledPin,LOW);
  }
  
  delay(500);

  

}

bool getNTPtime(int sec) {

  {
    uint32_t start = millis();
    do {
      time(&now);
      localtime_r(&now, &timeinfo);
      //Serial.print(".");
      delay(10);
    } while (((millis() - start) <= (1000 * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900)) return false;  // the NTP call was not successful
    //Serial.print("now ");  Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %m-%d-%y %T", localtime(&now));
    //Serial.println(time_output);
    //Serial.println();
  }
  return true;
}



/*
void showTime(tm localTime) {
  Serial.print(localTime.tm_mday);
  Serial.print('/');
  Serial.print(localTime.tm_mon + 1);
  Serial.print('/');
  Serial.print(localTime.tm_year - 100);
  Serial.print('-');
  Serial.print(localTime.tm_hour);
  Serial.print(':');
  Serial.print(localTime.tm_min);
  Serial.print(':');
  Serial.print(localTime.tm_sec);
  Serial.print(" Day of Week ");
  if (localTime.tm_wday == 0)   Serial.println(7);
  else Serial.println(localTime.tm_wday);
}
*/

 // Shorter way of displaying the time
  void showTime(tm localTime) {
  Serial.printf(
    "%04d-%02d-%02d %02d:%02d:%02d, day %d, %s time\n",
    localTime.tm_year + 1900,
    localTime.tm_mon + 1,
    localTime.tm_mday,
    localTime.tm_hour,
    localTime.tm_min,
    localTime.tm_sec,
    (localTime.tm_wday > 0 ? localTime.tm_wday : 7 ),
    (localTime.tm_isdst == 1 ? "summer" : "standard")
  );
  }
