#include "mem.h"

int strlen(char* str) {
    int i;
    for (i = 0; str[i] != 0; i++);

    return i;
}

int strset(char* str, char c) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = c;
    }
    
    return 0;
}

int memcmp(char* a, char* b, int size) {
    for (int i = 0; i < size; i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }

    return 1;
}

int memcpy(char* src, char* dst, int size) {
    for(int i = 0; i < size; i++) {
        dst[i] = src[i];
    }

    return 0;
}

int memset(char* src, char c, int size) {
    for (int i = 0; i < size; i++) {
        src[i] = c;
    }

    return 0;
}

int strcmp(char* a, char* b) {
    for (int i = 0; i < strlen(a); i++) {
        if (a[i] != b[i]) {
            return 0;
        }
    }

    return 1;
}

int strcpy(char* src, char* dst) {
    for(int i = 0; i < strlen(src); i++) {
        dst[i] = src[i];
    }

    return 0;
}
