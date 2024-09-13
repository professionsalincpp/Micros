#ifndef __OSLIB_CLASTER_H
#define __OSLIB_CLASTER_H

#include <Arduino.h>
#include "../utils/stringutils.hpp"
#include "../utils/memutils.hpp"

#define SYSTEM_CONFIG(x) init_system(#x); //CombinedOclaster<parse_claster_sizes(#x)[0], parse_claster_sizes(#x)[1], parse_claster_sizes(#x)[2]>
#define CONFIG_CLASTER(...)  claster::config_claster(#__VA_ARGS__)

static int __clasters_size_float = 4;
static int __clasters_size_int = 4;
static int __clasters_size_string = 8;

#define FLOAT_CLASTER_SIZE __clasters_size_float
#define INT_CLASTER_SIZE __clasters_size_int
#define STRING_CLASTER_SIZE __clasters_size_string


namespace claster {

    int percentageToBytes(int percentage) {
        return (percentage * (memory::getFreeRam() / 100));
    }

    void config_claster(const char* config) {
        int len = char_count(config, ';') + 1;
        for (int i = 0; i < len; i++) {
            String arg = getValue(config, ';', i);
            String name = getValue(arg, ':', 0);
            String value = getValue(arg, ':', 1);
            name.trim();
            value.trim();
            // Serial.println(arg);
            // Serial.print("name: '");
            // Serial.print(name); 
            // Serial.print("'\nvalue: '");
            // Serial.println(value);
            // Serial.println("'");
            if (name.equals("float")) {
                if (value.endsWith("%")) {
                    __clasters_size_float = percentageToBytes(value.toInt()) / sizeof(float);
                } else if (value.endsWith("b")) {
                    __clasters_size_float = value.toInt() / sizeof(float);
                } else {
                    __clasters_size_float = value.toInt();
                }
            } else if (name.equals("int")) {
                if (value.endsWith("%")) {
                    __clasters_size_int = percentageToBytes(value.toInt()) / sizeof(int);
                } else if (value.endsWith("b")) {
                    __clasters_size_int = value.toInt() / sizeof(int);
                } else {
                    __clasters_size_int = value.toInt();
                }
            } else if (name.equals("string")) {
                __clasters_size_string = value.toInt();
            }
        }
    }

    constexpr int init_system(const char* config) {
        return 0;
    }

    size_t *parse_claster_sizes(const char *str) {
        size_t *sizes = new size_t[3];
        String token;
        String current_size;
        for (size_t i = 0; i < strlen(str); i++) {
            if (str[i] == ':') {
                current_size = token;
                token = "";
            } else if (str[i] == ';') {
                if (current_size.equals("int")) {
                    sizes[0] = token.toInt();
                } else if (current_size.equals("float")) {
                    sizes[1] = token.toInt();
                } else if (current_size.equals("string")) {
                    sizes[2] = token.toInt();
                }
            } else {
                token += str[i];
            }
        }
        return sizes;
    }

    // static int __clasters_size = 0;

    template<typename dataType>
    class OClaster {
    public:
        size_t _size;
        dataType* data;

        OClaster(size_t size) : _size(size), data(new dataType[size]) {
            
        }

        OClaster(dataType* data, size_t size) : _size(size), data(data) {
            
        }

        ~OClaster() {
            delete[] data;
        }

        dataType get(int index) const {
            if (index < 0 || index >= _size) {
                return data[0];
            }
            return data[index];
        }

        void set(int index, dataType value) {
            if (index < 0 || index >= _size) {
                return;
            }
            data[index] = value;
        }

        void resize(size_t size) {
            _size = size;
            if (data != nullptr) {
                delete[] data;
            }
            data = new dataType[size];
        }

        size_t getSize() const {
            return _size;
        }

        size_t getRealSize() const {
            return _size * sizeof(dataType);
        }

        dataType& operator[](size_t index) {
            if (index < 0 || index >= _size) {
                return data[0];
            }
            return data[index];
        }

        OClaster(const OClaster& other) : _size(other._size), data(new dataType[_size]) {
            for (size_t i = 0; i < _size; i++) {
                data[i] = other.data[i];
            }
        }

        OClaster& operator=(const OClaster& other) {
            if (this != &other) {
                delete[] data;
                _size = other._size;
                data = new dataType[_size];
                for (size_t i = 0; i < _size; i++) {
                    data[i] = other.data[i];
                }
            }
            return *this;
        }
    };
    
} //namespace claster

#endif