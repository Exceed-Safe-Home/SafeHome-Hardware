#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define smoke_s 36 //Smoke
#define gas_s 39 //Gas
#define flame_s 32
#define shock_s 33
#define water_s 34

hw_timer_t *timer1 = NULL;

const int _size = 2*JSON_OBJECT_SIZE(5);
StaticJsonDocument<_size> JSONPut;
StaticJsonDocument<_size> JSONGet;

char str[100];
const char* ssid = "ZisZtR";
const char* password = "asd123456";

const char* put_url = "https://ecourse.cpe.ku.ac.th/exceed07/api/update_sensor/"; //put value through username

int water;
int gas;
int smoke;
int flame;
int shake;

String Serialnumber="01"; 
double check=0;

void WiFi_Connect(){
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");  
  }
  Serial.println("Connected to network"); 
  Serial.print("IP Adress : "); 
  Serial.println(WiFi.localIP());   
}

void _put(){
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(put_url+ Serialnumber);
    http.addHeader("Content-Type", "application/json");
  
    JSONPut["water_level"] = water;
    JSONPut["gas"] = gas;
    JSONPut["smoke"] = smoke;
    JSONPut["flame"] = flame;
    JSONPut["shake"] = shake;
       
    serializeJson(JSONPut, str);
    int httpCode = http.PUT(str);
    if(httpCode == HTTP_CODE_OK){
      String payload = http.getString();  
      Serial.println(httpCode); 
      Serial.println(payload);       
    }else{
      Serial.println(httpCode);
      Serial.println("ERROR on HTTP request");        
    }
  }else{
    WiFi_Connect();  
  }
  delay(100);
}

void earthquake(){
  if(check!=digitalRead(shock_s)){
    shake=0;
    check=!check;
  } else shake = 1;
}

void setup() {
  pinMode(smoke_s, INPUT);
  pinMode(gas_s, INPUT);
  pinMode(flame_s, INPUT);
  pinMode(shock_s, INPUT);
  pinMode(water_s, INPUT);
  Serial.begin(9600);
  WiFi_Connect();
  //timer for earthquake
  timer1 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer1, earthquake, true);
  timerAlarmWrite(timer1, 100000, true);
  timerAlarmEnable(timer1);
  delay(100);
}

void loop() {
  water = analogRead(water_s);
  gas = analogRead(gas_s);
  smoke = analogRead(smoke_s);
  flame = analogRead(flame_s);
  //shake = digitalRead(shock_s);
  
  //Serial.print("Shake: ");
  //Serial.println(shake);

  //Serial.print("Water: ");
  //Serial.println(water);

  /*Serial.print("Flame: ");
  Serial.println(flame);*/

  /*Serial.print("Gas: ");
  Serial.print(gas);

  Serial.print(" | Smoke: ");
  Serial.println(smoke);*/

  _put();
  delay(100);
}
