#include "../headers/emulator.h"
#include "../headers/types.h"
#include "../utils/stringutils.hpp"

void Emulator::initialize() {
    #define EMULATOR_INITIALIZED 1
}

void Emulator::setId(uint8_t id) {
    this->id = id;
}

uint8_t Emulator::getId() {
    return id;
}

void Emulator::setAccess(EAccess access) {
    this->access = access;
}

EAccess Emulator::getAccess() {
    return access;
}

Emulator &Emulator::operator=(const Emulator &other) {
    this->id = other.id;
    this->access = other.access;
    return *this;
}

int Emulator::setRegister(int index, int value) {
    if (index >= 0 && index < __clasters_size_int) {
        registerInt[index] = value;
        return 0;
    }
    return -1;
}

int Emulator::setRegister(int index, float value) {
    if (index >= 0 && index < __clasters_size_float) {
        registerFloat[index] = value;
        return 0;
    }
    return -1;
}

int Emulator::setRegister(int index, const char* value) {
    if (index >= 0 && index < __clasters_size_string) {
        registerString[index] = const_cast<char*>(value);
        return 0;
    }
    return -1;
}

int Emulator::getIntRegister(int index) {
    if (index >= 0 && index < __clasters_size_int) {
        return registerInt[index];
    }
    return -1;
}

float Emulator::getFloatRegister(int index) {
    if (index >= 0 && index < __clasters_size_float) {
        return registerFloat[index];
    }
    return -1.0f;
}

const char* Emulator::getStringRegister(int index) {
    if (index >= 0 && index < __clasters_size_string) {
        return registerString[index];
    }
    return nullptr;
}

int Emulator::getMemory(int address) {
    if (address >= 0 && address < MEMORY_SIZE) {
        return memory[address];
    }
    return -1;
}

void Emulator::resize(int _float_size, int _int_size, int _string_size) {
    registerFloat.resize(_float_size);
    registerInt.resize(_int_size);
    registerString.resize(_string_size);
}

void Emulator::setMemory(int address, int value) {
    if (address >= 0 && address < MEMORY_SIZE) {
        memory[address] = value;
    }
}

void Emulator::addHook(EmulatorHook hook, int command) {
    hooks.append(OPair<int, EmulatorHook>(command, hook));
}

int Emulator::getStack(int index) {
    // Implement stack operations
    // ...
    return -1;
}

void Emulator::setStack(int index, int value) {
    // Implement stack operations
    // ...
}

void Emulator::setPin(uint8_t pin, int value) {
    digitalWrite(pin, value);
}

int Emulator::getPin(uint8_t pin) {
    return digitalRead(pin);
}

// @brief Send ping command
// @param stream Serial stream
// @param id Device id
// @return device status on success 243 on error
template<class Stream> int Emulator::ping(Stream& stream, uint8_t id) {
    char charId = static_cast<char>(id) + '0';
    stream.print(charId);
    stream.print(",");
    stream.print(ECommands::PING);
    stream.print(",;\n");
    stream.flush();
    unsigned long start = millis();
    char buffer[16];
    buffer[0] = '2';
    buffer[1] = '4';
    buffer[2] = '3';
    int bytes = 0;
    while (millis() - start < PING_TIMEOUT) {
        bool available = false;
        while (stream.available() > 0) {
            buffer[bytes] = stream.read();
            bytes++;
            available = true;
        }
        if (available) break;
    }
    buffer[bytes] = '\0';
    stream.println(buffer);
    return atol(buffer);
}

// @brief Ping device if answer is OK then send package else return error
// @param stream Serial stream
// @param package Package to send
// @return 0 on success, -1 on error
template<class Stream> int Emulator::sendPackage(Stream& stream, OPackage& package) {
    int status = this->ping(stream, package.getId());
    if (status == EAnswer::OK) {
        //char charId = static_cast<char>(package.getId());
        String toSend = String((char)(package.getId() + '0')) + "," + String(package.getCommand()) + "," + String(package.getData()) + ";";
        stream.send(toSend.c_str());
        return 0;
    }
    return -1;
}

