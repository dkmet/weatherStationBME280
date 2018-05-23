## Vremenska postaja 

Oprema:

-ESP8266 NodeMcu 12 (0.9v)

-BMEP280


## Povezava BMEP280 z NodeMcu ESP8266

BMEP280
	NodeMCU
	
VIN
		3.5V
		
GND
		GND
		
SCL
		D1
		
SDA
		D2
		

Deep-Sleep:
NodeMCU
	NodeMCU
	
RST
		D0
		


Baterija:

+ na 5v

- na GND



## Nastavitve

Nastavitve se nahajajo v datoteki "settings.h", ob datoteki weather-station.ino.

```
#define server_url "http://username:geslo@www.moj-url.si/(webRequest)?openAgent&requestType=createNewDoc"
#define wifi_ssid "ssid_name"
#define wifi_password "wifi_password"
#define time_interval_min 30
const String weather_location= "ljubljana";
```
## Upload kode
Pred uploadom kode je potrebno izkllučiti deep sleep RST -D0, na ESPju

## Navodila za delo z github cli

git add README.md

git commit -m "-"

git push

## Zunanje povezave

[Deep-Sleep](https://www.losant.com/blog/making-the-esp8266-low-powered-with-deep-sleep)

[Povezava z baterijo](http://henrysbench.capnfatz.com/henrys-bench/arduino-projects-tips-and-more/powering-the-esp-12e-nodemcu-development-board/)
