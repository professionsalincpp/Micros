#ifndef __FUNCUTILS_H
#define __FUNCUTILS_H

#define InitList(list, size) T* list = new T[size];

#define enumerate(list) for (int i = 0; i < list.getSize(); i++)

extern int config();
static bool __configured = false;

void configIfNot() {
    if (!__configured) {
        config();
        __configured = true;
    }
}


#endif // __FUNCUTILS_H