
// required libraries 
#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <DHT.h> 

char ssid[] = SECRET_SSID;   // network SSID (name) 
char pass[] = SECRET_PASS;   // network password
int keyIndex = 0;            // network key Index number 
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#define DHTPIN 2 // Define ditgital pin for DHT sensor
#define DHTTYPE DHT11 //Define type of DHT sensor (DHT11)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);// Initialize serial 
  while (!Serial) {
    ; // wait for serial port to connect. 
  }
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
  dht.begin();
}

void loop() {

  // Connect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

   if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor! ");
    delay(2000);
    return;
  }

// data which is being measured by the DHT sensor and plotted in ThingSpeak
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful. ");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  
  delay(20000); // update chanel every 20 seconds
}