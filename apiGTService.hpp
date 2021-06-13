String raceTypes[3];
int eventsIds[3];
String eventsTimes [3];

void getProfile(String host, String endpoint) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  if (https.begin(*client, host + endpoint)) {  // HTTPS
    Serial.println("[HTTPS] POST...");
    int httpCode = https.POST(endpoint);
    if (httpCode > 0) {
      Serial.printf("[HTTPS] POST PROFILE... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        Serial.println(String("[HTTPS] Received payload: ") + payload);
        StaticJsonDocument<300> doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          return;
        }

        String profile = doc["profile"]["id"];
        Serial.println("Profile: " + profile);
        _PROFILE = profile;
      }
    } else {
      Serial.printf("[HTTPS] POST... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n\r");
  }

}



void getStats(String host, String endpoint) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  if (https.begin(*client, host + endpoint)) {  // HTTPS
    Serial.println("[HTTPS] POST...");
    int httpCode = https.POST(endpoint);
    if (httpCode > 0) {
      Serial.printf("[HTTPS] POST STATS... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = https.getString();
        Serial.println(String("[HTTPS] Received payload: ") + payload);
        DynamicJsonDocument doc(16536);
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          return;
        }
        int driverPoints = doc["stats"]["driver_point"];
        int driverClass = doc["stats"]["driver_class"];
        Serial.println("Driver Points: " + (String)driverPoints);
        Serial.println("Class: " + (String)driverClass);
        _DRIVER_POINTS = driverPoints;
        switch (driverClass) {
          case 1:
            _DRIVER_CLASS = "E";
            break;
          case 2:
            _DRIVER_CLASS = "D";
            break;
          case 3:
            _DRIVER_CLASS = "C";
            break;
          case 4:
            _DRIVER_CLASS = "B";
            break;
          case 5:
            _DRIVER_CLASS = "A";
            break;
          case 6:
            _DRIVER_CLASS = "S";
            break;
          case 7:
            _DRIVER_CLASS = "S+";
            break;
          default:
            break;
        }
      }
    } else {
      Serial.printf("[HTTPS] POST... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n\r");
  }

}


void getRacesInfo(String host, String endpoint) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;
  for (int i = 0; i < 3; i++) {
    if (https.begin(*client, host + endpoint + eventsIds[i] )) {  // HTTPS
      Serial.println("[HTTPS] POST...");
      int httpCode = https.POST(endpoint);
      if (httpCode > 0) {
        Serial.println("[HTTPS] POST...:" + endpoint + eventsIds[i] + "\n");
        // file found at server?
        if (httpCode == HTTP_CODE_OK) {
          String payload = https.getString();
          Serial.println(String("[HTTPS] Received payload: ") + payload);
          DynamicJsonDocument doc(16384);
          DeserializationError error = deserializeJson(doc, payload, DeserializationOption::NestingLimit(20));
          delay(50);
          if (error) {
            Serial.println("Deserialization ERROR "+ (String)error.c_str());
            return;
          }
          String raceType = doc["event"][0]["value"][0]["GameParameter"]["events"][0]["regulation"]["car_category_types"][0];
          raceTypes[i] =  raceType;
          delay(100);
          Serial.println("Race Type: " + raceTypes [i]);
        }
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n\r");
    }

    delay(500);

  }
  _RACES_TYPES[0]=raceTypes[0];
  _RACES_TYPES[1]=raceTypes[1];
  _RACES_TYPES[2]=raceTypes[2];

}


void getDailyRaces(String host, String endpoint) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
  client->setInsecure();
  HTTPClient https;

  for (int channel_id_csv = 0; channel_id_csv < 3; channel_id_csv++) {
    if (https.begin(*client, host + endpoint  + String(channel_id_csv + 1) )) {  // HTTPS
      Serial.println("[HTTPS] POST... " + host + endpoint  + String(channel_id_csv + 1));
      int httpCode = https.POST(endpoint);
      if (httpCode > 0) {
        Serial.printf("[HTTPS] POST PROFILE... code: %d\n", httpCode);
        // file found at server?
        if (httpCode == HTTP_CODE_OK) {
          String payload = https.getString();
          Serial.println(String("[HTTPS] Received payload: ") + payload);
          DynamicJsonDocument doc(16384);
          DeserializationError error = deserializeJson(doc, payload);
          if (error) {
            return;
          }
          String eventId = doc["event_calendar"][0]["event_id"];
          String eventTime = doc["event_calendar"][0]["start_time"];
          String aux = eventTime.substring(11, 16);
          String aux_min, aux_hor;
          aux_hor = aux.substring(0, 2);
          aux_min = aux.substring(3, 5);
          int auxInt = aux_hor.toInt() + _GMT;
          eventTime = (String)auxInt + ':' + aux_min;
          Serial.println("Event ID of " + (String)(channel_id_csv + 1) + " is " + eventId);
          Serial.println("Event of " + (String)(channel_id_csv + 1) + " starts at " + eventTime);
          Serial.println("\r\n\r\n");
          eventsIds[channel_id_csv] = eventId.toInt();
          eventsTimes [channel_id_csv] = eventTime;
        }
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n\r", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n\r");
    }
    delay(1000);
  }
  _RACES_TIMES[0]=eventsTimes[0];
  _RACES_TIMES[1]=eventsTimes[1];
  _RACES_TIMES[2]=eventsTimes[2];
}
