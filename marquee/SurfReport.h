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

class SurfReport {  
  
private:

    typedef struct {
      String title;
      String warnings;
      String forecast;
      String extended;
      String date;
    } RSSsurffeed;

    RSSsurffeed RSSsurf;

    String XMLgetValue(HTTPClient &http, String key, int skip, char end_of_value);

public:
    SurfReport(); //empty constructor

    void updateSurf_RSS();

    String RSS_get_title();
    String RSS_get_warnings();
    String RSS_get_forecast();
    String RSS_get_extended();
    String RSS_get_date(); 

    String cleanText(String text);
};

#endif