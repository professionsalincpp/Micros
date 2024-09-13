#ifndef __CONVERTUTILS_H
#define __CONVERTUTILS_H

#include <Arduino.h>

#define convert(x, y) to##x(y)
#define toint toInt
#define tofloat toFloat
#define todouble toDouble
#define string char*

int toInt(const char* str) {
    return atoi(str);
}

int toInt(char* str) {
    return atoi(str);
}

int toInt(const char* str, int base) {
    return strtol(str, NULL, base);
}

int toInt(char* str, int base) {
    return strtol(str, NULL, base);
}

float toFloat(const char* str) {
    return atof(str);
}

float toFloat(char* str) {
    return atof(str);
}

double toDouble(const char* str) {
    return (double)atof(str);
}

double toDouble(char* str) {
    return (double)atof(str);
}

char* toCharBuffer(const char* str) {
    return (char*)str;
}

char* toCharBuffer(char* str) {
    return (char*)str;
}

char* toCharBuffer(int num) {
    char* str = new char[8];
    sprintf(str, "%d", num);
    return str;
}

char* toCharBuffer(float num) {
    char* str = new char[16];
    sprintf(str, "%f", (double)num);
    return str;
}

char* toCharBuffer(double num) {
    char* str = new char[24];
    sprintf(str, "%f", num);
    return str;
}

char* toCharBuffer(long num) {
    char* str = new char[24];
    sprintf(str, "%ld", num);
    return str;
}

char* toCharBuffer(long long num) {
    char* str = new char[24];
    sprintf(str, "%lld", num);
    return str;
}

char* toCharBuffer(unsigned long num) {
    char* str = new char[24];
    sprintf(str, "%lu", num);
    return str;
}

char* toCharBuffer(unsigned long long num) {
    char* str = new char[24];
    sprintf(str, "%llu", num);
    return str;
}

const char* toCstring(char* str) {
    return str;
}

const char* toCstring(const char* str) {
    return str;
}

const char* toCstring(int num) {
    char* str = new char[8];
    sprintf(str, "%d", num);
    return str;
}

const char* toCstring(float num) {
    char* str = new char[16];
    sprintf(str, "%f", (double)num);
    return str;
}

const char* toCstring(double num) {
    char* str = new char[24];
    sprintf(str, "%f", num);
    return str;
}

const char* toCstring(long num) {
    char* str = new char[24];
    sprintf(str, "%ld", num);
    return str;
}

const char* toCstring(long long num) {
    char* str = new char[24];
    sprintf(str, "%lld", num);
    return str;
}

const char* toCstring(unsigned long num) {
    char* str = new char[24];
    sprintf(str, "%lu", num);
    return str;
}

const char* toCstring(unsigned long long num) {
    char* str = new char[24];
    sprintf(str, "%llu", num);
    return str;
}


#endif // __CONVERTUTILS_H