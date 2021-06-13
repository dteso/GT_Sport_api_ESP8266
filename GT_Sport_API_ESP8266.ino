#include <ArduinoJson.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <Wire.h>

#include "WiFiClientSecureBearSSL.h"
#include "clock.h"
#include "config.h"
#include "globals.h"
#include "gt_api_params.h"
#include "displayParams.h"

#include "ESP8266_Utils.hpp"
#include "displayService.hpp"
#include "apiGTService.hpp"

void loadData() {
  endpoint = "/profile/?user_no=" + (String)user_no + "&job=1";
  getProfile(server, endpoint);
  delay(300);
  endpoint = "/profile/?user_no=" + (String)user_no + "&job=3";
  getStats(server, endpoint);
  delay(300);
  endpoint = "/event/?job=3&channel_id_csv=";
  getDailyRaces(server, endpoint);
  delay(300);
  endpoint = "/event/?job=1&event_id_csv=";
  getRacesInfo(server, endpoint);
  delay(300);
}



void setup() {
  lastScore = 0;
  Serial.begin(115200);
  webServer.begin();

  pinMode(D4, OUTPUT); //Green LED
  digitalWrite(D4, LOW);

  pinMode(D5, OUTPUT);
  digitalWrite(D5, LOW); //Red LED

  pinMode(D6, OUTPUT); //Buzzer

  loadingScreen();
  ConnectWiFi_STA();
  display.clearDisplay();
  display.drawBitmap(0, 0, medusaka_logoBitmap, 128, 32, WHITE);
  display.display();
  loadData();
}

void loop() {
  String strHour;
  String strMin;
  clk();
  ntp();
  readSerial();
  renderWebServer();
  if (instantChange == true) {
    switch (sc) {
      case 1:
        display.clearDisplay();
        displayPrint(2, 0, 12, _PROFILE);
        break;
      case 5:
        if (lastScore < _DRIVER_POINTS) {
          pointsDiff = " + " + (String)(_DRIVER_POINTS - lastScore);
          digitalWrite(D4, HIGH);
          digitalWrite(D5, LOW);
          tone(D6, 2000);
          delay(50);
          tone(D6, 0);
          delay(100);
          tone(D6, 2000);
          delay(50);
          tone(D6, 0);
          delay(50);
          tone(D6, 2000);
          delay(50);
          tone(D6, 0);
          lastScore = _DRIVER_POINTS;
        } else if (lastScore > _DRIVER_POINTS) {
          pointsDiff = " - " + (String)(lastScore - _DRIVER_POINTS);
          digitalWrite(D4, LOW);
          digitalWrite(D5, HIGH);
          tone(D6, 700);
          delay(50);
          tone(D6, 0);
          delay(100);
          tone(D6, 700);
          delay(50);
          tone(D6, 0);
          delay(50);
          tone(D6, 700);
          delay(50);
          tone(D6, 0);
          lastScore = _DRIVER_POINTS;
        }
        display.setTextSize(2);
        testscrolltext(pointsDiff + " p" );
        display.display();
        break;
        break;
      case 10:
        display.setTextSize(2);
        testscrolltext("  " + (String)_DRIVER_POINTS);
        displayPrint(1, 0, 0, "-DRIVER POINTS-");
        break;
      case 15:
        display.setTextSize(3);
        testscrolltext("  " + (String)_DRIVER_CLASS);
        displayPrint(1, 0, 0, "-DRIVER CLASS-");
        break;
      case 20:
        display.clearDisplay();
        display.startscrollleft(0x00, 0x0F);
        display.drawBitmap(0, 0, next_race_A, 128, 32, WHITE);
        display.display();
        break;
      case 25:
      case 26:
      case 27:
      case 28:
      case 29:
        display.stopscroll();
        display.clearDisplay();
        displayPrint(1, 90, 0, _RACES_TIMES[0]);
        displayPrint(2, 5, 15, _RACES_TYPES[0] + "-" + timeLeft(realHour, realMinute, realSec, _RACES_TIMES[0].substring(0,2).toInt(), _RACES_TIMES[0].substring(3,5).toInt()));
        break;
      case 30:
        display.clearDisplay();
        display.startscrollleft(0x00, 0x0F);
        display.drawBitmap(0, 0, next_race_B, 128, 32, WHITE);
        display.display();
        break;
      case 35:
      case 36:
      case 37:
      case 38:
      case 39:
        display.stopscroll();
        display.clearDisplay();
        displayPrint(1, 90, 0, _RACES_TIMES[1]);
        displayPrint(2, 5, 15, _RACES_TYPES[1] + "-" + timeLeft(realHour, realMinute, realSec, _RACES_TIMES[1].substring(0,2).toInt(), _RACES_TIMES[1].substring(3,5).toInt()));
        break;
      case 40:
        display.clearDisplay();
        display.startscrollleft(0x00, 0x0F);
        display.drawBitmap(0, 0, next_race_C, 128, 32, WHITE);
        display.display();
        break;
      case 45:
      case 46:
      case 47:
      case 48:
      case 49:
        display.stopscroll();
        display.clearDisplay();
        displayPrint(1, 90, 0,_RACES_TIMES[2]);
        displayPrint(2, 5, 15, _RACES_TYPES[2] + "-" + timeLeft(realHour, realMinute, realSec, _RACES_TIMES[2].substring(0,2).toInt(), _RACES_TIMES[2].substring(3,5).toInt()));
        break;
      case 50:
        display.stopscroll();
        display.clearDisplay();
        strHour=(String)realHour;
        if (realMinute < 10) {
          strMin = "0" + (String)realMinute;
        }else{
          strMin = (String)realMinute;
        }
        displayPrint(3, 15, 10, strHour + ":" + strMin);
        break;
      case 55:
        display.clearDisplay();
        display.startscrollleft(0x00, 0x0F);
        display.drawBitmap(0, 0, gt_logoBitmap, 128, 32, WHITE);
        display.display();
        break;
      case 59:
        digitalWrite(D4, LOW);
        digitalWrite(D5, LOW);
        loadData();
        break;
      default:
        break;
    }
  }
}

void readSerial(){
  String reading;
  while (Serial.available()) //Check if there is an available byte to read
  {
      delay(10); //Delay added to make thing stable
      char d = Serial.read(); //Conduct a serial read
      if (d == '#') {break;}
      reading += d; //Shorthand for reading = reading + c
  }
                              
  if (reading.length() > 0) 
  {
      Serial.println("READ FROM APP>>");
      Serial.println(reading);
      reading="";
  }
}
