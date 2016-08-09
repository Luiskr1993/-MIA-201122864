#ifndef REPORTES_H_INCLUDED
#define REPORTES_H_INCLUDED

#include "disco.h"
//#include "lwh.h"


void do_mbrReport(const char* pathDisco, const char* pathsalida);
void do_diskReport(const char*pathDisco,const char*pathSalida);
void do_inodeReport(const char*pathDisco,const char*pathSalida,PARTITION p);
void do_blockReport(const char*pathDisco,const char*pathSalida,PARTITION p);
void do_bminodeReport(const char*pathDisco,const char*pathSalida,PARTITION p);
void do_bmblockReport(const char*pathDisco,const char*pathSalida,PARTITION p);
void do_treeReport(const char*rutaDisco,PARTITION p,const char*pathSalida, const char*pathDisco);
void do_sbReport(const char*pathDisco,const char*pathSalida,PARTITION p);
void do_fileReport(char *ruta, char *name, char *pathDisco, PARTITION p);
void do_ebrReport(const char* pathDisco, const char*pathsalida);




#endif // REPORTES_H_INCLUDED
