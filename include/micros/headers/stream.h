#ifndef __STREAM_H
#define __STREAM_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "types.h"
#include <EEPROM.h>

template <typename SerialType>
class OBasicStream : public Print {
template <typename T> friend class OStream;
public: 
    SerialType* _serial;
    OBasicStream() {}
    OBasicStream(SerialType* serial) : _serial(serial) {}
    size_t write(uint8_t byte) override;
    size_t write(const uint8_t *buffer, size_t size) override;
    int available();
    int available(unsigned long timeout);
    int read();
    int read(char *buffer, size_t size);
    int peek();
    int peek(char *buffer, size_t size);
    char *readAll();
    void skip(int n);
    void skipAll();
    void flush();
    void clear();
    OBasicStream &operator=(const OBasicStream &stream);
    using Print::write;
};

template <typename SerialType>
class OStream : public OBasicStream<SerialType> {
    unsigned long timeout = 50;
public:
    OStream() : OBasicStream<SerialType>() {}
    OStream(SerialType* serial) : OBasicStream<SerialType>(serial) {}
    OStream(SerialType* serial, int timeout) : OBasicStream<SerialType>(serial), timeout(timeout) {}
    void setTimeout(int milliseconds);
    size_t send(const char* msg);
    size_t send(OMessage& msg);
    size_t send(char* data);
    template <typename PrintAble>
    size_t send(PrintAble msg);
    template <typename PrintAble>
    size_t print(PrintAble msg);
    template <typename PrintAble>
    size_t println(PrintAble msg);
    size_t printf(const char* format, ...);
    size_t receive(char* buffer);
    OStream& operator=(const OStream& stream);
};

#endif // __STREAM_H