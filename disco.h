#ifndef DISCO_H_INCLUDED
#define DISCO_H_INCLUDED

#define FORMATO_COMPLETO 1
#define FORMATO_RAPIDO 0

#define RAPIDO 'R'
#define COMPLETO 'C'

#define ELIMINADO 0
#define ACTIVO 1

#define OCUPADO '1'
#define LIBRE '0'

#define KB 1024
#define MB 1024*1024

#define PRIMARIA 1
#define EXTENDIDA 2
#define LOGICA 3

#define PRIMER_AJUSTE 1
#define MEJOR_AJUSTE 2
#define PEOR_AJUSTE 3

#include <math.h>
#include "cadena.h"




typedef struct _partition{
    char part_status; //si la particion esta activa
    char part_type;//si es primaria (P) o extendida (E)
    char part_fit; //ajuste B (Best), F (First) o W (worst)
    int part_start;
    int part_size;
    char part_name[16];
}PARTITION;



typedef struct _mbr{
    int mbr_tamano;//tam del disco
    time_t mbr_fecha_creacion;// fecha de creacion
    int mbr_disk_signature;///identificador de disco
    PARTITION mbr_partition[4];
}MBR;

typedef struct _ebr{
    char part_status;/// '0' o '1'
    char part_fit;///B (Best), F (First) o W (worst)
    int part_start;
    int part_size;
    int part_next;
    char part_name[16];
}EBR;

void CrearDisco(const char* path, int size, int unit);
void WriteMBR(const char*diskPath, MBR m);
void WriteEBR(const char*diskPath, EBR e);
MBR ReadMBR(const char*diskPath);
EBR ReadEBR(const char*diskPath);
void EliminarDisco(const char* path);
///particiones
PARTITION BuscarParticion(const char* pathdisco, const char* nombreParticion);
int getParticionIndex(const char* pathdisco, const char* nombreParticion);
void AgregarParticion(const char* path,PARTITION p);
int BuscarAjusteParticion(MBR mbr, int TAMNecesario);
int getNumPart(MBR mbr,char p_type);
void EliminarParticion(const char*path, const char* nombreParticion, char mode);
void MEspacioParticion(const char*path, const char* nombreParticion, int size);
void PrintParticiones(MBR mbr);






char* getRutaIndex(const char* carpeta);
char* getRutaDisco(const char* carpeta,const char* nombreDisco);

void AumentarDisco(const char* Carpeta, const char* id_disk,int SIZE_MB);
void EscribirEnDisco(const char* ruta,int sizeMB);


void ReducirDisco(const char* Carpeta, const char* id_disk,int TAM_MB);
void ImprimirDiscos(const char* Carpeta);
char* getNewID(const char* old_ID);
int BuscarDisco(const char* directorioIndex,const char* idDisco);
///*DISCO getDisco(const char* ArchivoIndex,const char* idDisco);




/*********manejo de particiones************/

void AgregarParticion2(const char* carpeta, const char* idDisco,PARTITION p);
//int getNumBloques(int TamParticionBytes, int TamBloques);
int BuscarAjuste(const char * pathDisco,PARTITION p);
int BuscarAjusteLogico(const char * pathDisco,PARTITION p);

void EliminarParticion2(const char*carpeta, const char* idDisco, const char* nombreParticion);
void FormatearParticion(const char*carpeta, const char* idDisco, const char* nombreParticion, char FormatoCompleto);




int getFileSize(char * pathArchivo);



#endif // DISCO_H_INCLUDED
