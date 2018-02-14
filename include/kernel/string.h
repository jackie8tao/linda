#ifndef __LINDA_STRING_H
#define __LINDA_STRING_H

#include <types.h>

char * ___strtok;
char * strcpy(char *,const char *);
char * strncpy(char *,const char *, size_t);
char * strcat(char *, const char *);
char * strncat(char *, const char *, size_t);
char * strchr(const char *,int);
char * strrchr(const char *,int);
char * strpbrk(const char *,const char *);
char * strtok(char *,const char *);
char * strstr(const char *,const char *);
size_t strlen(const char *);
size_t strnlen(const char *,size_t);
size_t strspn(const char *,const char *);
int strcmp(const char *,const char *);
int strncmp(const char *,const char *,size_t);

void * memset(void *, char, size_t);
void * memcpy(void *, const void *, size_t);
void * memmove(void *, const void *, size_t);
void * memscan(void *, int, size_t);
int memcmp(const void *, const void *, size_t);

#endif