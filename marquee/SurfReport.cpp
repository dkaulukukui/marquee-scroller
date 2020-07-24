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

#define arr_len( x )  ( sizeof( x ) / sizeof( *x ) )

SurfReport::SurfReport(String apiKey, String spot)
{
  myApiKey = apiKey;
  mySpot = spot;
}

void SurfReport::updateSurfSource(String apiKey, String spot)
{
  myApiKey = apiKey;
  mySpot = spot;
}

void SurfReport::updateSurf() {
  JsonStreamingParser parser;
  parser.setListener(this);
  HTTPClient http;

  //Replace with string for surf data to parse
  //String apiGetData = "http://" + String(servername) + "/v2/top-headlines?sources=" + mySource + "&apiKey=" + myApiKey;
  //String apiGetData = "https://www.surfline.com/surf-report/ala-moana-bowls/5842041f4e65fad6a7708b42";
  String SURF_SOURCE = "http://magicseaweed.com/api/" + myApiKey + "/forecast/?spot_id=" + mySpot;//

  Serial.println("Getting Surf Data");
  Serial.println(SURF_SOURCE);
  http.begin(SURF_SOURCE);
  int httpCode = http.GET();

  if (httpCode > 0) {  // checks for connection
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    if(httpCode == HTTP_CODE_OK) {
      // get length of document (is -1 when Server sends no Content-Length header)
      int len = http.getSize();
      // create buffer for read
      char buff[128] = { 0 };
      // get tcp stream
      WiFiClient * stream = http.getStreamPtr();
      // read all data from server
      Serial.println("Start parsing...");
      while(http.connected() && (len > 0 || len == -1)) {
        // get available data size
        size_t size = stream->available();
        //Serial.print("Size of feed=");
        //Serial.println(size);
        if(size) {
          // read up to 128 byte
          int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
          for(int i=0;i<c;i++) {
            parser.parse(buff[i]); 
          }
            
          if(len > 0)
            len -= c;
          }
        delay(1);
      }
    }
    http.end();
  } else {
    Serial.println("connection for surf data failed: " + String(SURF_SOURCE)); //error message if no client connect
    Serial.println();
    return;
  }
}

