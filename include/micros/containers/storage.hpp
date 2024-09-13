#ifndef __STORAGE_H
#define __STORAGE_H

#include "eeprom.h"
#include "../utils/stringutils.hpp"
#include "../utils/convertutils.hpp"


namespace storage {

static int __storageFloatSize = 10;
static int __storageIntSize = 10;
static int __storageCharSize = 80; 

#define CONST_INT(x) const_cast<int>(x)
#define STORAGE_SIZE_FLOAT __storageFloatSize
#define STORAGE_SIZE_INT __storageIntSize
#define STORAGE_SIZE_CHAR __storageCharSize


/** 
 * @brief Storage configuration
 * @param config storage configuration
 * @code{.cpp}
 * CONFIG_STORAGE(float: 10%; int: 20b; uint: 80b;)
 * @endcode
**/
#define CONFIG_STORAGE(...) storage::config_storage(#__VA_ARGS__)

int convertPercentageToMemory(int percentage) {
    return (percentage * (EEPROM.length() / 100));
}

void config_storage(const char * config) {
    String str = String(config);
    int count = countChar(str, ';') + 1;
    for (int i = 0; i < count; i++) {
        String arg = getValue(str, ';', i);
        String name = getValue(arg, ':', 0);
        String value = getValue(arg, ':', 1);
        name.trim();
        value.trim();
        if (name.equalsIgnoreCase("float")) {
            if (value.endsWith("%")) {
                __storageFloatSize = convertPercentageToMemory(value.toInt());
            } else if (value.endsWith("b")) {
                __storageFloatSize = value.toInt() - value.toInt() % sizeof(float);
            } else {
                __storageFloatSize = value.toInt() * sizeof(float);
            }
        } else if (name.equalsIgnoreCase("int")) {
            if (value.endsWith("%")) {
                __storageIntSize = convertPercentageToMemory(value.toInt());
            } else if (value.endsWith("b")) {
                __storageIntSize = value.toInt() - value.toInt() % sizeof(int);
            } else {
                __storageIntSize = value.toInt() * sizeof(int);
            }
        } else if (name.equalsIgnoreCase("char")) {
            if (value.endsWith("%")) {
                __storageCharSize = convertPercentageToMemory(value.toInt());
            } else if (value.endsWith("b")) {
                __storageCharSize = value.toInt() - value.toInt() % sizeof(char);
            } else {
                __storageCharSize = value.toInt() * sizeof(char);
            }
        }
    }
}


static int __lastoffset = 0;

template <typename T>
class StorageCell {
public:
    T value;
    int offset;
    StorageCell(int offset) {
        this->offset = offset;
        EEPROM.get(offset, value);
    }
    T& operator=(const T& other) {
        value = other;
        EEPROM.put(offset, value);
        return value;
    }
    T& operator=(T&& other) {
        value = other;
        EEPROM.put(offset, value);
        return value;
    }

    operator T() { return value; }
    operator T() const { return value; }
    T* operator&() { return &value; }
    T* operator&() const { return &value; }
    T get() { return value; }
    T get() const { return value; }
    void set(T value) { 
        this->value = value; 
        EEPROM.put(offset, value);
    }
    void set(T&& value) { 
        this->value = value; 
        EEPROM.put(offset, value); 
    }
};

template <class T>
class OStorage {
public:
    size_t size;
    int offset;
    int realsize;
    OStorage() {
        size = 0;
        offset = 0;
        realsize = 0;
    }

    OStorage(size_t size) {
        size_t max_size = (EEPROM.length() - __lastoffset) / sizeof(T);
        size = min(size, max_size);
        offset = __lastoffset;
        int __newoffset = offset + sizeof(T) * size;
        realsize = __newoffset - offset;
        if (realsize < (int)sizeof(T)) {
            realsize = 0; // если размер будет меньше чем занимает тип данных, то обнуляем его
        }
        __lastoffset = __newoffset;
    }
    StorageCell<T> operator[](size_t index) { 
        StorageCell<T> ret(offset + index * sizeof(T));
        return ret;
    }
    size_t getSize() const { return realsize / sizeof(T); }
    size_t getRealSize() const { return realsize; }
    ~OStorage() = default;
};

class OMainStorage {
public:
    OStorage<float> floatStorage = OStorage<float>(STORAGE_SIZE_FLOAT / sizeof(float));
    OStorage<int> intStorage = OStorage<int>(STORAGE_SIZE_INT / sizeof(int));
    OStorage<char> charStorage = OStorage<char>(STORAGE_SIZE_CHAR / sizeof(char));
    OMainStorage() = default;
    ~OMainStorage() = default;
};

/// @brief gets available space in EEPROM
/// @return size in bytes
size_t getFreeSpace() {
    return min(EEPROM.length() - __lastoffset, EEPROM.length());
}

/// @brief gets used space in EEPROM
/// @return size in bytes
size_t getUsedSpace() {
    return __lastoffset;
}

/// @brief gets total space in EEPROM
/// @return size in bytes
size_t getTotalSpace() {
    return EEPROM.length();
}

} // namespace Storage



#endif // __STORAGE_H