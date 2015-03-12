// Copy of library written by daried https://github.com/dareid/sparkcore.thingspeak
#include "newThingSpeak.h"

newThingSpeakLibrary::newThingSpeak::newThingSpeak(String apiKey) 
{
    key = apiKey;
    timeout= DEFAULT_RESPONSE_WAIT_TIME;
    query.reserve(80);
}

void newThingSpeakLibrary::newThingSpeak::setConnectionTimeout(uint32_t milliseconds) 
{
    this->timeout = milliseconds;   
}

bool newThingSpeakLibrary::newThingSpeak::recordValue(int fieldId, String fieldValue)
{
    if(fieldId < 1 || fieldId > NUMBER_OF_FIELDS) {
        return false;
    }
    values[fieldId-1] = "";
    values[fieldId-1].concat(fieldValue);
    return true;
}

void newThingSpeakLibrary::newThingSpeak::composeQuery()
{
    query = "/update?key=";
    query.concat(key);
    for(int ct = 0; ct < NUMBER_OF_FIELDS; ct++) 
    {
        if(values[ct] != "") 
        {
            query.concat("&field");
            query.concat(ct+1);
            query.concat("=");
            query.concat(values[ct]);
            values[ct] = "";
        }
    }
}

bool newThingSpeakLibrary::newThingSpeak::sendValues() 
{
    if (client.connect(THINGSPEAK_API, 80))
    {
        bool ret = false;
        this->composeQuery();
        char buf[300];  //pick a size here
        strcat(buf,"GET ");
        strcat(buf, query.c_str());
        strcat(buf, " HTTP/1.0\r\n");
        strcat(buf, "Host: ");
        strcat(buf, THINGSPEAK_API);
        strcat(buf, "\r\nContent-Length: 0\r\n\r\n");
        client.write((uint8_t*)buf,strlen(buf));

	unsigned long lastTime = millis();
        while( client.available() == 0 && millis()-lastTime < this->timeout ) { 
        }  
        lastTime = millis();
        while ( client.available() || (millis()-lastTime < this->timeout) ) {
            if (client.available()) {
                ret = true;
                client.read();
                lastTime = millis();
            }
        }
        client.stop();
        return ret;
    }
    else
    {
        client.stop();
        return false;
    }
}
