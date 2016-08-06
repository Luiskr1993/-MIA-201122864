#ifndef CADENA_H_INCLUDED
#define CADENA_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
char* CCAT(const char* a,const char*b);
char* CaCAT(const char* a,const  char b);
char getLast(const char* cad);
char* subString(const char*cad,int start,int length);
char* CaCAT2(const char* a,const char b);

int toInt(const char* cad);

char* getParentPath(const char*fullPath);
char* fileName(const char*fullPath);
char* time_t_to_string(time_t t);

#endif // CADENA_H_INCLUDED
