#ifndef __MEMUTILS_H
#define __MEMUTILS_H

extern int __bss_end;
extern void *__brkval;
extern int __heap_start;

#define CHIP_ATMEGA328P 1
#define CHIP_ATMEGA2560 2
#define CHIP_ATMEGA32U4 3
#define CHIP_ESP32 4
#define CHIP_ESP8266 5
#define CHIP_STM32F103 6
#define CHIP_STM32F401 7
#define CHIP_NRF52832 8
#define CHIP_NRF52840 9
#define CHIP_RP2040 10
#define UNKNOWN_CHIP 11


#if defined(__AVR_ATmega328P__)
#define CHIP CHIP_ATMEGA328P
#define RAM_SIZE 0x800
#elif defined(__AVR_ATmega2560__)
#define CHIP CHIP_ATMEGA2560
#define RAM_SIZE 0x2000
#elif defined(__AVR_ATmega32U4__)
#define CHIP CHIP_ATMEGA32U4
#define RAM_SIZE 0xA00
#elif defined(ESP32)
#define CHIP CHIP_ESP32
#define RAM_SIZE 0x80000
#elif defined(ESP8266)
#define CHIP CHIP_ESP8266
#define RAM_SIZE 0x8000
#elif defined(__STM32F1__) // CHIP_STM32F103
#define CHIP CHIP_STM32F103
#define RAM_SIZE 0x2000
#elif defined(__STM32F4__) // CHIP_STM32F401
#define CHIP CHIP_STM32F401
#define RAM_SIZE 0x10000
#elif defined(NRF52832) // CHIP_NRF52832
#define CHIP CHIP_NRF52832
#define RAM_SIZE 0x4000
#elif defined(NRF52840) // CHIP_NRF52840
#define CHIP CHIP_NRF52840
#define RAM_SIZE 0x8000
#elif defined(RP2040) // CHIP_RP2040
#define CHIP CHIP_RP2040
#define RAM_SIZE 0x20000
#else
#ifndef RAM_SIZE
#define CHIP 11
#warning "Unsupported chip, set RAM_SIZE value manually"
#define RAM_SIZE 0x800
#endif
#endif

enum EChip {
    ATMEGA328P = CHIP_ATMEGA328P,
    ATMEGA2560 = CHIP_ATMEGA2560,
    ATMEGA32U4 = CHIP_ATMEGA32U4,
    ESP32 = CHIP_ESP32,
    ESP8266 = CHIP_ESP8266,
    STM32F103 = CHIP_STM32F103,
    STM32F401 = CHIP_STM32F401,
    NRF52832 = CHIP_NRF52832,
    NRF52840 = CHIP_NRF52840,
    RP2040 = CHIP_RP2040,
    UNKNOWN = UNKNOWN_CHIP
};

EChip getChip() {
    return static_cast<EChip>(CHIP);
}

String getChipName() {
    switch (getChip()) {
        case ATMEGA328P:
            return "ATMEGA328P";
        case ATMEGA2560:
            return "ATMEGA2560";
        case ATMEGA32U4:
            return "ATMEGA32U4";
        case ESP32:
            return "ESP32";
        case ESP8266:
            return "ESP8266";
        case STM32F103:
            return "STM32F103";
        case STM32F401:
            return "STM32F401";
        case NRF52832:
            return "NRF52832";
        case NRF52840:
            return "NRF52840";
        case RP2040:
            return "RP2040";
        default:
            return "UNKNOWN";
    }
}

namespace memory {

    int getFreeRam() {
        int v;
        int stack;
        return (int) &stack - (__brkval == 0 ? (int) &__heap_start : (int) __brkval) - sizeof(v);
    }

    int getTotalRam() {
        return RAM_SIZE;
    }


    int getUsedRam() {
        return getTotalRam() - getFreeRam();
    }

} // namespace memory


#endif // __MEMUTILS_H