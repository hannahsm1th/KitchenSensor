/* This program was created for my final project for SIT123
    Author: Hannah Smith
    Created: September, 21 2020
    Adapted from:
    Code for connecting to ThingsSpeak using the ESP-01 from
    https://create.arduino.cc/projecthub/neverofftheinternet
*/
/* ---------- Libraries ---------- */
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>

/* ---------- Constants ---------- */
#define DHTPIN 7 // temperature humidity sensor
#define PIRPIN 6 // motion sensor
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

float motionReadings = 0;
int i = 0;

long startTime = 0;
long waitTime = 0;
SoftwareSerial ESP8266(2, 3); // Rx,  Tx
boolean relay1_st = false;
boolean relay2_st = false;
unsigned char check_connection = 0;
unsigned char times_check = 0;
boolean error;


/* ---------- Variables ---------- */
float h, t; //the initial values read for the humidity and temperature
int state; //the state of the motion sensor, recorded as 1 for active and 0 for inactive


void setup() {

  Serial.begin(9600);
  ESP8266.begin(9600);

  startTime = millis();

  pinMode(13, OUTPUT);

  //LED flashes to indicate the program is starting
  for (int i = 0; i < 5; i++) {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
  }

  dht.begin(); //Setup for temperature sensing
  pinMode(PIRPIN, INPUT); //Setup for PIR

  ESP8266.println("AT+RST");
  delay(2000);
  Serial.println("Connecting to Wifi");
  while (check_connection == 0)
  {
    Serial.print(".");
    ESP8266.print("AT+CWJAP=\"Cactus\",\"cactusjuice\"\r\n");
    ESP8266.setTimeout(5000);
    if (ESP8266.find("WIFI CONNECTED\r\n") == 1)
    {
      Serial.println("WIFI CONNECTED");
      break;
    }
    times_check++;
    if (times_check > 3)
    {
      times_check = 0;
      Serial.println("Trying to Reconnect..");
    }
  }
}


void loop() {
  for (i = 0; i < 20; i++)
  {
    readMotionSensors();
    motionReadings += state;
    delay(1000);    
  }
  motionReadings /= 20;
  waitTime = millis() - startTime;
  if (waitTime > 20000) {
    readTempSensors();
    writeThingSpeak();
    startTime = millis();
  };
  i = 0;
}


//Functions

void readTempSensors(void)
{
  h = dht.readHumidity(); //Read the data and store it as floats
  t = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" Humidity: ");
  Serial.println(h);
}

void readMotionSensors(void)
{
  if (digitalRead(PIRPIN) == HIGH) {
    state = 1; //an active reading is recorded as 1
    Serial.println("Motion detected!");
  }
  else {
    state = 0; //inactive
    Serial.println("No motion detected.");
  }
}


void writeThingSpeak(void)
{
  startThingSpeakCmd();
  String getStr = "GET /update?api_key=";
  getStr += "RC1FB3JL6A8FGZSN"; //API key for the ThingSpeak project
  getStr += "&field1=";
  getStr += String(t);
  getStr += "&field2=";
  getStr += String(h);
  getStr += "&field3=";
  getStr += String(motionReadings);
  getStr += "\r\n\r\n";
  GetThingspeakcmd(getStr);
  Serial.println(getStr);
}

void startThingSpeakCmd(void)
{
  ESP8266.flush();
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += "184.106.153.149"; // api.thingspeak.com IP address
  cmd += "\",80";
  ESP8266.println(cmd);
  Serial.print("Start Commands: ");
  Serial.println(cmd);

  if (ESP8266.find("Error"))
  {
    Serial.println("AT+CIPSTART error");
    return;
  }
}

String GetThingspeakcmd(String getStr)
{
  String cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  ESP8266.println(cmd);
  Serial.println(cmd);

  if (ESP8266.find(">"))
  {
    ESP8266.print(getStr);
    Serial.println(getStr);
    delay(500);
    String messageBody = "";
    while (ESP8266.available())
    {
      String line = ESP8266.readStringUntil('\n');
      if (line.length() == 1)
      {
        messageBody = ESP8266.readStringUntil('\n');
      }
    }
    Serial.print("MessageBody received: ");
    Serial.println(messageBody);
    return messageBody;
  }
  else
  {
    ESP8266.println("AT+CIPCLOSE");
    Serial.println("AT+CIPCLOSE");
  }
}
