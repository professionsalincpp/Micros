#include <Arduino.h>
#include "config.h"

using namespace storage;

void printProgressBar(int percentage, int length=100, char symbol='#', char empty=' ', char end='\n', bool showPercentage=true) {
    int progress = percentage * length / 100;
    stream.print("[");
    for (int i = 0; i < length; i++) {
        if (i < progress) {
            stream.print(symbol);
        } else {
            stream.print(empty);
        }
    }
    stream.print("] ");
    if (showPercentage) {
        stream.print(" ");
        stream.print(percentage);
        stream.print("%");
    }
    stream.print(end);
}

void info() {
    stream.print("Program Started\n");
    stream.print("Main storage float size: "); stream.print(mainStorage.floatStorage.getRealSize()); stream.print("\n");
    stream.print("Main storage int size: "); stream.print(mainStorage.intStorage.getRealSize()); stream.print("\n");
    stream.print("Main storage char size: "); stream.print(mainStorage.charStorage.getRealSize()); stream.print("\n");
    stream.print("Busy space: "); stream.print(storage::getUsedSpace()); stream.print("\n");
    stream.print("Available space: "); stream.print(storage::getFreeSpace()); stream.print("\n");
    printProgressBar((storage::getUsedSpace() * 100) / storage::getTotalSpace(), 20, '#', ' ', '\n', true);
}

int getArgAsInt(OPackage &package, int argIndex) {
    const char* arg = package.getArg(argIndex);
    if (arg == nullptr) {
        return -1;
    }
    return atoi(arg);
}

ECommonTaskProcessError storageSetHook(OStream<SERIALTYPE>& stream, OPackage& package) {
    int offset = getArgAsInt(package, 0);
    int value = getArgAsInt(package, 1);
    if (offset == -1 || value == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    mainStorage.intStorage[offset] = value;
    return ECommonTaskProcessError::ErrorNone;
}

ECommonTaskProcessError storageGetHook(OStream<SERIALTYPE>& stream, OPackage& package) {
    int offset = getArgAsInt(package, 0);
    if (offset == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    stream.print(mainStorage.intStorage[offset]);
    return ECommonTaskProcessError::ErrorNone;
}

void initialize() {
    Serial.begin(9600);
    Serial.println("Initialized\n");
    pinMode(LED_BUILTIN, OUTPUT);
    emulator.addHook(storageSetHook);
    info();
}

void update() {
    // put your update code here
}