// @brief Receive package
// @param stream Serial stream
// @param package Package to receive
// @return 0 on success, -1 on error, -2 on invalid package
template<class Stream>
int Emulator::receivePackage(Stream& stream, OPackage& package) {
    const int bufferSize = 256;
    char buffer[bufferSize];
    int bytesRead = stream.receive(buffer);
    if (bytesRead <= 0) {
        int count = 0;
        stream.println("Bytes read: " + String(count));
        stream.println("Received: ");
        while (stream.available() > 0) {
            char c = (char)stream.read();
            stream.print(count++);
            stream.print(" = '");
            stream.print(c);
            stream.print("'\n");
        }
        stream.println("Received: " + String(count));
        return ReceivePackageError::PackageReceiveError;
    }
    
    String toReceive = String(buffer);
    // if (toReceive.length() == 0 || !isValidId(toReceive.charAt(0))) {
    //     stream.println("Invalid package: " + toReceive);
    //     return ReceivePackageError::InvalidPackage;
    // }

    char id = toReceive.charAt(0);
    String command = toReceive.substring(toReceive.indexOf(',') + 1, toReceive.lastIndexOf(','));
    String data = toReceive.substring(toReceive.lastIndexOf(',') + 1, toReceive.indexOf(';'));

    package.setCommand(command.toInt());
    package.setId((int)(id - '0'));

    char* dataBuffer = new char[data.length() + 1];
    data.toCharArray(dataBuffer, data.length() + 1);
    package.setData(dataBuffer);
    package.setSize(data.length() + 1);

    delete[] dataBuffer;

    return ReceivePackageError::NoError;
}

bool Emulator::isValidId(char id) {
    uint8_t idInt = id - '0';
    return idInt == this->id;
}

// @brief Process common tasks
// @param stream Serial stream
// @return ECommonTaskProcessError::ErrorNone on success, else ECommonTaskProcessError
template<class Stream> int Emulator::processCommonTasks(Stream& stream) {
    stream.println("Status: " + String(this->id));
    if (!isTasksAvailable(stream)) return ECommonTaskProcessError::NoTasksAvailable;
    OPackage package;
    int receiveStatus = receivePackage(stream, package);
    stream.println("Command: " + String(package.getCommand()));
    if (receiveStatus != ReceivePackageError::NoError) {
        if (receiveStatus == ReceivePackageError::PackageReceiveError) return ECommonTaskProcessError::ReceiveError;
        if (receiveStatus == ReceivePackageError::InvalidPackage) return ECommonTaskProcessError::ReceiveInvalidPackage;
    }
    int processStatus = processTask(stream, package);
    return processStatus;
}

template<class Stream> bool Emulator::isTasksAvailable(Stream& stream) {
    if (stream.available(5ul) > 0) {
        return peekId(stream) == this->id;
    }
    return false;
}

template<class Stream> uint8_t Emulator::peekId(Stream& stream) {
    int id = stream.peek();
    if (id < 0) return -1;
    return (int)(id - '0');
}

template<class Stream>
int Emulator::processTask(Stream &stream, OPackage &package) {
    // Проверка доступа
    if (!checkAccess(stream, package.getCommand())) {
        return ECommonTaskProcessError::AccessDenied;
    }

    // Обработка команд
    switch (package.getCommand()) {
        case ECommands::PING:
            return processPing(stream, package);
        case ECommands::SET_MEMORY:
            return processSetMemory(stream, package);
        case ECommands::SET_PIN:
            return processSetPin(stream, package);
        case ECommands::SET_REGISTER:
            return processSetRegister(stream, package);
        case ECommands::SET_STACK:
            return processSetStack(stream, package);
        case ECommands::GET_PIN:
            return processGetPin(stream, package);
        case ECommands::GET_MEMORY:
            return processGetMemory(stream, package);
        case ECommands::GET_REGISTER:
            return processGetRegister(stream, package);
        case ECommands::GET_STACK:
            return processGetStack(stream, package);
        default:
            for (auto hook : hooks) {
                int command = hook.first;
                EmulatorHook hookFunction = hook.second;
                if (command == (int)package.getCommand()) {
                    return hookFunction(stream, package);
                }
            }
            return ECommonTaskProcessError::TaskProcessError;
    }
    return ECommonTaskProcessError::TaskProcessError;
}

