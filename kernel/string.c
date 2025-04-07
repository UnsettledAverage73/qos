#include "string.h"

void int_to_ascii(int n, char str[]) {
    int i = 0, isNegative = 0;

    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (n < 0) {
        isNegative = 1;
        n = -n;
    }

    while (n != 0) {
        int rem = n % 10;
        str[i++] = rem + '0';
        n /= 10;
    }

    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';
    reverse(str, i);
}

void hex_to_ascii(int n, char str[]) {
    int i = 0;
    str[i++] = '0';
    str[i++] = 'x';

    int started = 0;
    for (int j = 28; j >= 0; j -= 4) {
        int digit = (n >> j) & 0xF;
        if (!started && digit == 0) continue;
        started = 1;
        str[i++] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
    }

    if (!started)
        str[i++] = '0';

    str[i] = '\0';
}

void reverse(char* str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char tmp = str[start];
        str[start++] = str[end];
        str[end--] = tmp;
    }
}

int strlen(const char* str) {
    int len = 0;
    while (str[len]) len++;
    return len;
}

void* memcpy(void* dest, const void* src, int count) {
    char* dst = (char*)dest;
    const char* srcp = (const char*)src;
    for (int i = 0; i < count; i++)
        dst[i] = srcp[i];
    return dest;
}

void* memset(void* dest, int val, int count) {
    unsigned char* ptr = (unsigned char*)dest;
    for (int i = 0; i < count; i++)
        ptr[i] = (unsigned char)val;
    return dest;
}

int strcmp(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] && str2[i]) {
        if (str1[i] != str2[i])
            return str1[i] - str2[i];
        i++;
    }
    return str1[i] - str2[i];
}

void strcpy(char* dest, const char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

void strcat(char* dest, const char* src) {
    while (*dest) dest++;
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}
