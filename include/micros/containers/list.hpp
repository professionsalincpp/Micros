#ifndef __OSLIB_LIST_H
#define __OSLIB_LIST_H

int convertIndex(int index, int size) {
    if (index < 0) {
        index += size + 1;
    }
    return index;
}

template<typename T>
class OList {
public:
    T* data;
    size_t size;
    OList(T* data, size_t size) : data(data), size(size) {}
    OList(size_t size) : data((T*)malloc(sizeof(T) * size)), size(size) {}
    OList() { data = nullptr; size = 0; };
    OList(OList&& other) noexcept {
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }

    OList& operator=(OList&& other) noexcept {
        if (this != &other) {
            delete[] data; 
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }

    OList(const OList& other) {
        data = new T[other.size];
        size = other.size;
        for (size_t i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    }

    OList& operator=(const OList& other) {
        if (this != &other) {
            delete[] data;
            data = new T[other.size];
            size = other.size;
            for (size_t i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }
    ~OList() { free(data); }
    T& operator[](size_t index) const { return data[convertIndex(index, size)]; }
    size_t getSize() const { return size; }
    size_t getRealSize() const { return sizeof(data); }
    size_t append(T item) {
        T* tmp = (T*)malloc(sizeof(T) * (size + 1));
        for (size_t i = 0; i < size; i++) {
            tmp[i] = data[i];
        }
        tmp[size] = item;
        free(data);
        data = tmp;
        size++;
        return size;
    }
    size_t appbegin(T item) {
        T* tmp = (T*)malloc(sizeof(T) * (size + 1));
        for (size_t i = 0; i < size; i++) {
            tmp[i + 1] = data[i];
        }
        tmp[0] = item;
        free(data);
        data = tmp;
        size++;
        return size;
    }
    T* getData() const { return data; }
    T& get(size_t index) const { return data[index]; }
    void set(size_t index, T& item) { data[convertIndex(index, size)] = item; }
    void clear() {
        data = NULL;
        size = 0;
    }
    size_t remove(T item) {
        T* tmp = (T*)malloc(sizeof(T) * (size - 1));
        size_t index = 0;
        for (size_t i = 0; i < size; i++) {
            if (data[i] != item) {
                tmp[index] = data[i];
                index++;
            }
        }
        free(data);
        data = tmp;
        size--;
        return size;
    }
    size_t pop(int index) {
        index = convertIndex(index, size);
        T* tmp = (T*)malloc(sizeof(T) * (size - 1));
        for (size_t i = 0; i < size; i++) {
            if (i < index) {
                tmp[i] = data[i];
            } else if (i > index) {
                tmp[i - 1] = data[i];
            }
        }
        free(data);
        data = tmp;
        size--;
        return size;
    }
    size_t popBack() {
        return pop(-1);
    }
    size_t popFront() {
        return pop(0);
    }
    size_t insert(int index, T item) {
        index = convertIndex(index, size);
        T* tmp = (T*)malloc(sizeof(T) * (size + 1));
        for (int i = 0; i < index; i++) {
            tmp[i] = data[i];
        }
        tmp[index] = item;
        for (size_t i = index; i < size; i++) {
            tmp[i + 1] = data[i];
        }
        free(data);
        data = tmp;
        size++;
        return size;
    }

    int indexOf(T item) {
        for (size_t i = 0; i < size; i++) {
            if (data[i] == item) {
                return i;
            }
        }
        return -1;
    }

    T* begin() { return data; }
    T* end() { return data + size; }

    const T* begin() const { return data; }
    const T* end() const { return data + size; }
};


#endif