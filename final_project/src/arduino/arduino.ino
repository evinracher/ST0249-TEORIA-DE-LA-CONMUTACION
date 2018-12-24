#include "DHT.h"
#define DHTPIN A1
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>

char ssid[] = "IoT-B19";
char pass[] = "meca2017*";

//pins definition
#define door 7

int status = WL_IDLE_STATUS;

WiFiClient client;

// ThingSpeak Settings
char server[] = "api.thingspeak.com";
String writeAPIKey = "R06YPN36SULKDX11";
String readAPIKey = "2QMHMLMMNR5XQ6E0";

DynamicJsonBuffer jsonBuffer;

unsigned long lastConnectionTime = 0; // track the last connection time
const unsigned long postingInterval = 3L * 1000L; // post data every 3 seconds

/*
   Indeces
   Inputs
   data[0] //fans (Don't implemented)
   data[1] //lights
   data[2] //motors (Don't implemented)
   Outputs
   data[3] //temperature
   data[4] //door_status
   data[6] //humid
*/
float data[6];

//Inputs
bool fans;
bool lights;
bool motors;

//Outputs
float temperature = 0.0;
float door_status = 0.0;
float humid = 0.0;

void readHumidAndTemperature() {
  humid = dht.readHumidity();

  // Read temperature as Fahrenheit (isFahrenheit = true)
  temperature = dht.readTemperature(true);
  
  data[3] = temperature;
  data[5] = humid;

  if (isnan(humid) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(humid);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" *F\t");
  Serial.println();
}



void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();
  while ( status != WL_CONNECTED) {
    Serial.println("Connecting");
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected");
}

void loop() {
  while ( status != WL_CONNECTED) {
    Serial.println("Connection lost. Reconecting...");
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

if (millis() - lastConnectionTime >= postingInterval) {
    fans = getData(0);
    delay(100);
    lights = getData(1);
    delay(100);
    motors = getData(2);
    processingInputs();
    processingOutputs();
    delay(3000);    
    sendData();
    lastConnectionTime = millis();
  }
  delay(1000);
}

void doorStatus() {
  if (digitalRead(door) == HIGH) {
    data[4] = 1;
    door_status = 1;
    Serial.println("Opening door");
  } else {
    data[4] = 0;
    door_status = 0;
    Serial.println("Closing door");
  }
}

void processingOutputs() {
  readHumidAndTemperature();
  doorStatus();
}

void processingInputs() {
  if (lights == 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}


/*
 * Send all data to the Thingspeak server for all outputs
 */
void sendData() {
  client.stop();
  String value4 = String(data[3], DEC).substring(0, 4);
  String value5 = String(data[4], DEC).substring(0, 4);
  String value6 = String(data[5], DEC).substring(0, 4);
  Serial.println(value4);
  Serial.println(value5);
  Serial.println(value6);

  String data = String("field4=" + value4
                       + "&field5=" + value5
                       + "&field6=" + value6);

  if (client.connect(server, 80)) {
    Serial.println("Sending");
    client.println("POST /update HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("X-THINGSPEAKAPIKEY: " + writeAPIKey);
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(data.length());
    client.print("\n\n");
    client.print(data);
    if (client.println() == 0) {
      Serial.println(F("Failed to send request"));
      return;
    }
  } else {
    Serial.println("Bad thingspeak sending conneciton");
  }
}


int getData(int field) {
  client.stop();
  ++field;
  int value;
  String channelData = "";

  if (client.connect(server, 80)) {
    Serial.println("getting");
    client.println("GET /channels/629324/fields/" + String(field) + "/last?api_key=2QMHMLMMNR5XQ6E0");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    if (client.println() == 0) {
      Serial.println(F("Failed to send request"));
      return 0;
    }
    while (client.available() == 0);  // wait till something comes in; you may want to add a timeout here

    unsigned long lastRead = millis();  // keeps track of last time a byte was read
    while (millis() - lastRead < 2000) {  // while 2 secs havent passed since a byte was read
      while (client.available()) {
        channelData +=  client.read();
        lastRead = millis();   // update last time something was read
      }
    }

    Serial.println(channelData);

    value = processingEntry(channelData);
    Serial.println("field" + String(field) + " = " + String(value));
    return value;
  } else {
    Serial.println("Bad thingspeak getting connection");
    return -1;
  }
}

/*
 * Only return zero or one. 
 * Zero (Data = 48, ascii) means that disable (turn off).
 * One means that the field is activated (turn on). 
 */
int processingEntry(String Data) {
  if ( Data == "48") {
    return 0;
  } else {
    return 1;
  }
}
