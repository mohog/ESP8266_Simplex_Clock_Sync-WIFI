#include <ESP8266WiFi.h>
#include <time.h>

const char* ssid = "<your wifi ssid>";
const char* password = "<your wifi password>";

int ledPin = 13;

int timezone = -6 * 3600; //CST
int dst = 0;

void setup() {
  
  pinMode(ledPin,OUTPUT);
  digitalWrite(ledPin,LOW);

  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  Serial.println();

  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");   
}

void loop() {
  
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
   
  // Hourly correction all hours except 05:00 and 17:00 relay turns on at 57 minutes and 54 seconds

  if( (p_tm->tm_hour != 5) && (p_tm->tm_hour != 17) && (p_tm->tm_min == 57) && (p_tm->tm_sec == 54)){
      digitalWrite(ledPin,HIGH);
  }
  
    // Hourly correction all hours except 05:00 and 17:00 relay turns off after 8 seconds
  if( (p_tm->tm_hour != 5) && (p_tm->tm_hour != 17) && (p_tm->tm_min == 58) && (p_tm->tm_sec == 2)){
      digitalWrite(ledPin,LOW);
  }
  
    // 12 hour correction at 05:57 relay turns on at 57 minutes and 54 seconds

  if( (p_tm->tm_hour == 5) && (p_tm->tm_min == 57) && (p_tm->tm_sec == 54)){
      digitalWrite(ledPin,HIGH);
  }
  
      // 12 hour correction at 05:57 relay off after 14 seconds

  if( (p_tm->tm_hour == 5) && (p_tm->tm_min == 58) && (p_tm->tm_sec == 8)){
      digitalWrite(ledPin,LOW);
  }
  
      // 12 hour correction at 17:57 relay turns on at 57 minutes and 54 seconds

  if( (p_tm->tm_hour == 17) && (p_tm->tm_min == 57) && (p_tm->tm_sec == 54)){
      digitalWrite(ledPin,HIGH);
  }
  
      // 12 hour correction at 17:57 relay off after 14 seconds

  if( (p_tm->tm_hour == 17) && (p_tm->tm_min == 58) && (p_tm->tm_sec == 8)){
      digitalWrite(ledPin,LOW);
  }
  
  delay(500);

  

}
