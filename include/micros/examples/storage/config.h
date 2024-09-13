#define PING_TIMEOUT 100
#define MAIN_SERIAL Serial

#define SERIALTYPE HardwareSerial

#include <micros/os>

using namespace storage;

CONFIG_START

#define LED_BUILTIN 13

CONFIG_STORAGE (
     float : 20%;
     int : 20b;
     char : 10
);

CONFIG_CLASTER (
     float : 1;
     int : 1;
     string : 1
);

CONFIG_END

storage::OMainStorage mainStorage;
OStream<SERIALTYPE> stream(&Serial);
Emulator emulator(0x0, FULLACCESS);
