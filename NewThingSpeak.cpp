// Copy of library written by daried https://github.com/dareid/sparkcore.thingspeak
#include "newThingSpeak.h"

newThingSpeakLibrary::newThingSpeak::newThingSpeak(String apiKey) 
{
    key = apiKey;
    timeout= DEFAULT_RESPONSE_WAIT_TIME;
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
    values[fieldId-1] = fieldValue;
    return true;
}

String newThingSpeakLibrary::newThingSpeak::composeQuery()
{
    String result = String ("/update?key=" + key);
    for(int ct = 0; ct < NUMBER_OF_FIELDS; ct++) 
    {
        if(values[ct] != NULL) 
        {
            String fieldParameter = String("&field" + String((ct+1), DEC) + "=" + values[ct]);
            result.concat(fieldParameter);
            values[ct] = NULL;
        }
    }
    return result;
}

bool newThingSpeakLibrary::newThingSpeak::sendValues() 
{
    if (client.connect(THINGSPEAK_API, 80))
    {
        bool ret = false;
        client.println(String("GET " + this->composeQuery() + " HTTP/1.0"));
        client.println(String("Host: " + String(THINGSPEAK_API)));
        client.println("Content-Length: 0");
        client.println();
		
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
        
        return ret;
    }
    else
    {
        return false;
    }
}