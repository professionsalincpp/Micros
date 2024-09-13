#include "../headers/stream.h"


// Write a single byte
// @param byte The byte to write
// @return The number of bytes written
template <class SerialType>
size_t OBasicStream<SerialType>::write(uint8_t byte) {
    _serial->write(byte);
    return 1;
}

// Write an array of bytes
// @param buffer The bytes to write
// @param size The number of bytes to write
// @return The number of bytes written
template <class SerialType>
size_t OBasicStream<SerialType>::write(const uint8_t *buffer, size_t size) {
    _serial->write(buffer, size);
    return size;
}

// Check if data is available
// @return size of available bytes if data is available, 0 otherwise
template <class SerialType>
int OBasicStream<SerialType>::available() {
    return _serial->available();
}

// Check if data is available
// @return size of available bytes if data is available, 0 otherwise
template <class SerialType>
int OBasicStream<SerialType>::available(unsigned long timeout) {
    size_t bytes = 0;
    unsigned long start = millis();
    while (millis() - start < timeout) {
        bytes = _serial->available();
        delay(1);
    }
    return bytes;
}

// Read a single byte
// @return The byte read
template <class SerialType>
int OBasicStream<SerialType>::read() {
    return _serial->read();
}

// Read an array of bytes
// @param buffer The buffer to read into
// @param size The number of bytes to read
// @return The number of bytes read
template <class SerialType>
int OBasicStream<SerialType>::read(char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (_serial->available())
            buffer[i] = _serial->read();
    }
    return size;
}

// Read a single byte without removing it from the buffer
// @return The byte
template <class SerialType>
int OBasicStream<SerialType>::peek() {
    return _serial->peek();
}

// Read an array of bytes without removing them from the buffer
// @param buffer The buffer to read into
// @param size The number of bytes to read
// @return The number of bytes read
template <class SerialType>
int OBasicStream<SerialType>::peek(char *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (_serial->available())
            buffer[i] = _serial->peek();
    }
    return size;
}

// Skip a number of bytes
// @param n The number of bytes to skip
template <class SerialType>
void OBasicStream<SerialType>::skip(int n) {
    for (int i = 0; i < n; i++) {
        _serial->read();
    }
}

template <class SerialType> void OBasicStream<SerialType>::skipAll() {
    while (_serial->available()) {
        _serial->read();
    }
}


// Flush the buffer
// @return void
template<class SerialType>
void OBasicStream<SerialType>::flush() {
    _serial->flush();
}

template<class SerialType> 
char *OBasicStream<SerialType>::readAll() {
    char* buffer = new char[available()];
    read(buffer, available());
    return buffer;
}

template<class SerialType>
size_t OStream<SerialType>::send(const char* msg) {
    for (size_t i = 0; i < strlen(msg); i++) {
        this->_serial->write((int)msg[i]);
    }
    return strlen(msg);
}

template <class SerialType>
size_t OStream<SerialType>::send(OMessage& msg) {
    send(msg.c_str());
    return msg.getSize();
}

template<class SerialType>
size_t OStream<SerialType>::send(char* data) {
    const char* msg = data;
    return send(msg);
}

template<class SerialType>
void OStream<SerialType>::setTimeout(int milliseconds) {
    timeout = milliseconds;
}

template<class SerialType>
size_t OStream<SerialType>::receive(char* buffer) {
    int bytes = 0;
    int start = millis();
    do {
        while (this->_serial->available()) {
            buffer[bytes] = (char)this->_serial->read();
            bytes++;
        }
        yield();
        delay(1);
    } while (millis() - start < timeout);
    buffer[bytes] = '\0';
    return bytes;
}

template<class SerialType> template<class PrintAble> size_t OStream<SerialType>::print(PrintAble msg) {
    return this->_serial->print(msg);
}

template<class SerialType> void OBasicStream<SerialType>::clear() {
    skipAll();
}

template<class SerialType> template<class PrintAble> size_t OStream<SerialType>::println(PrintAble msg) {
    return this->_serial->println(msg);
}

template<class SerialType> template<class PrintAble> size_t OStream<SerialType>::send(PrintAble msg) {
    return print(msg);
}

template<class SerialType> OStream<SerialType>& OStream<SerialType>::operator=(const OStream& stream) {
    this->_serial = stream._serial;
    this->timeout = stream.timeout;
    return *this;
}

template<class SerialType> OBasicStream<SerialType>&  OBasicStream<SerialType>::operator=(const OBasicStream& stream) {
    this->_serial = stream._serial;
    return *this;
}