// Функции-обработчики для каждой команды
template<class Stream>
int Emulator::processPing(Stream &stream, OPackage &package) {
    return stream.send(EAnswer::OK);
}

template<class Stream>
int Emulator::processSetMemory(Stream &stream, OPackage &package) {
    int index = getArgAsInt(package, 0);
    int value = getArgAsInt(package, 1);
    if (index == -1 || value == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    setMemory(index, value);
    return ECommonTaskProcessError::ErrorNone;
}

template<class Stream>
int Emulator::processSetPin(Stream &stream, OPackage &package) {
    int pin = getArgAsInt(package, 0);
    int value = getArgAsInt(package, 1);
    stream.print("setPin(" + String(pin) + ", " + String(value) + ")");
    if (pin == -1 || value == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    setPin(pin, value);
    return ECommonTaskProcessError::ErrorNone;
}

template<class Stream>
int Emulator::processSetRegister(Stream &stream, OPackage &package) {
    int index = getArgAsInt(package, 0);
    int value = getArgAsInt(package, 1);
    stream.print("setRegister(" + String(index) + ", " + String(value) + ")");
    if (index == -1 || value == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    setRegister(index, value);
    stream.println(" = " + String(getIntRegister(index)));
    return ECommonTaskProcessError::ErrorNone;
}

template<class Stream>
int Emulator::processSetStack(Stream &stream, OPackage &package) {
    int index = getArgAsInt(package, 0);
    int value = getArgAsInt(package, 1);
    if (index == -1 || value == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    setStack(index, value);
    return ECommonTaskProcessError::ErrorNone;
}

template<class Stream>
int Emulator::processGetPin(Stream &stream, OPackage &package) {
    int pin = getArgAsInt(package, 0);
    if (pin == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    stream.send(getPin(pin));
    return ECommonTaskProcessError::ErrorNone;
}

template<class Stream>
int Emulator::processGetMemory(Stream &stream, OPackage &package) {
    int address = getArgAsInt(package, 0);
    if (address == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    stream.send(getMemory(address));
    return ECommonTaskProcessError::ErrorNone;
}

template<class Stream>
int Emulator::processGetRegister(Stream &stream, OPackage &package) {
    int index = getArgAsInt(package, 0);
    if (index == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    stream.send(getIntRegister(index));
    return ECommonTaskProcessError::ErrorNone;
}

template<class Stream>
int Emulator::processGetStack(Stream &stream, OPackage &package) {
    int index = getArgAsInt(package, 0);
    if (index == -1) {
        return ECommonTaskProcessError::InvalidArgument;
    }
    stream.send(getStack(index));
    return ECommonTaskProcessError::ErrorNone;
}

// Функция для преобразования аргумента в целое число
int Emulator::getArgAsInt(OPackage &package, int argIndex) {
    const char* arg = package.getArg(argIndex);
    if (arg == nullptr) {
        return -1;
    }
    return atoi(arg);
}

template<class Stream> bool Emulator::checkAccess(Stream &stream, uint16_t command) {
    if (this->access == FULLACCESS) return true;
    if (this->access == NOACCESS) return false;
    else {
        switch (command) {
            case ECommands::SET_PIN:
                return this->access.SET_PIN;
            case ECommands::SET_REGISTER:
                return this->access.SET_REGISTER;
            case ECommands::SET_MEMORY:
                return this->access.SET_MEMORY;
            case ECommands::SET_STACK:
                return this->access.SET_STACK;
            default:
                return true;
        }
    }
    return true;
}