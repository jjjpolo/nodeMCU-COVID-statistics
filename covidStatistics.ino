#include <Arduino.h>

//*********Requested libraries for ESP or NodeMCU
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
char* SSID = "YOUR SSID";
char* password = "YOUR PASSWORD";

//************For https get request
#include <WiFiClientSecureBearSSL.h>
// Fingerprint for COVID https rest API:
const uint8_t fingerprint[20] = {0x7b, 0x00, 0x29, 0x73, 0x45, 0x13, 0xa3, 0xf7, 0x95, 0x84, 0x42, 0xea, 0xe0, 0x28, 0x1b, 0xf4, 0x0d, 0xb8, 0x11, 0x53};
String covidAPI_url = "https://corona.lmao.ninja/v2/countries/";

//***************TO let the ESP knows many Wifi credentials
ESP8266WiFiMulti WiFiMulti;


//**************For oled
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Code of the image displayed on OLED:
#include "customLogo.h";


//**************For Json manage and parsing
#include <ArduinoJson.h>

//*****************************************************SETUP
void setup()
{
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.drawBitmap(0, 0, logo_bmp, 128, 64, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(64, 0);            // Start at top-left corner
  display.print("Coneccting");
  display.setCursor(0, 54);            // Start at top-left corner
  display.print("v1.0.0 beta");
  display.display();

  Serial.print("\n\n\n");

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(SSID, password); // You can use this line as many wifi credentials as you need, of course, each line must have different the credentials you want to add
}


//*****************************************************User functions
void printText(String text)
{
  Serial.print(text);
  display.clearDisplay();
  display.setTextSize(0.7);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(0, 0);            // Start at top-left corner
  display.print(text);
  display.display();
}

String getData(String json, String title, String *param, int limit) // Title is a strin to be returned as title of the response Limit = length of param
{
  StaticJsonDocument<700> doc;
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return "error";
  }
  String response = title;
  for (int i = 0; i < limit; i++)
  {
    response += *param + ":";
    long temp = doc[*param];
    response += String(temp);
    response += "\n";
    param++;
  }
  return response;
}

void getCovidStatistics(String country, String param[])
{
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED))
  {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    client->setFingerprint(fingerprint);
    HTTPClient https;
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(*client, covidAPI_url + country)) // HTTPS get request
    {
      Serial.print("[HTTPS] GET...\n");
      int httpCode = https.GET(); // start connection and send HTTP header

      if (httpCode > 0)  // httpCode will be negative on error
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) // file found at server
        {
          String payload = https.getString();
          Serial.println(payload);
          String mainData = getData(payload, "COVID-19 " + country + "'S \nCURRENT STATISTICS: \n", &param[0], 5);
          printText(mainData);
        }
      }
      else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    }
    else  {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  Serial.println("Waiting  before next round...");
}


//*****************************************************MAIN
void loop()
{
  String country[] = {"MEXICO", "CHINA", "USA", "ITALY", "ECUADOR", "SPAIN", "GERMANY", "FRANCE", "CANADA"}; // Add here all the countries you want to know it statistics
  String param[] = {"cases", "deaths", "recovered", "critical", "active"}; // Using postman you can know all the available parameters
  for (int i = 0 ; i < 9; i++)
  {
    getCovidStatistics(country[i], param);
    delay(5000);
  }
  display.clearDisplay();
  display.drawBitmap(0, 0, logo_bmp, 128, 64, WHITE); // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  display.display();
  delay(2000);
}