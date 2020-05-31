/** The MIT License (MIT)

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
#ifndef SURFREPORT_INCLUDED
#define SURFREPORT_INCLUDED

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <JsonListener.h>
#include <JsonStreamingParser.h> // --> https://github.com/squix78/json-streaming-parser
#include <TimeLib.h> // https://github.com/PaulStoffregen/Time 

extern const char * months[];
extern const char * day_of_week[];

class SurfReport: public JsonListener {
  
  private:
    String myApiKey;
    String mySpot;

    String currentKey = "";
    String last_LVL_Key ="";
    int counterReport = 0;
    int nest_level = 0;
    int dayReport = 0;

    typedef struct {
      String report_timestamp;
      String wave_ht;
      String wave_dir_deg;
      String wave_dir_compass;
      String wave_period;
      String wind_sp;
      String wind_dir_deg;
      String wind_dir_compass;
    } surffeed;

    surffeed surf[5]; //store one report per day

 public:
    SurfReport(String apiKey, String spot);
    void updateSurfSource(String apiKey, String spot);
    void updateSurf();
    String getSpot();
    String format_report(int index);

    String cleanText(String text);
    
    virtual void whitespace(char c);
    virtual void startDocument();
    virtual void key(String key);
    virtual void value(String value);
    virtual void endArray();
    virtual void endObject();
    virtual void endDocument();
    virtual void startArray();
    virtual void startObject();
};

#endif