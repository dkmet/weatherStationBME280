/*
 * BME280 GND --> NodeMCU GND
 * BME280 3.3V --> NodeMCU 3V3
 * BME280 SDA --> NodeMCU D2
 * BME280 SCL --> NodeMCU D1
 */


#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "settings.h"



// const float SEA_LEVEL_PRESSURE_HPA = 1013.25;
// const int DELAY = 2000;
// const int STARTUP_DELAY = 500;

Adafruit_BME280 bme;

void setup() {

  Serial.begin(115200);
 
  // Počakam da se postavi ESP
  while(!Serial) { }  
  Serial.println("");
  Serial.println("Zagon");
  

  // pošljem podatke
  sendData();
  
  Serial.println("Spanje " +  String(time_interval_min)+ " min");
  Serial.println("");
  ESP.deepSleep(60e6 * time_interval_min); // spanje time_interval_min minut  
}

void sendData(){
  String payload; // vrnje niz iz strežnika  
  int httpCode;  // odgovor iz strežniaka
  float h;     // vlaga
  float t;     //temperatura
  float p;   //tlak v hPa
  int sleepIteraction;        //ponovitve spanja
  // Zaženem DHT senzor
  
  bme.begin();
 
 // Povežem na WIFI
  Serial.print("Povezujem se na wifi."); 
  WiFi.begin(wifi_ssid, wifi_password);  
  unsigned long wifiConnectStart = millis(); 
  while (WiFi.status() != WL_CONNECTED) {
   if (WiFi.status() == WL_CONNECT_FAILED) {
      Serial.println("Napaka: Neuspešna povezava. Prosimo preveriti podatke za brezžično povezavo.");
      delay(10000);
   }

    delay(500);
    Serial.print(".");
    if (millis() - wifiConnectStart > 15000) {
      Serial.println("Neuspešna povezava do internega");
      return;
    }
  }
  Serial.println("");  
  Serial.println("WiFi povezava je vzpostavljena");
  Serial.print("Naslov IP: ");
  Serial.println(WiFi.localIP());

      // preberem podatke iz DHT senzorja

  // Temperature    
   t = bme.readTemperature();    
  h = bme.readHumidity();
  p = bme.readPressure() / 100.0;
  
  sleepIteraction =0;
  if(isnan(t)){
    Serial.print("Branje podatkov iz senzorja .");  
    while(isnan(t) && sleepIteraction < 30){
      delay(500);
      Serial.print(".");
      t = bme.readTemperature();    
      h = bme.readHumidity();
      p = bme.readPressure() / 100.0;
      sleepIteraction = sleepIteraction +1;
    }  
    Serial.println("");  
  }
 
  
  // v primeru napake pri branju senzorja DHT
  // izpišem napako
  // zaprem http senzor
  // poskusim vsakih 10s
  // prekinem nadaljevanje izvajanja
   if (isnan(h) || isnan(t)) {
    Serial.println("Napaka: Napaka pri branju podatkov iz senzorja.");
    return;
  }

  // Povezava do strežnika
  HTTPClient http;
  http.begin(server_url); 
  http.addHeader("Content-Type", "application/json"); 
  
  // pošljem temperaturo
  // v primeru da je prišlo do napake, izpišem napako, drugale temperaturo
   httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"1\"],\"idlokacija\":[\"4#" + weather_location + "\"],\"vrednost\":["+ String(t) +"]}}");
   payload = http.getString();                  //Get the response payload  
   if(httpCode != 200){    
    Serial.println(payload);    //Print request response payload
   }else{
    Serial.println("Temperatura: "+String(t)+ " C");
   }
   
 
  // pošljem vlago
  // v primeru da je prišlo do napake, izpišem napako,
  // v nasprotnem primeru izpišem vlago.
     httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"2\"],\"idlokacija\":[\"4#" + weather_location + "\"],\"vrednost\":["+ String(h) +"]}}");
     payload = http.getString();                  //Get the response payload  
    if(httpCode != 200){
      payload = http.getString();         
       Serial.println(payload);   
    }else{
      Serial.println("Vlaga: "+String(h)+ "%");
    }
  
  // pošljem tlak
  // v primeru da je prišlo do napake, izpišem napako,
  // v nasprotnem primeru izpišem tlak
     httpCode = http.POST("{\"requestParams\":{\"module\":\"VREME\",\"doctypeindex\":[21],\"senzortype\":[\"3\"],\"idlokacija\":[\"4#" + weather_location + "\"],\"vrednost\":["+ String(p) +"]}}");
     payload = http.getString();                  //Get the response payload  
    if(httpCode != 200){
      payload = http.getString();         
       Serial.println(payload);   
    }else{
      Serial.println("Tlak: "+String(p)+ "hPa");
    }
     
     // Zaprem HTTP clienta
   http.end(); 
}


void loop() {
}

