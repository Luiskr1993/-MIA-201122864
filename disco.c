#include "disco.h"
void WriteMBR(const char*diskPath, MBR m){
    //printf("path disco: %s\n",diskPath);
    FILE* disk=fopen(diskPath,"r+b");
    if(disk!=NULL){
        fseek(disk,0,SEEK_SET);
        fwrite(&m,sizeof(MBR),1,disk);
        fclose(disk);
    }else{
        printf("Disco no encontrado. Error ocurrido en escribir MBR.\n");
    }
};

void WriteEBR(const char*diskPath, EBR e){
    FILE* disk=fopen(diskPath,"r+b");
    if(disk!=NULL){
        fseek(disk,e.part_start,SEEK_SET);
        fwrite(&e,sizeof(EBR),1,disk);
        fclose(disk);
    }else{
        printf("Disco no encontrado. Error ocurrido en escribir MBR.\n");
    }
};
MBR ReadMBR(const char*diskPath){
    MBR mbr;
    mbr.mbr_tamano=-1;
    FILE* disk=fopen(diskPath,"r+b");
    if(disk!=NULL){
        fseek(disk,0,SEEK_SET);
        fread(&mbr,sizeof(MBR),1,disk);
        fclose(disk);
    }else{
        printf("Disco no encontrado. Error ocurrido en leer MBR.\n");
    }
    return mbr;
};

EBR ReadEBR(const char*diskPath){
    EBR ebr;
    ebr.part_size = -1;
    FILE* disk=fopen(diskPath,"r+b");
    if(disk!=NULL){
        fseek(disk,0,SEEK_SET);
        fread(&ebr,sizeof(EBR),1,disk);
        fclose(disk);
    }else{
        printf("Particion no encontrada. Error ocurrido en leer EBR.\n");
    }
    return ebr;
};
void CrearDisco(const char* path, int size,int unit){
    int i;
    char c[1024];
    long escritos=0;
    //c[0]='\0';
    char* parent=getParentPath(path);

    system(CCAT("mkdir -m a=rwx -p ",parent));//para crear la carpeta que no exista en el path introducido por el usuario
    FILE* nuevoDisco=NULL;
    nuevoDisco=fopen(path,"wb");
    if(nuevoDisco!=NULL){
        //fseek(nuevoDisco,0,SEEK_SET);
        printf("Creando disco...\n");
        for(i=0;i<size*unit/KB;i++){
            escritos+=fwrite(&c,sizeof(c),1,nuevoDisco);
        }
        fclose(nuevoDisco);
        ////////////////Añadiendo MBR///////////////////

//    PARTITION mbr_partition[4];
        printf("Agregando MBR...\n");
        MBR mbr;
        mbr.mbr_tamano=size*unit;
        mbr.mbr_fecha_creacion=time(0);

        int signatureRandom = time(NULL);

                    srand(signatureRandom+1);
                    int numero = rand() % 10000000 + 1000000; //numero aleatorio entre cero y 5

        mbr.mbr_disk_signature=numero;

        int i;
        PARTITION basura={LIBRE,'P','B',-1,-1,""};
        for (i=0;i<4;i++){
            mbr.mbr_partition[i] =basura;
        }
        WriteMBR(path, mbr);

        /////////////////////////////////////////////////
        printf("Disco Creado Exitosamente! (%ld/%d KB escritos corretamente)\n",escritos,unit*size/KB);
        ///printf("... ... ... ... ... ... ... ...\n");

        return;
    }
    printf("Error al crear disco!");
};
void EliminarDisco(const char* path){
    char result[2];
    FILE* discoEliminar=fopen(path,"rb");
    if(discoEliminar==NULL){
        printf("Error. El disco: %s, no existe",path);
    }else{
        fclose(discoEliminar);
        printf("Esta a punto de eliminar el disco: %s.\n Desea Continuar?(Y/N)",path);
        //char result=getchar();
        gets(result);
        if(result[0]=='y'||result[0]=='Y'){
            remove(path);
            printf("Disco eliminado.\n");
        }else{printf("Operacion cancelada.\n");}
    }
};

int getNumPart(MBR mbr,char p_type){
    int n=0;
    int i;
    for(i=0;i<4;i++)
        if(mbr.mbr_partition[i].part_type==p_type&&mbr.mbr_partition[i].part_status==OCUPADO)
            n++;
    return n;
};


int BuscarAjusteParticion(MBR mbr,int TAMNecesario){
    int r=-1;
    int cotaInf=sizeof(MBR);

    int cotaSup=mbr.mbr_tamano;
    int i;

    PARTITION partActual;

    for(i=0;i<4;i++){
        partActual=mbr.mbr_partition[i];
        if(partActual.part_status==OCUPADO){
            if(partActual.part_start-cotaInf>=TAMNecesario)return cotaInf;
            else{
                cotaInf=partActual.part_start+partActual.part_size;
            }
        }
    }
    if(cotaSup-cotaInf>=TAMNecesario)return cotaInf;
    else return-1;
    return r;
};

