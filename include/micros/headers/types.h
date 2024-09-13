#ifndef __OSLIB_TYPES_H
#define __OSLIB_TYPES_H

#include <Arduino.h>
#include <EEPROM.h>
#include "../containers/claster.hpp"
#include "../containers/list.hpp"
#include "../containers/storage.hpp"
#include "../containers/pair.hpp"

#define charset char*

using Int = int;
using Float = float;
using Double = double;
using CharBuffer = char*;
using CString = const char*;
using IntArray = int*;
using FloatArray = float*;
using DoubleArray = double*;
using CharBufferArray = char**;
using CStringArray = const char**;
using IntList = OList<int>;
using FloatList = OList<float>;
using DoubleList = OList<double>;
using CharBufferList = OList<char*>;
using CStringList = OList<const char*>;

#define uint unsigned int

#define claster4_t(x) OClaster<4, x>
#define claster8_t(x) OClaster<8, x>
#define claster16_t(x) OClaster<16, x>
#define claster32_t(x) OClaster<32, x>

#define message_t(x) OMessage x

#define CLASTER_CONFIG(x) #x

class OMessage {
public:
    char* data;
    size_t size;
    OMessage(char* data, int size) : data(data), size(size) {}
    OMessage(const char* message);
    OMessage(const OMessage& message);
    OMessage(char c);
    OMessage();
    ~OMessage();

    void set(const char* message);
    void set(const OMessage& message);
    const char* c_str();
    void clear();
    char* getData() const;
    size_t getSize() const;
    void setData(char* data);
    void setSize(size_t size);
    char &operator[](size_t index) const;
    OMessage& operator=(const OMessage& message);
    OMessage& operator=(const char* message);
    OMessage operator+(OMessage message);
    OMessage operator+(const char* message);
    OMessage operator+(char c);
    OMessage& operator+=(const OMessage& message);
    OMessage& operator+=(const char* message);
    OMessage& operator+=(char c);
    bool operator==(const OMessage& message) const;
    bool operator!=(const OMessage& message) const;
};

// @brief Package for communication
// @param command Command
// @param data Data
// @param size Size
// @param id Device id
class OPackage {
public:
    uint16_t command;
    char* data;
    size_t size;
    uint8_t id;
    OPackage(uint16_t command, char* data, int size, uint8_t id);
    OPackage(uint16_t command, const char* message, uint8_t id);
    OPackage(OPackage& package);
    OPackage();
    ~OPackage();
    void set(const char* message);
    void set(OPackage& message);
    const char* c_str();
    void clear();
    char* getData();
    const char* getDataCstr();
    uint16_t getCommand();
    size_t getSize();
    void setCommand(uint16_t command);
    void setData(char* data);
    void setSize(size_t size);
    void setId(uint8_t id);
    uint8_t getId();
    char &operator[](size_t index) const;
    OPackage& operator=(const OPackage& message);
    OPackage& operator=(const char* message);
    const char* getArg(int index);
    int countArgs();
    const char** getArgs();
};


#endif // __OSLIB_TYPES_H__