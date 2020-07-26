#include <LiquidCrystal.h>
#include <OneWire.h> 
#include <DallasTemperature.h>
#include <ESP8266WiFi.h> 

// Connections for LCD need to be as follows:
// LiquidCrystal lcd(rs, rw, E, d4, d5, d6, d7);
LiquidCrystal lcd(2, 0, 4, 5, 12, 14, 16);


//Define pin used for temp probes
#define ONE_WIRE_BUS 13
// Setup a oneWire instance to communicate with D18S20's
OneWire oneWire(ONE_WIRE_BUS);  
// Pass oneWire reference to DallasTemperature library
DallasTemperature sensors(&oneWire);

// Variable defs
int timeout = 0;
int deviceCount = 0;
float temperature;
float mytemps[4];


//Wifi information use to connect to wifi
char* ssid = "Wifiname";
const char* password =  "passphrase";

//Information host to recieve data 
const uint16_t port = 8090; //Port socket is listening on
const char * host = "192.168.1.1"; //ip address socket is listening on.


void setup()
{
  Serial.begin(9600); 
  lcd.begin(20,4);              // Setup size of LCD
  lcd.clear();                  // start with a blank screen
  sensors.begin();  // Start up the sensor library
  lcd.setCursor(0,0); //Set cusor to position 0 first line of display
  lcd.print("Locating devices..."); //write text on first line of display
  lcd.setCursor(0,1);
  lcd.print("Found "); 
  deviceCount = sensors.getDeviceCount(); // locate devices on the bus and return number of devices
  lcd.print(deviceCount); 
  lcd.print(" devices.");
  lcd.setCursor(0,3);
  lcd.print("Sleeping ");
  lcd.print("3 ");
  delay(1000);
  lcd.print("2 ");
  delay(1000);
  lcd.print("1 ");
  delay(1000);
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("Trying to connect to");
  lcd.setCursor(0,1);
  lcd.print(ssid);  
  

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED and timeout < 20) {
    delay(500);
    lcd.println("..");
    timeout = timeout + 1;
    }
  lcd.clear(); 
  if ( timeout >= 20) { //if connection to wifi have not been made within the time print information.
    lcd.setCursor(0,0);  
    lcd.print("Failed to connect to wifi");
    lcd.setCursor(0,1);
    lcd.print("Data will not not be send");
    delay(2000);
    }
}

void loop() 
{
  WiFiClient client;
  while (true) {
    sensors.requestTemperatures();
    // Display temperature from each sensor by looping over the number of sensors previously obtained.
    // By using the index to select the line in the display the sensors are printed on individual lines in the LCD
    for (int i = 0;  i < deviceCount;  i++) {
      lcd.setCursor(0,i);
      lcd.print("Sensor ");
      lcd.print(i+1);
      lcd.print(" : ");
      temperature = sensors.getTempCByIndex(i);
      lcd.print(temperature);
      lcd.print((char)223);//shows degrees symbol
      lcd.print("C ");
      mytemps[i] = temperature; //write temperature to array to alway to a fixed length array to the server
      }
    if (timeout < 20) { //only send data if wifi connection didn't timeout during setup
      if (!client.connected()) { //if connection to socket server no established, establish it
        client.connect(host, port);
        }
      if (!client.connected()) { //if not connection don't sendt data
        lcd.print("!"); //indicate data is NOT send by ! mark in buttom right coner
        }
      else { //If connection established send data to socket
        client.println("###");
        for(int i = 0; i < 4; i++) {
          client.println(mytemps[i]);
          }
        lcd.print("#"); //indicate data IS send by ! mark in buttom right coner
        }
      }
    delay(1000); //refresh rate
    }     
  }