PARTITION BuscarParticion(const char* pathdisco, const char* nombreParticion){
    PARTITION p;
    p.part_size=-1;
    MBR mbr=ReadMBR(pathdisco);

            if(mbr.mbr_tamano==-1){
                printf("Error en la busqueda de la particion. No se puede acceder al disco.\n");
            }else{
                int i;
                PARTITION tempPart;
                for(i=0;i<4;i++){
                    tempPart=mbr.mbr_partition[i];
                    if(strcmp(tempPart.part_name,nombreParticion)==0&&tempPart.part_status==OCUPADO){
                        return tempPart;
                    }
                }
                printf("Particion -%s- no encontrada en el disco -%s-.\n",nombreParticion,pathdisco);
            }

    return p;
};
int getParticionIndex(const char* pathdisco, const char* nombreParticion){
    int r=-1;
    MBR mbr=ReadMBR(pathdisco);
            if(mbr.mbr_tamano==-1){
                printf("Error en la busqueda de la particion. No se puede acceder al disco.\n");
            }else{
                int i;
                PARTITION tempPart;
                for(i=0;i<4;i++){
                    tempPart=mbr.mbr_partition[i];
                    if(strcmp(tempPart.part_name,nombreParticion)==0&&tempPart.part_status==OCUPADO){
                        return i;
                    }
                }
                printf("Particion -%s- no encontrada en el disco -%s-.\n",nombreParticion,pathdisco);
            }
    return r;
};

void AgregarParticion(const char* path,PARTITION p){
    MBR mbr=ReadMBR(path);
    if(mbr.mbr_tamano==-1)return;

    PARTITION abuscar=BuscarParticion(path,p.part_name);
    if(abuscar.part_size!=-1){printf("Ya existe una particion con el nombre: %s.\n",p.part_name);return;}
    //int ajuste=BuscarAjusteParticion(mbr,p.part_size);
    //if(ajuste==-1){printf("Error al crear la particion. no se encontro ajuste.");return;}

            /**Validando la creacion de la particion**/
            int numPrimarias=getNumPart(mbr,'P');
            int numExtendidas=getNumPart(mbr,'E');
            switch(p.part_type){
                case 'P':
                    printf("Creando Particion Primaria...\n");

                    if(numPrimarias+numExtendidas<4){
                        //inesertarParticion primaria
                        int byteDeAjuste=BuscarAjusteParticion(mbr,p.part_size);
                        printf("ajuste en:%d\n",byteDeAjuste);
                        if(byteDeAjuste<0){
                            printf("No se puede crear particion Primaria. No se encontro ajuste. \n");
                            return;
                        }else{
                            p.part_start=byteDeAjuste;

                            ///adjutando en el arreglo
                            int i;
                            for(i=0;i<4;i++){
                               if(mbr.mbr_partition[i].part_status==LIBRE){
                                    mbr.mbr_partition[i]=p;
                                    break;
                               }
                            }
                            ///reordenando el arreglo
                            PARTITION temp;
                            int n=numPrimarias+numExtendidas+1;///mas uno por la nueva
                       //     printf("n:%d\n",n);
                            int j;
                            for(i=1;i<n;i++){
                                printf("i:%d\n",i);
                                for(j=0;j<n-i;j++){
                         //           printf("j:%d\n",j);
                         //           printf("%d>%d?",mbr.mbr_partition[j].part_start,mbr.mbr_partition[j+1].part_start);
                                    if(mbr.mbr_partition[j].part_start>mbr.mbr_partition[j+1].part_start){
                         //               printf("%d>%d ok",mbr.mbr_partition[j].part_start,mbr.mbr_partition[j+1].part_start);
                                        temp=mbr.mbr_partition[j];
                                        mbr.mbr_partition[j]=mbr.mbr_partition[j+1];
                                        mbr.mbr_partition[j+1]=temp;
                                    }
                                }
                            }
                            printf("Actualizando MBR...\n");
                            WriteMBR(path,mbr);
                            printf("Creacion concluida.\n");
                        }
                    }else{
                        printf("No es posiblie Crear la particion Primaria.La cantidad de particiones primarias y extendidas en un disco no debe ser mayor a 4.\n");
                    }
                break;
                case 'L':
                   printf("Aun no se implementa el metodo para crear particiones logicas\n");
                break;
                case 'E':
                    printf("Creando Particion Extendida...\n");

                    if(numExtendidas>0){printf("No se puede crear la particion extendida. Ya existe una.\n");return;}
                    if(numPrimarias+numExtendidas<4){
                        //inesertarParticion primaria
                        int byteDeAjuste=BuscarAjusteParticion(mbr,p.part_size);
                        printf("ajuste en:%d\n",byteDeAjuste);
                        if(byteDeAjuste<0){
                            printf("No se puede crear particion Extendida. No se encontro ajuste. \n");
                            return;
                        }else{
                            p.part_start=byteDeAjuste;

                            ///adjutando en el arreglo
                            int i;
                            for(i=0;i<4;i++){
                               if(mbr.mbr_partition[i].part_status==LIBRE){
                                    mbr.mbr_partition[i]=p;

                                    break;
                               }
                            }
                            ///reordenando el arreglo
                            PARTITION temp;
                            int j;
                            for(j=numPrimarias+numExtendidas-1;j<=0;j--){
                                for(i=0;i<j;i++){
                                    if(mbr.mbr_partition[i].part_start>mbr.mbr_partition[i+1].part_start){
                                        temp=mbr.mbr_partition[i+1];
                                        mbr.mbr_partition[i+1]=mbr.mbr_partition[i];
                                        mbr.mbr_partition[i]=temp;
                                    }
                                }
                            }

                            ///Creando el EBR inicial para la particion extendida
                            printf("Agregando EBR...\n");
                            EBR ebr;
                            ebr.part_start = p.part_start;
                            ebr.part_next = 00000;
                            ebr.part_size = p.part_size;
                            ebr.part_status = p.part_status;
                            ebr.part_fit = p.part_fit;
                            //ebr.part_name = p.part_name;


                            WriteEBR(path, ebr);

                            /////////////////////////////////////////////////
        printf("EBR agregado correctamente en la Particion Extendida: &c \n");

                            printf("Actualizando MBR...\n");
                            WriteMBR(path,mbr);
                            printf("Creacion concluida.\n");
                        }
                    }else{
                        printf("No es posiblie Crear la particion Extendida. La cantidad de particiones primarias y extendidas en un disco no debe ser mayor a 4.\n");
                    }
                break;

            }


};