void SurfReport::updateSurf_RSS() {

  const char* host = "https://www.weather.gov/source/hfo/xml/Surf.xml";
  const char* fingerprint = "07e32864918f4238a0542a2ccdc17c98798ca1d8";
  const int httpsPort = 443;

  //WiFiClientSecure client;

  //client.setInsecure(); //added due to https connection refused errors, we dont care about it being secure

  HTTPClient https;

  Serial.println("updating RSS surf");
  Serial.print("[HTTPS] begin...\n");
   
  if (https.begin(host, fingerprint)) {  // HTTPS original
  //if (https.begin(host)) {  // HTTPS  modified to solve connection refused errors 24JUL2020 0922

    Serial.print("[HTTPS] GET...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        
        RSSsurf.title = cleanText(XMLgetValue(https, "<description", 1));
        RSSsurf.warnings = cleanText(XMLgetValue(https, "<description", 1));
        RSSsurf.forecast = cleanText(XMLgetValue(https, "<description", 1));
        RSSsurf.extended = cleanText(XMLgetValue(https, "<description", 1));

        //Serial.println();
        Serial.print("[HTTPS] connection closed or file end.\n");
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
  }
}

String SurfReport::getSpot()
{
    return mySpot;
}

void SurfReport::whitespace(char c) {
   //Serial.println("whitespace");
}

void SurfReport::startDocument() {
  //Serial.println("start document");
  counterReport = 0;
  nest_level = 0;
  dayReport = 0;
}

void SurfReport::key(String key) {
  //Serial.println("key: " + key);
  currentKey = key;
}

void SurfReport::value(String value) {
  //Serial.println("value: " + value);
  if (dayReport == 5) {
    // we are full so return
    return;
  }
    if (nest_level == 1 && currentKey == "timestamp") {
      //only grab one report for each day
      if(counterReport == 0 || counterReport == 8 || counterReport == 16 || counterReport == 24 || counterReport == 32){
        surf[dayReport].report_timestamp = value;
      }
    }
    
    if (last_LVL_Key == "combined" && nest_level == 4) //nest level 4 is the combined swell data
    {
     if(currentKey == "height") {
        if(counterReport == 0 || counterReport == 8 || counterReport == 16 || counterReport == 24 || counterReport == 32){
          surf[dayReport].wave_ht = value;
        }
     }
      else if (currentKey == "period") {
        if(counterReport == 0 || counterReport == 8 || counterReport == 16 || counterReport == 24 || counterReport == 32){
          surf[dayReport].wave_period = value;
        }
     }
      else if (currentKey == "direction") {
        if(counterReport == 0 || counterReport == 8 || counterReport == 16 || counterReport == 24 || counterReport == 32){
          surf[dayReport].wave_dir_deg = value;
        }
      }
      else if (currentKey == "compassDirection") {
        if(counterReport == 0 || counterReport == 8 || counterReport == 16 || counterReport == 24 || counterReport == 32){
          surf[dayReport].wave_dir_compass = value;
          Serial.println(format_report(dayReport));
          dayReport++;          
        }
        counterReport++;
      }
    }  
  //Serial.println(currentKey + "=" + value);
}

void SurfReport::endArray() {
  //Serial.println("end array. ");
}

void SurfReport::endObject() {
  //Serial.println("end object. ");
  nest_level--;
}
void SurfReport::startArray() {
 // Serial.println("start array. ");
}

void SurfReport::startObject() {
  //Serial.println("start object. ");
  last_LVL_Key = currentKey; //save last level key for nested filtering
  nest_level++;
}

void SurfReport::endDocument() {
  //Serial.println("end document. ");
}

String SurfReport::format_report(int index, boolean add_date){
  String report_string = "";
  int upper_surf_height = surf[index].wave_ht.toFloat() + 2;
  String surf_range = surf[index].wave_ht + "-" + String(upper_surf_height) +"ft ";

    if(add_date == true){
    String report_date = "";
    //format time into readable string format
    time_t time_raw = surf[index].report_timestamp.toInt();//convert to time_t object
    report_date = day_of_week[dayOfWeek(time_raw)];// + String(day(time_raw))+months[month(time_raw)]+String(year(time_raw));
    report_string = report_date +":"+ surf_range +surf[index].wave_dir_compass;
    } else{
      report_string = surf_range + surf[index].wave_dir_compass;
    };
  return report_string;
}

String SurfReport::cleanText(String text) {
  text.replace("’", "'");
  text.replace("“", "\"");
  text.replace("”", "\"");
  text.replace("`", "'");
  text.replace("‘", "'");
  text.replace("„", "'");
  text.replace("\\\"", "'");
  text.replace("•", "-");
  text.replace("é", "e");
  text.replace("è", "e");
  text.replace("ë", "e");
  text.replace("ê", "e");
  text.replace("à", "a");
  text.replace("â", "a");
  text.replace("ù", "u");
  text.replace("ç", "c");
  text.replace("î", "i");
  text.replace("ï", "i");
  text.replace("ô", "o");
  text.replace("…", "...");
  text.replace("–", "-");
  text.replace("Â", "A");
  text.replace("À", "A");
  text.replace("æ", "ae");
  text.replace("Æ", "AE");
  text.replace("É", "E");
  text.replace("È", "E");
  text.replace("Ë", "E");
  text.replace("Ô", "O");
  text.replace("Ö", "Oe");
  text.replace("ö", "oe");
  text.replace("œ", "oe");
  text.replace("Œ", "OE");
  text.replace("Ù", "U");
  text.replace("Û", "U");
  text.replace("Ü", "Ue");
  text.replace("ü", "ue");
  text.replace("Ä", "Ae");
  text.replace("ä", "ae");
  text.replace("ß", "ss");
  text.replace("»", "'");
  text.replace("«", "'");
  text.replace("&#44;", ",");
  text.replace("\n", "");
  return text;
}

String SurfReport::XMLgetValue(HTTPClient &http, String key, int skip /*, int get*/) {
  bool found = false, look = false;
  int ind = 0;   //character counter for the key
  String ret_str = "";

  int len = http.getSize();
  char char_buff[1];
  WiFiClient * stream = http.getStreamPtr();
  while (http.connected() && (len > 0 || len == -1)) {
    size_t size = stream->available();
    //Serial.print(char_buff[0]); //debug to print chars as they come in
    if (size) {
      int c = stream->readBytes(char_buff, ((size > sizeof(char_buff)) ? sizeof(char_buff) : size));
      if (len > 0)
        len -= c;
      if (found) {
        if (char_buff[0] == 60 /*get <= 0*/)
          break;
        if (skip == 0) {
          ret_str += char_buff[0];
         // get --;
        } else
          skip --;
      }
      else if ((!look) && (char_buff[0] == key[0])) {  //Search for first character in key
        look = true;  //Set look bool to indicate currently comparing key
        ind = 1;  //set key search index to 1
      } else if (look && (char_buff[0] == key[ind])) {  //if looking and current char matches key 
        ind ++; //increment key search index
        if (ind == key.length()) {  //successfully matched the key
          found = true;  //only set found if at correct instance
        }
      } else if (look && (char_buff[0] != key[ind])) { //if looking and current char doesnt match key then reset
        ind = 0;
        look = false;
      }
    }
  }
  return ret_str;
}

String SurfReport::RSS_get_title()
{
    return this->RSSsurf.title;
}

String SurfReport::RSS_get_warnings()
{
    return this->RSSsurf.warnings;
}

String SurfReport::RSS_get_forecast()
{
    return this->RSSsurf.forecast;
}

String SurfReport::RSS_get_extended()
{
    return this->RSSsurf.extended;
}