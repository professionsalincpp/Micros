#include <Arduino.h>

#ifndef __OSLIB_STRINGUTILS_H
#define __OSLIB_STRINGUTILS_H

const char* concat_const_char(const char* str1, const char* str2) {
    char* str = (char*)malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}
#include <string.h>

char** split(const char* str, const char* delimiter, int* count) {
    int len = strlen(str);
    int delimiter_len = strlen(delimiter);
    int num_args = 0;
    for (int i = 0; i < len; i++) {
        if (strncmp(str + i, delimiter, delimiter_len) == 0) {
            num_args++;
            i += delimiter_len - 1;
        }
    }
    num_args++;

    char** args = (char**)malloc((num_args + 1) * sizeof(char*));
    int arg_len = 0;
    for (int i = 0; i < len; i++) {
        if (strncmp(str + i, delimiter, delimiter_len) == 0) {
            args[arg_len] = (char*)malloc((i - arg_len + 1) * sizeof(char));
            strncpy(args[arg_len], str + arg_len, i - arg_len);
            args[arg_len][i - arg_len] = '\0';
            arg_len = i + delimiter_len;
        }
    }
    args[arg_len] = (char*)malloc((len - arg_len + 1) * sizeof(char));
    strncpy(args[arg_len], str + arg_len, len - arg_len);
    args[arg_len][len - arg_len] = '\0';

    *count = num_args;
    return args;
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length()-1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if(data.charAt(i)==separator || i==maxIndex){
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }

    return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

int countChar(String str, char c) {
    int count = 0;
    for (int i = 0; i < (int)str.length(); i++) {
        if (str.charAt(i) == c) {
            count++;
        }
    }
    return count;
}

const char** str_split(const char* a_str, const char a_delim) {
    int count = 0;
    const char* p = a_str;

    // Подсчитываем количество токенов
    while (*p != '\0') {
        if (*p == a_delim) {
            count++;
        }
        p++;
    }

    // Выделяем память для массива строк
    const char** result = new const char*[count + 1];

    // Выделяем память для каждого токена
    p = a_str;
    int i = 0;
    while (*p != '\0') {
        int len = 0;
        while (*p != '\0' && *p != a_delim) {
            len++;
            p++;
        }

        // Выделяем память для токена
        char* token = new char[len + 1];
        strncpy(token, a_str, len);
        token[len] = '\0';

        // Добавляем токен в массив
        result[i] = token;
        i++;

        // Пропускаем разделитель
        if (*p == a_delim) {
            p++;
        }
    }

    // Добавляем nullptr в конец массива
    result[count] = nullptr;

    return result;
}

const char* str_strip(const char* str) {
    size_t str_len = strlen(str);
    size_t new_len = 0;
    int start_index = 0;
    int end_index = str_len - 1;
    for (size_t i = 0; i < str_len; i++) {
        if (!isspace(str[i])) {
            start_index = i;
            break;
        }
    }
    for (size_t i = str_len - 1; i >= 0; i--) {
        if (!isspace(str[i])) {
            end_index = i;
            break;
        }
    }
    new_len = end_index - start_index + 1;
    char* p2 = new char[new_len + 1];
    strncpy(p2, str + start_index, new_len);
    p2[new_len] = '\0';
    return p2;
}


int char_count(const char* str, char c) {
    int count = 0;
    while (*str) {
        if (*str == c) {
            count++;
        }
        str++;
    }
    return count;
}

const char* hex(int x) {
    char _hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    if (x < 0) {
        x = -x;
    }
    int i = 0;
    while (x > 0) {
        _hex[i++] = _hex[x % 16];
        x /= 16;
    }
    _hex[i] = 0;
    char* str = (char*)malloc(i + 1);
    for (int j = 0; j < i; j++) {
        str[j] = _hex[i - j - 1];
    }
    str[i] = 0;
    return str;
}

bool endsWith(const char* str, const char* suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (str_len < suffix_len) {
        return false;
    }
    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}


bool startsWith(const char* str, const char* prefix) {
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);
    if (str_len < prefix_len) {
        return false;
    }
    return strncmp(str, prefix, prefix_len) == 0;
}

// const char* concate(const char* str1, const char* str2) {
//     char* str = (char*)malloc(strlen(str1) + strlen(str2) + 1);
//     strcpy(str, str1);
//     strcat(str, str2);
//     return str;
// }

// const char* concat(const char* str1, const char* str2) {
//     char* str = (char*)malloc(strlen(str1) + strlen(str2) + 1);
//     strcpy(str, str1);
//     strcat(str, str2);
//     return str;
// }

template <typename T>
const char* concat(T arg) {
  return toCstring(arg);
}

template <typename T1, typename T2>
const char* concat(T1 arg1, T2 arg2) {
  return concat_const_char(toCstring(arg1), toCstring(arg2));
}

template <typename T1, typename T2, typename T3>
const char* concat(T1 arg1, T2 arg2, T3 arg3) {
  return concat_const_char(concat_const_char(toCstring(arg1), toCstring(arg2)), toCstring(arg3));
}

// ... и так далее для большего количества аргументов

template <typename T1, typename T2, typename T3, typename... Args>
const char* concat(T1 arg1, T2 arg2, T3 arg3, Args... args) {
  return concat_const_char(concat_const_char(concat_const_char(toCstring(arg1), toCstring(arg2)), toCstring(arg3)), concat(args...));
}

int hexToInt(const char* str) {
    int num = 0;
    for (int i = 0; i < (int)strlen(str); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            num = num * 16 + (str[i] - '0');
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            num = num * 16 + (str[i] - 'A' + 10);
        } else if (str[i] >= 'a' && str[i] <= 'f') {
            num = num * 16 + (str[i] - 'a' + 10);
        }
    }
    return num;
}

const char* charToBinary(char chr) {
    char* str = new char[9];
    for (int i = 0; i < 8; i++) {
        str[i] = (chr & 1) ? '1' : '0';
        chr >>= 1;
    }
    str[8] = '\0';
    return str;
}

char binaryToChar(const char* str) {
    char chr = 0;
    for (int i = 0; i < 8; i++) {
        chr <<= 1;
        if (str[i] == '1') {
            chr |= 1;
        }
    }
    return chr;
}

String binaryFormat(byte value) {
    String binary_string = "";
    for (int i = 7; i >= 0; i--) {
        binary_string += ((value >> i) & 1) ? "1" : "0";
    }
    return binary_string;
}

String stringToBinary(String input_string) {
    String binary_string = "";
    for (int i = 0; i < (int)input_string.length(); i++) {
        char c = input_string[i];
        byte ascii_value = (byte)c;
        binary_string += binaryFormat(ascii_value);
        binary_string += " ";
    }
    return binary_string;
}

#endif // __OSLIB_STRINGUTILS_H