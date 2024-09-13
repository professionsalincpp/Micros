#include "../headers/types.h"
#include "../utils/stringutils.hpp"


OMessage::OMessage(const char * message) {
    set(message);
}

OMessage::OMessage(const OMessage& message) {
    set(message);
}

OMessage::OMessage(char c) {
    data = (char *)malloc(sizeof(char));
    size = 1;
    data[0] = c;
}

OMessage::OMessage() {
    data = NULL;
    size = 0;
}

OMessage::~OMessage () {
    free(data);
}

void OMessage::set(const char * message) {
    free(data);
    size_t len = strlen(message);
    setSize(len);
    data = (char *)malloc(sizeof(char) * len);
    for (size_t i = 0; i < len; i++) {
        data[i] = message[i];
    }
}

void OMessage::set(const OMessage& message) {
    free(data);
    size_t len = strlen(message.data);
    setSize(len);
    data = (char*)malloc(sizeof(char) * len);
    for (size_t i = 0; i < len; i++) {
        data[i] = message.data[i];
    }
} 

const char * OMessage::c_str(void) {
    const char* tmp = data;
    return tmp;
}

void OMessage::clear() {
    setData(NULL);
    setSize(0);
}

char * OMessage::getData() const {
    return data;
}

size_t OMessage::getSize() const {
    return size;
}

void OMessage::setData(char * data) {
    free(this->data);
    this->data = data;
}

void OMessage::setSize(size_t size) {
    size = size;
}

char &OMessage::operator[](size_t index) const {
    return data[index];
}

OMessage& OMessage::operator=(const OMessage& message) {
    set(message);
    return *this;
}

OMessage& OMessage::operator=(const char* message) {
    set(message);
    return *this;
}

bool OMessage::operator==(const OMessage& message) const {
    return (size == message.size) && (strcmp(data, message.data) == 0);
}

bool OMessage::operator!=(const OMessage& message) const {
    return!(*this == message);
}

OMessage OMessage::operator+(OMessage message) {
    size_t _size = this->getSize() + message.getSize();
    char* _data = (char*)malloc(_size * sizeof(char));
    for (int i = 0; i < (int)this->getSize(); i++) {
        _data[i] = this->data[i];
    }
    for (int i = 0; i < (int)message.getSize(); i++) {
        _data[i + this->getSize()] = message.data[i];
    }
    return OMessage(_data, _size);
}

OMessage OMessage::operator+(const char* message) {
    return *this + OMessage(message);
}

OMessage& OMessage::operator+=(const char* message) {
    this->setData((OMessage(message) + *this).data);
    return *this;
}

OMessage& OMessage::operator+=(const OMessage& other) {
    this->setData((*this + other).data);
    return *this;
}

OMessage& OMessage::operator+=(char c) {
    this->setData((OMessage(c) + *this).data);
    return *this;
}

OMessage OMessage::operator+(char c) {
    return *this + OMessage(c);
}


OPackage::OPackage(uint16_t command, char* data, int size, uint8_t id) {
    this->data = new char[size + 1]; // +1 for null terminator
    strcpy(this->data, data);
    setSize(size);
    setCommand(command);
    setId(id);
}

OPackage::OPackage(uint16_t command, const char * message, uint8_t id) {
    this->data = strdup(message); // strdup выделяет память и копирует строку
    setSize(strlen(message));
    setCommand(command);
    setId(id);
}

OPackage::OPackage(OPackage& package) {
    this->data = new char[package.getSize() + 1]; // +1 для завершающего нулевого символа
    strcpy(this->data, package.data);
    setSize(package.getSize());
    setCommand(package.getCommand());
}

OPackage::OPackage() {
    data = NULL;
    size = 0;
}

OPackage::~OPackage () {
    if (data != NULL) {
        free(data);
    }
}

void OPackage::set(const char * message) {
    if (data != NULL) {
        delete[] data;
    }
    data = strdup(message);
    setSize(strlen(message));
}

void OPackage::set(OPackage& package) {
    if (data != NULL) {
        delete[] data;
    }
    data = new char[package.getSize() + 1]; // +1 для завершающего нулевого символа
    strcpy(data, package.data);
    setSize(package.getSize());
}

const char * OPackage::c_str(void) {
    const char* tmp = data;
    return tmp;
}

void OPackage::clear() {
    setData(NULL);
    setSize(0);
}

char * OPackage::getData() {
    return this->data;
}

size_t OPackage::getSize() {
    return this->size;
}

void OPackage::setData(char * data) {
    if (this->data != nullptr) {
        delete[] this->data;
    }
    this->data = new char[strlen(data) + 1];
    strcpy(this->data, data);
}

void OPackage::setSize(size_t size) {
    this->size = size;
}

uint8_t OPackage::getId() {
    return this->id;
}

void OPackage::setId(uint8_t id) {
    this->id = id;
}

char &OPackage::operator[](size_t index) const {
    return data[index];
}

OPackage& OPackage::operator=(const OPackage& package) {
    if (this != &package) {
        if (data != NULL) {
            free(data);
        }
        size_t len = strlen(package.data);
        data = (char*)malloc(sizeof(char) * len);
        for (size_t i = 0; i < len; i++) {
            data[i] = package.data[i];
        }
    }
    return *this;
}

OPackage& OPackage::operator=(const char* message) {
    set(message);
    return *this;
}

uint16_t OPackage::getCommand() {
    return command;
}

void OPackage::setCommand(uint16_t command) {
    this->command = command;
}

const char* operator+(const char* msg1, OMessage msg2) {
    char * tmp;
    size_t len = strlen(msg1);
    tmp = (char *)malloc((len + msg2.getSize()) * sizeof(char));
    for (size_t i = 0; i < len; i++) {
        tmp[i] = msg1[i];
    }
    for (size_t i = 0; i < msg2.getSize(); i++) {
        tmp[i + len] = msg2.data[i];
    }
    return tmp;
}

const char *OPackage::getDataCstr() {
    char* tmp = (char*)malloc(sizeof(char) * (size + 1));
    for (size_t i = 0; i < size; i++) {
        tmp[i] = data[i];
    }
    tmp[size] = '\0';
    const char* msg = tmp;
    free(tmp);
    return msg;
}

const char* OPackage::getArg(int index) {
    if (index < 0 || index >= countArgs()) {
        return NULL;
    }
    const char **args = getArgs();
    const char *tmp = args[index];
    if (args != NULL) {
        free(args);
    }
    return tmp;
}

int OPackage::countArgs() {
    if (data == NULL) {
        return 0;
    }
    int argc = 1;
    for (size_t i = 0; i < size; i++) {
        if (data[i] == '-') {
            argc++;
        }
    }
    return argc;
}

const char** OPackage::getArgs() {
    char* tmp = data;
    int argStart = 0;
    int arg = 0;
    char** args = (char**)malloc(sizeof(char*) * countArgs());
    for (int i = 0; i < (int)size; i++) {
        if (tmp[i] == '-' || tmp[i] == '\0') {
            // copy string from argStart to i
            args[arg] = (char*)malloc(sizeof(char) * (i - argStart + 1));
            for (int j = argStart; j < i; j++) {
                args[arg][j - argStart] = tmp[j];
            }
            args[arg][i - argStart] = '\0';
            arg++;
            argStart = i + 1;
        }
    }
    return const_cast<const char**>(args);
}









