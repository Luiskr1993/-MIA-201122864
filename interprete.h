#ifndef INTERPRETE_H_INCLUDED
#define INTERPRETE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include "disco.h"
#include "cadena.h"
#include "lwh.h"
#include "reportes.h"

///definiciones

#define mkdisk
#define rmdisk
#define fdisk
#define mkfs
#define mount
#define unmount

#define mkfile
#define cat
#define rem
#define edit
#define ren
#define mkdir
#define cp
#define mv
#define find

#define rep
#define exec

#define MAX_MONTADOS 25
typedef struct _muntStruct{
    char status;
    char path[200];
    char nombre[16];
    char letra;
    int numero;
    PARTITION part;
}PARTITION_MOUNTED;

PARTITION_MOUNTED Montados[MAX_MONTADOS];



char bufferComando[250];
char bufferParametros[10][100];

char PARAM_COMMAND[10];///nombre del commando a utilizar : mkdir...
int PARAM_size;///positivo mayor que cero para mkdisk,fdisk. =>cero para: mkfile,edit
char PARAM_unit;
char PARAM_path[100];
char PARAM_type [5];
char PARAM_fit[5];
char PARAM_delete[6];
char PARAM_name[25];
int PARAM_add;
int PARAM_addValue;
char PARAM_id [10];
int PARAM_p;
char PARAM_cont[100];
char PARAM_dest [100];
char PARAM_iddest [100];
char PARAM_file [100];
char comentario[100];


//typedef char Parametro[100];
//Parametro bufferParametros[10];

void do_mkdisk();
void do_rmdisk();
void do_fdisk(char action);
//void do_mkfs(char action);
void do_mount();
void do_unmount();

/***Metodos de manejo de archivos***/

/*
void do_mkfile();
void do_cat();
void do_rem();
void do_edit();
void do_ren();
void do_mkdir();
void do_cp();
void do_mv();
void do_find();
void do_rep();
void do_exec();
*/

void InicializarMontados();
void MontarParticion(const char* pathdisco, const char* nombreparticion);
void DesMontarParticion(const char* id);
void ImprimirMontados();
int BuscarEnMontados(const char* id);

void ResetParam();
int ReadConsole(char* mensajeSalida, char* Destino,size_t sz);
void Limpiar();
void interpretarComando();
void interpretarComentario();
void Console();
int mainMenu1();



#endif // INTERPRETE_H_INCLUDED
