#ifndef __EMULATOR_H
#define __EMULATOR_H

#include <Arduino.h>
#include "../containers/claster.hpp"
#include "../containers/list.hpp"
#include "../containers/pair.hpp"
#include "../utils/funcutils.hpp"

#define MEMORY_SIZE 0x80
#define STACK_SIZE 0x10

#define REGISTER_INT 0x0
#define REGISTER_FLOAT 0x1
#define REGISTER_STRING 0x2

#ifndef PING_TIMEOUT
#define PING_TIMEOUT 100
#endif // PING_TIMEOUT

enum ECommands {
    PING = 0x1, // PING command
    SET_REGISTER = 0x2, // SET_REGISTER command
    GET_REGISTER = 0x3, // GET_REGISTER command
    GET_MEMORY = 0x4, // GET_MEMORY command
    SET_MEMORY = 0x5, // SET_MEMORY command
    GET_STACK = 0x6, // GET_STACK command
    SET_STACK = 0x7, // SET_STACK command
    SET_PIN = 0x8, // SET_PIN command
    GET_PIN = 0x9, // GET_PIN command
};

enum EAnswer {
    OK = 0xF1,
    ERROR = 0xF2,
    TIMEOUT = 0xF3
};

enum ECommonTaskProcessError {
    ErrorNone = 0,
    NoTasksAvailable = 1,
    ReceiveError = 2,
    ReceiveInvalidPackage = 3,
    TaskProcessError = 4,
    AccessDenied = 5,
    StreamNullPtr = 6,
    InvalidArgument = 7
};

enum ReceivePackageError {
    NoError = 0,
    PackageReceiveError = 1,
    InvalidPackage = 2
};

struct EAccess {
    bool SET_PIN = false;
    bool SET_REGISTER = false;
    bool SET_MEMORY = false;
    bool SET_STACK = false;
    bool SET_STORAGE = false;
    EAccess(bool pin, bool register_, bool memory, bool stack, bool storage)
        : SET_PIN(pin), SET_REGISTER(register_), SET_MEMORY(memory), SET_STACK(stack), SET_STORAGE(storage) {}
    bool operator[](int index) const {
        switch (index) {
            case 0:
                return SET_PIN;
            case 1:
                return SET_REGISTER;
            case 2:
                return SET_MEMORY;
            case 3:
                return SET_STACK;
            case 4:
                return SET_STORAGE;
            default:
                return false;
        }
    }
    bool operator==(const EAccess& access) const {
        return SET_PIN == access.SET_PIN && SET_REGISTER == access.SET_REGISTER && SET_MEMORY == access.SET_MEMORY && SET_STACK == access.SET_STACK && SET_STORAGE == access.SET_STORAGE;
    }
    bool operator!=(const EAccess& access) const {
        return !(*this == access);
    }
};

extern const EAccess FULLACCESS {true, true, true, true, true};
extern const EAccess NOACCESS {false, false, false, false, false};
extern const EAccess PROTECTEDACCESS {false, true, false, false, true};

using EmulatorHook = ECommonTaskProcessError (*)(OStream<SERIALTYPE> &stream, OPackage &package);

class Emulator {
public:
    uint8_t id;
    EAccess access = PROTECTEDACCESS;
    claster::OClaster<int> registerInt = claster::OClaster<int>(__clasters_size_int);
    claster::OClaster<float> registerFloat = claster::OClaster<float>(__clasters_size_float);
    claster::OClaster<char*> registerString = claster::OClaster<char*>(__clasters_size_string);
    claster::OClaster<uint8_t> memory = MEMORY_SIZE;
    claster::OClaster<uint16_t> stack = STACK_SIZE;
    OList<OPair<int, EmulatorHook>> hooks;
    Emulator(uint8_t id, EAccess access) : id(id), access(access) {
        configIfNot();
        this->resize(__clasters_size_float, __clasters_size_int, __clasters_size_string);
    }
    Emulator(uint8_t id) : id(id) { 
        configIfNot();
        this->resize(__clasters_size_float, __clasters_size_int, __clasters_size_string);
    }
    Emulator() {
        configIfNot();
        this->resize(__clasters_size_float, __clasters_size_int, __clasters_size_string);
    }
    ~Emulator() = default;
    void initialize();
    void setId(uint8_t id);
    void resize(int _float_size, int _int_size, int _string_size);
    void addHook(EmulatorHook hook, int command=10);
    uint8_t getId();
    void setAccess(EAccess access);
    EAccess getAccess();
    int setRegister(int index, int value);
    int setRegister(int index, float value);
    int setRegister(int index, const char* value);
    int getIntRegister(int index);
    float getFloatRegister(int index);
    const char* getStringRegister(int index);
    int getMemory(int address);
    void setMemory(int address, int value);
    int getStack(int index);
    void setStack(int index, int value);
    void setPin(uint8_t pin, int value);
    int getPin(uint8_t pin);
    template<typename Stream>
    int ping(Stream& stream, uint8_t id);
    template<typename Stream>
    int sendPackage(Stream& stream, OPackage& package);
    template<typename Stream>
    int receivePackage(Stream& stream, OPackage& package);
    template<typename Stream>
    int processCommonTasks(Stream& stream);
    template<typename Stream>
    int processTask(Stream& stream, OPackage& package);
    template<class Stream>
    int processPing(Stream &stream, OPackage &package);
    template<class Stream>
    int processSetMemory(Stream &stream, OPackage &package);
    template<class Stream>
    int processSetPin(Stream &stream, OPackage &package);
    template<class Stream>
    int processSetRegister(Stream &stream, OPackage &package);
    template<class Stream>
    int processSetStack(Stream &stream, OPackage &package);
    template<class Stream>
    int processGetPin(Stream &stream, OPackage &package);
    template<class Stream>
    int processGetMemory(Stream &stream, OPackage &package);
    template<class Stream>
    int processGetRegister(Stream &stream, OPackage &package);
    template<class Stream>
    int processGetStack(Stream &stream, OPackage &package);
    template<typename Stream>
    bool isTasksAvailable(Stream& stream);
    template<typename Stream>
    uint8_t peekId(Stream& stream);
    template<typename Stream>
    bool checkAccess(Stream& stream, uint16_t command);
    bool isValidId(char id);
    Emulator& operator=(const Emulator& emulator);
private:
    int getArgAsInt(OPackage &package, int argIndex);
};

#endif  // __EMULATOR_H