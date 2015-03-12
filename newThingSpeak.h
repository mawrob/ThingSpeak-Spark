// Copy of library written by daried https://github.com/dareid/sparkcore.thingspeak
#include "application.h"

static const int NUMBER_OF_FIELDS = 9;
static const char THINGSPEAK_API [] = "api.thingspeak.com";
static const uint32_t DEFAULT_RESPONSE_WAIT_TIME = 1500;


namespace newThingSpeakLibrary
{
    class newThingSpeak
    {
        private:
            TCPClient client;
            String key;
            String values [NUMBER_OF_FIELDS];
            String query;
            void composeQuery();
            uint32_t timeout;
        public:
            newThingSpeak(String apiKey);
            bool recordValue(int fieldId, String fieldValue);
            bool sendValues();
            void setConnectionTimeout(uint32_t milliseconds);

    };
}
