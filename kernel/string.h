#ifndef STRING_H
#define STRING_H

void int_to_ascii(int n, char str[]);
void hex_to_ascii(int n, char str[]);
void reverse(char* str, int length);
int strlen(const char* str);
void* memcpy(void* dest, const void* src, int count);
void* memset(void* dest, int val, int count);
int strcmp(const char* str1, const char* str2);
void strcpy(char* dest, const char* src);
void strcat(char* dest, const char* src);

#endif