void EliminarParticion(const char*path, const char* nombreParticion,char mode){
    char result[10];
    int partIndex=getParticionIndex(path,nombreParticion);
    if(partIndex==-1){
        printf("Error al eliminar la particion, no se encontro: %s.\n",nombreParticion);
        return;
    }
    //ReadConsole("Particion encontrada, desea eliminarla? [Y/N].\n",result,9);
    printf("%s encontrada, desea eliminartla? [Y/N].\n",nombreParticion);
    fgets(result,9,stdin);
    if(result[0]!='y'&&result[0]!='Y'){return;}
    MBR mbr=ReadMBR(path);
    mbr.mbr_partition[partIndex].part_status=LIBRE;
    if(mode==COMPLETO){
        printf("Aplicando eliminacion completa...\n");
        FILE*disco=fopen(path,"r+b");
        fseek(disco,mbr.mbr_partition[partIndex].part_start,SEEK_SET);

        char buff='\0';
        fwrite(&buff,sizeof(char),mbr.mbr_partition[partIndex].part_size,disco);

        fclose(disco);
    }
    ///reordenando tabla
    WriteMBR(path,mbr);
    printf("Filanzo la eliminacion de: %s.\n",nombreParticion);
};


void MEspacioParticion(const char*path, const char* nombreParticion, int size){
    int partIndex=getParticionIndex(path,nombreParticion);
    if(partIndex==-1){
        printf("Error al modificar la particion, no se encontro: %s.\n",nombreParticion);
        return;
    }
    MBR mbr=ReadMBR(path);
    if(size<0){
        printf("Intentando reducir: %d bytes.\n",size);
        int finalsize=mbr.mbr_partition[partIndex].part_size+size;///le puse un + porque ya viene negativo
        if(finalsize<0){
            printf("Imposible reducir el espacio de la particion. El tamaño final resulta negativo.\n");
        }else{
            mbr.mbr_partition[partIndex].part_size=finalsize;
            WriteMBR(path,mbr);
            printf("Tamaño reducido a: %d.\n",finalsize);
        }
    }else if(size>0){
        printf("Intentando aumentar: %d bytes.\n",size);
        int finalsize=mbr.mbr_partition[partIndex].part_size+size;
        ///bucando espacio disponible
        int i;
        int cotaSup=-1;
        for(i=partIndex+1;i<4;i++){
            if(mbr.mbr_partition[i].part_status==OCUPADO){
                cotaSup=mbr.mbr_partition[i].part_start;
                break;
            }
        }
        if(cotaSup==-1){///por si es la ultima particion
            cotaSup=mbr.mbr_tamano;
        }
        ///validando apliacion:
        //printf("TamPart=%d.\n",mbr.mbr_tamano);
        //printf("cotasup=%d.\n",cotaSup);
        //printf("p_start=%d.\n",mbr.mbr_partition[partIndex].part_start);
        //printf("final size=%d.\n",finalsize);

        if(cotaSup-mbr.mbr_partition[partIndex].part_start<finalsize){
            printf("Imposible aumentar el espacio de la particion. No hay suficiente espacio.\n");
        }else{
            mbr.mbr_partition[partIndex].part_size=finalsize;
            WriteMBR(path,mbr);
            printf("Tamaño aumentado a: %d.\n",finalsize);
        }
    }
};
void PrintParticiones(MBR mbr){
    printf("Particiones:\n");
    int t;
    for (t=0;t<4;t++){
        if(mbr.mbr_partition[t].part_status==OCUPADO)
        printf("%s  %d  tam:%d\n",mbr.mbr_partition[t].part_name,mbr.mbr_partition[t].part_start,mbr.mbr_partition[t].part_size);
    }
};





