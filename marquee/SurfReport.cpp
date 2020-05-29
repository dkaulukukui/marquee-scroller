/* The MIT License (MIT)

Copyright (c) 2018 Donald Kaulukukui

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "SurfReport.h"

SurfReport::SurfReport(String apiKey)
{
  myApiKey = apiKey;
}

void SurfReport::updateConfig(String apiKey, String spot)
{
  myApiKey = apiKey;
  mySpot = spot;
}

String SurfReport::getSurfReport()
{
  WiFiClient client;
  //String apiGetData = "GET /v2.1/get-time-zone?key=" + myApiKey + "&format=json&by=position&lat=" + myLat + "&lng=" + myLon + " HTTP/1.1";
  String apiGetData = "replace with URL for API used";
  Serial.println("Getting Surf Report for " + mySpot);
  Serial.println(apiGetData);
  String result = "";
  
  /*
  if (client.connect(servername, 80)) {  //starts client connection, checks for connection
    client.println(apiGetData);
    client.println("Host: " + String(servername));
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    Serial.println("connection for Surf Report data failed"); //error message if no client connect
    Serial.println();
    return 20;
  }

  while (client.connected() && !client.available()) delay(1); //waits for data
  */
  Serial.println("Waiting for data");

  /*boolean record = false;
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    if (String(c) == "{") {
      record = true;
    }
    if (record) {
      result = result + c;
    }
    if (String(c) == "}") {
      record = false;
    }
  }
  client.stop(); //stop client
  Serial.println(result);

  char jsonArray [result.length() + 1];
  result.toCharArray(jsonArray, sizeof(jsonArray));
  jsonArray[result.length() + 1] = '\0';
  DynamicJsonBuffer json_buf;
  JsonObject& root = json_buf.parseObject(jsonArray);

  */
  localMillisAtUpdate = millis();
  Serial.println();
  /*if (root["timestamp"] == 0) {
    return 20;
  } else {
    return (unsigned long) root["timestamp"];
  }
  */
 return "Surf Report String"
}

String SurfReport::getSpot()
{
    return mySpot
}
