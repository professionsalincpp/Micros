#include <Arduino.h>
#include "config.h"

using namespace storage;
using namespace claster;

void printProgressBar(long percentage, int length=100, char symbol='#', char empty=' ', char end='\n', bool showPercentage=true);
void info();

void initialize() {
    Serial.begin(9600);
    Serial.println("Initialized\n");
    info();
}

void update() {
    // put your update code here
}

void printProgressBar(long percentage, int length=100, char symbol='#', char empty=' ', char end='\n', bool showPercentage=true) {
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
    stream.println("Program Started\n");
    stream.print("Main storage float size: "); stream.print(mainStorage.floatStorage.getRealSize()); stream.print("\n");
    stream.print("Main storage int size: "); stream.print(mainStorage.intStorage.getRealSize()); stream.print("\n");
    stream.print("Main storage char size: "); stream.print(mainStorage.charStorage.getRealSize()); stream.print("\n");
    stream.print("Used space: "); stream.print(storage::getUsedSpace()); stream.print("\n");
    stream.print("Free space: "); stream.print(storage::getFreeSpace()); stream.print("\n");
    stream.print("Total space: "); stream.print(storage::getTotalSpace()); stream.print("\n");
    printProgressBar((storage::getUsedSpace() * 100) / storage::getTotalSpace(), 20, '#', ' ', '\n', true);
    stream.print("\n");
    stream.print("Claster float size: "); stream.print(emulator.registerFloat.getSize()); stream.print("\n");
    stream.print("Claster int size: "); stream.print(emulator.registerInt.getSize()); stream.print("\n");
    stream.print("Claster string size: "); stream.print(emulator.registerString.getSize()); stream.print("\n");
    stream.print("Total ram: "); stream.print(memory::getTotalRam()); stream.print("\n");
    stream.print("Free ram: "); stream.print(memory::getFreeRam()); stream.print("\n");
    stream.print("Used ram: "); stream.print(memory::getUsedRam()); stream.print("\n");
    printProgressBar(static_cast<long long>(memory::getUsedRam()) * 100 / memory::getTotalRam(), 20, '#', ' ', '\n', true);
    stream.print("Chip: "); stream.print(getChipName()); stream.print("\n");
}