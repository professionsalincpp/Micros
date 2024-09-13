#define PING_TIMEOUT 100
#define MAIN_SERIAL Serial

#define SERIALTYPE HardwareSerial

#include <micros/os>

using namespace storage;

CONFIG_START

CONFIG_STORAGE (
     float : 50%;
     int : 20b;
     char : 10
);

CONFIG_CLASTER (
     float : 20b;
     int : 2;
     string : 2
);

CONFIG_END

storage::OMainStorage mainStorage;
OStream<SERIALTYPE> stream(&Serial);
Emulator emulator(0x0, FULLACCESS);
