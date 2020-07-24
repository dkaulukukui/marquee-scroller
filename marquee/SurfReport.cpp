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

SurfReport::SurfReport()
{
 //empty constructor
}

void SurfReport::updateSurf_RSS() {

  const char* host = "https://www.weather.gov/source/hfo/xml/Surf.xml";
  const char* fingerprint = "07e32864918f4238a0542a2ccdc17c98798ca1d8";
  const int httpsPort = 443;

  HTTPClient https;

  Serial.println("updating RSS surf");
  Serial.print("[HTTPS] begin...\n");
   
  if (https.begin(host, fingerprint)) { 

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
        RSSsurf.date = XMLgetValue(https, "<lastBuildDate", 1);
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
  text.replace("\n", " ");
  text.replace("NOTE: Please check with local authorities regarding beach closures.", "");
  text.replace("Surf heights are forecast heights of the face, or front, of waves. The surf forecast is based on the significant wave height, the average height of the one third largest waves, at the locations of the largest breakers. Some waves may be more than twice as high as the significant wave height. Expect to encounter rip currents in or near any surf zone.", "");
  text.replace("Surf along ", "");
  text.replace("facing shores ", "");
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

String SurfReport::RSS_get_date()
{
    return this->RSSsurf.date;
}