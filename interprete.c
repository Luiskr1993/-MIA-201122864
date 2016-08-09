#include "interprete.h"



void do_mkdisk(){
     int error=0;
     int unidad=MB;
     if(PARAM_size<1){printf("ERROR[mkdisk]. Se necesita parametro -size, Debe ser positivo y mayor que cero.\n");error=1;}
     if(strcmp(PARAM_path,"")==0){printf("ERROR[mkdisk]. Falta el parmetro -path.\n");error=1;}
     if(PARAM_unit!='\0'){
        if(PARAM_unit=='k'||PARAM_unit=='K')unidad=KB;
        else if(PARAM_unit=='m'||PARAM_unit=='M')unidad=MB;
        else{
            printf("ERROR[mkdisk]. El parametro opcional -unit solo adminte 'k'|'K'|'m'|'M'.\n");error=1;

        }
     }

     if(!error){
        ///hacer...
        CrearDisco(PARAM_path,PARAM_size,unidad);
     }
};


void do_rmdisk(){
    if(strcmp(PARAM_path,"")==0){
        printf("ERROR[rmdisk]. Falta el parametro -path.\n");
    }else{
        EliminarDisco(PARAM_path);
    }
};

void do_fdisk(char action){
    int unidad=KB;
    int error=0;
    char tipoParticion='P';
    char tipoAjuste='W';
    char tipoDel;
    ///parametro unit
    if(PARAM_unit!='\0'){
        if(PARAM_unit=='k'||PARAM_unit=='K')unidad=KB;
        else if(PARAM_unit=='m'||PARAM_unit=='M')unidad=MB;
        else if(PARAM_unit=='b'||PARAM_unit=='B')unidad=1;
        else{
            printf("ERROR. El parametro opcional -unit solo adminte |'b'|'B'|'k'|'K'|'m'|'M'.\n");error=1;
        }
    }
    ///para el parametro size
    if(PARAM_delete==0&&PARAM_add==0){
        if(PARAM_size<1){printf("ERROR. Para crear se necesita parametro -size, Debe ser positivo y mayor que cero.\n");error=1;}
    }
    ///path
    if(strcmp(PARAM_path,"")==0){printf("ERROR. Falta el parmetro -path.\n");error=1;}
    ///type
    if(strcmp(PARAM_type,"")!=0){
        if(strcmp(PARAM_type,"P")==0)tipoParticion='P'; //Particion Primaria
        else if(strcasecmp(PARAM_type,"E")==0)tipoParticion='E'; //Particion Extendida
        else if(strcasecmp(PARAM_type,"L")==0)tipoParticion='L'; //Particion Logica
        else{
            printf("ERROR. El parametro opcional -type solo adminte 'P'|'E'|'L'.\n");error=1;
        }
    }
    ///fit
    if(strcmp(PARAM_fit,"")!=0){
        if(strcasecmp(PARAM_fit,"B")==0)tipoAjuste='B'; //Best Fit
        else if(strcasecmp(PARAM_fit,"F")==0)tipoAjuste='F'; //First Fit
        else if(strcasecmp(PARAM_fit,"F")==0)tipoAjuste='W'; //Worst Fit
        else{
            printf("ERROR. El parametro opcional -fit solo adminte 'B'|'F'|'W'.\n");error=1;
        }
    }
    ///name
    if(strcmp(PARAM_name,"")==0){printf("ERROR. Falta el parmetro -name.\n");error=1;}
    ///delete
    if(strcmp(PARAM_delete,"")!=0){
        if(strcasecmp(PARAM_delete,"fast")==0)tipoDel=RAPIDO;
        else if(strcasecmp(PARAM_delete,"full")==0)tipoDel=COMPLETO;
        else{
            printf("ERROR. El parametro opcional -delete solo adminte 'fast'|'full'.\n");error=1;
        }
    }

    ///operacion co  la particion
    PARTITION p;
    p.part_status=OCUPADO;

    p.part_type=tipoParticion;
    p.part_fit=tipoAjuste; //ajuste B (Best), F (First) o W (worst)
    p.part_start=-1;
    p.part_size=PARAM_size*unidad;
    strcpy(p.part_name,PARAM_name);
    if(error==1)return;
    switch(action){
        case 'S':
            AgregarParticion(PARAM_path,p);
        break;
        case 'D':
            ///eliminar particion
            EliminarParticion(PARAM_path,PARAM_name,tipoDel);
        break;
        case 'A':
            ///cambiartamanoparticion
            MEspacioParticion(PARAM_path,PARAM_name,PARAM_addValue*unidad);
        break;
    }

};



void do_mount(){
    int error=0;
    ///path
    if(strcmp(PARAM_path,"")==0){printf("ERROR. Falta el parmetro -path.\n");error=1;}
    ///name
    if(strcmp(PARAM_name,"")==0){printf("ERROR. Falta el parmetro -name.\n");error=1;}
    if(!error){
        ///montar
        MontarParticion(PARAM_path,PARAM_name);
    }
};

void do_unmount(){
    if(strcmp(PARAM_id,"")==0){
        printf("ERROR. Falta el parmetro -id.\n");
    }else{
        ///desmontar
        DesMontarParticion(PARAM_id);
    }
};



void do_rep(){
    int error=0;
    ///id
    if(strcmp(PARAM_id,"")==0){printf("ERROR. Falta el parmetro -id.\n");error=1;}
    ///path
    if(strcmp(PARAM_path,"")==0){printf("ERROR. Falta el parmetro -path.\n");error=1;}
    ///name
    if(strcmp(PARAM_name,"")==0){printf("ERROR. Falta el parmetro -name.\n");error=1;}

    ///file
    //if(strcmp(PARAM_file,"")==0){printf("ERROR. Falta el parmetro -file.\n");error=1;}

    if(error==0){
        ///hacer
        int partindex=BuscarEnMontados(PARAM_id);
        if(partindex==-1){printf("La particion no esta montada en el sistem .S.\n");return;}
        PARTITION_MOUNTED part=Montados[partindex];

        if(strcasecmp(PARAM_name,"mbr")==0)do_mbrReport(part.path,PARAM_path);
        else if(strcasecmp(PARAM_name,"disk")==0)do_diskReport(part.path,PARAM_path);
        else if(strcasecmp(PARAM_name, "journaling")==0)do_ebrReport(part.path, PARAM_path);
        else if(strcasecmp(PARAM_name,"inode")==0);
        else if(strcasecmp(PARAM_name,"block")==0);
        else if(strcasecmp(PARAM_name,"bm_inode")==0);
        else if(strcasecmp(PARAM_name,"bm_block")==0);
        else if(strcasecmp(PARAM_name,"tree")==0);
        else if(strcasecmp(PARAM_name,"sb")==0);
        else if(strcasecmp(PARAM_name,"file")==0);
        else if(strcasecmp(PARAM_name,"Ls +i")==0);
        else if(strcasecmp(PARAM_name,"Ls +l")==0);
           else{
               printf("El parametro path solo acepta: 'mbr', 'ebr', 'disk', 'inode', 'block', 'bm_inode', 'mb_block', 'tree', 'sb' y 'file'.\n");
               error=1;
           }

    }
};
void do_exec(){
    if(strcmp(PARAM_path,"")==0){
        printf("ERROR. Falta el parmetro -path.\n");
    }else{
        ///hacer
        char pausa='0';
        char result[10];

        FILE* script=fopen(PARAM_path,"r");
        if(script==NULL){printf("Error el archivo de Script no existe.\n");return;}
        ReadConsole("Desea ralizar pausas entre instrucciones? [Y/N].\n-->",result,9);
        if(result[0]=='y'||result[0]=='Y')pausa='1';

        //fscanf (script, "%s", bufferComando);
        fscanf (script, "%[^\n] \n", bufferComando);
       do  {
            printf("%s\n",bufferComando);
            if(bufferComando[0]!='#'){interpretarComando();}
            fscanf (script, "%[^\n] \n", bufferComando);
            //fscanf (script, "%s", bufferComando);
            if(pausa=='1')ReadConsole("",result,9);
        }while ( ! feof (script) );


        fclose(script);
    }
};

void ResetParam(){
        strcpy(PARAM_COMMAND,"");

        PARAM_unit='\0';
        strcpy(PARAM_path,"");
        strcpy(PARAM_type,"");
        strcpy(PARAM_fit,"");
        //PARAM_delete=0;
        strcpy(PARAM_delete,"");
        strcpy(PARAM_name,"");
        PARAM_add=0;
        PARAM_addValue=0;
        strcpy(PARAM_id,"");
        PARAM_p=0;
        PARAM_size=-1;
        strcpy(PARAM_cont,"");
        strcpy(PARAM_dest,"");
        strcpy(PARAM_iddest,"");
        strcpy(PARAM_file,"");
}
int ReadConsole(char* mensajeSalida, char* Destino,size_t sz){
//    int curentChar;
    if (mensajeSalida!=NULL){
        printf("%s",mensajeSalida);
        fflush(stdout);
    }
    if(fgets(Destino,sz,stdin)){
        Destino[strlen(Destino)-1]='\0';

        fflush(stdout);

        return 0;
    }
    /*if( Destino[strlen(Destino)-1]!='\n'){
        curentChar=getchar();
        while((curentChar!='\n')&&(curentChar!=EOF)){

        }
    }*/
    return 1;
};
void Limpiar(){
    system("clear");
    printf(".---------------------------------------------------------------------------.\n");
    printf("|                      Manejo e implementacion de Archivos.                  |\n");
    printf("|                           Segundo Semestre 2016.                           |\n");
    printf("|                        Luis Carlos Valiente Salazar                        |\n");
    printf(".---------------------------------------------------------------------------.\n");
};
void interpretarComando(){
    ResetParam();
    char tipoOperacion='0';
    int Param=0;
    char*token;
    token = strtok(bufferComando, " ");


    if(strcasecmp(token, "#")==0){//Validación para comentarios

        interpretarComentario();

        return;
    }

    while( token != NULL &&Param<10)
    {
        strcpy(bufferParametros[Param],token);
        token = strtok(NULL, " ");
        Param++;
    }

    if(Param<=0)return;
    strcpy(PARAM_COMMAND,bufferParametros[0]);
    int max=Param;
    char ActualParam[250];
    char* paramVal;




    ///separando valores de parametros
    for(Param=1;Param<max;Param++){
        //if(bufferParametros[Param][0]!='\0')
        //printf("%s\n",bufferParametros[Param]);
        strcpy(ActualParam,bufferParametros[Param]);
        token = strtok(ActualParam, ":");
        paramVal = strtok(NULL, ":");



    ///almacenando los valores de cada parametro encontrado...
        if(strcasecmp(token,"-size")==0){
            if(paramVal!=NULL)
            PARAM_size=toInt(paramVal);//atoi(token);
            //PARAM_size=0;
            if(tipoOperacion=='0')
            tipoOperacion='S';
        }
        else if(strcasecmp(token,"+unit")==0){
            if(paramVal!=NULL)
            PARAM_unit=paramVal[0];
        }
        else if(strcasecmp(token,"-path")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_path,paramVal);
        }
        else if(strcasecmp(token,"+type")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_type,paramVal);
        }
        else if(strcasecmp(token,"+fit")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_fit,paramVal);
        }
        else if(strcasecmp(token,"+delete")==0){
            //PARAM_delete=1;
            if(paramVal!=NULL)
            strcpy(PARAM_delete,paramVal);
            if(tipoOperacion=='0')
            tipoOperacion='D';
        }
        else if(strcasecmp(token,"-name")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_name,paramVal);
        }
        else if(strcasecmp(token,"+add")==0){
            if(paramVal!=NULL){
                PARAM_add=1;
                PARAM_addValue=toInt(paramVal);//atoi(strtok(NULL,"="));
            }
            if(tipoOperacion=='0')
            tipoOperacion='A';
        }
        else if(strcasecmp(token,"-id")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_id,paramVal);
        }
        else if(strcasecmp(token,"-p")==0){
            if(paramVal==NULL)
            PARAM_p=1;
            else
            PARAM_p=-1;///indica que se le asigno valor en la entrada
        }
        else if(strcasecmp(token,"-cont")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_cont,paramVal);
        }
        else if(strcasecmp(token,"-dest")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_dest,paramVal);
        }
        else if(strcasecmp(token,"-iddest")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_iddest,paramVal);
        }
        else if(strcasecmp(token,"-file")==0){
            if(paramVal!=NULL)
            strcpy(PARAM_iddest,paramVal);
        }
        else if(strcasecmp(token, "\"")==0){
            printf("Espacio Detectado %s. \n", token);
        }

        else{


                printf("Error. No se reconoce el parametro: %s.\n",token);
                return;
        }
    }
    ///evaluando cada comando (con sus errores)

    if(strcasecmp(PARAM_COMMAND,"mkdisk")==0)do_mkdisk();
    else if(strcasecmp(PARAM_COMMAND,"rmdisk")==0)do_rmdisk();
    else if(strcasecmp(PARAM_COMMAND,"fdisk")==0)do_fdisk(tipoOperacion);
    else if(strcasecmp(PARAM_COMMAND,"mkfs")==0);
    else if(strcasecmp(PARAM_COMMAND,"mount")==0)do_mount();
    else if(strcasecmp(PARAM_COMMAND,"unmount")==0)do_unmount();
    else if(strcasecmp(PARAM_COMMAND,"mkfile")==0);
    else if(strcasecmp(PARAM_COMMAND,"cat")==0);
    else if(strcasecmp(PARAM_COMMAND,"rem")==0);
    else if(strcasecmp(PARAM_COMMAND,"edit")==0);
    else if(strcasecmp(PARAM_COMMAND,"ren")==0);
    else if(strcasecmp(PARAM_COMMAND,"mkdir")==0);
    else if(strcasecmp(PARAM_COMMAND,"cp")==0);
    else if(strcasecmp(PARAM_COMMAND,"mv")==0);
    else if(strcasecmp(PARAM_COMMAND,"find")==0);
    else if(strcasecmp(PARAM_COMMAND,"rep")==0)do_rep();
    else if(strcasecmp(PARAM_COMMAND,"exec")==0)do_exec();
    else{

            printf("El comando: %s no exxiste!\n",PARAM_COMMAND);


    }




};

void interpretarComentario(){

   // ResetParam();

    char*token;
    token = strtok(bufferComando, " ");

    printf("Comentario: ");

    while( token != NULL)
    {
        printf("%s ", token);
        token = strtok(NULL, " ");

    }

    printf(".\n");



};

void Console(){
    ReadConsole("\n-->",bufferComando,250);
    //printf("comando: %s",bufferComando);

    if(strcasecmp(bufferComando,"exit")==0)
    return;
    else if(strcasecmp(bufferComando,"clear")==0)
    Limpiar();
    else if(strcasecmp(bufferComando, "#")==0){
        printf("Este es un comentario\n");
        interpretarComentario();
    }
    else if(strlen(bufferComando)>0)
      interpretarComando();
    //interpretarComentario();
    ///interpretarcomando
    Console();
};

void InicializarMontados(){
    int i;
    for(i=0;i<MAX_MONTADOS;i++){
        Montados[i].status=LIBRE;
    }
};


void MontarParticion(const char* pathdisco, const char* nombreparticion){
    printf("\n-------------------------------------------------\n");
    printf("\nMontando particion...\n");
    PARTITION p=BuscarParticion(pathdisco,nombreparticion);
    if(p.part_size==-1){printf("Error en montaje de particion. No se encontro.\n");return;}
    char letraMax='a'-1;
    char letraDisco='\0';
    int numMax=0;
    int indexLibre=-1;
    int i;
    ///buscando espacio libre y formando el id para montaje
    PARTITION_MOUNTED temp;
    for(i=0;i<MAX_MONTADOS;i++){
        temp=Montados[i];
        if(temp.status==OCUPADO){
            if(letraMax<temp.letra)letraMax=temp.letra;//por si no hay otra particion del mismo disco.
            if(strcmp(temp.path,pathdisco)==0){//pertenece al disco
                if(strcmp(temp.nombre,nombreparticion)==0){printf("La particion: %s del disco: %s ya esta montada.\n",nombreparticion,pathdisco);return;}
                if(temp.numero>numMax)numMax=temp.numero;
                letraDisco=temp.letra;
            }
        }else{
            if(indexLibre==-1)
            indexLibre=i;
        }
    }
    if(indexLibre==-1){printf("Lo sentimos no hay espacio para montar la particion.\n");return;}
    printf("Direccion de montaje ubicado.\n");
    printf("ID generado.\n");
    //reutilicemos el temp para no declarar otra :)
    temp.status=OCUPADO;
    strcpy(temp.nombre,nombreparticion);
    strcpy(temp.path,pathdisco);
    if(letraDisco=='\0')temp.letra=letraMax+1;
    else temp.letra=letraDisco;
    temp.numero=numMax+1;
    temp.part=p;

    Montados[indexLibre]=temp;



    printf("Particion montada exitosamente.\n");
    char result[10];
    printf("Desea ver las particiones montadas? [Y/N].\n");
    gets(result);
    if(result[0]=='y'||result[0]=='Y')
    ImprimirMontados();
};


void DesMontarParticion(const char* id){
    printf("Buscando Particion en montados...\n");
    int pIndex=BuscarEnMontados(id);
    if(pIndex==-1){printf("Particion: '%s' no encontrada.\n",id);return;}
    printf("Particion encontrada [%d].\n",pIndex);
    printf("Desmontando...\n");
    Montados[pIndex].status=LIBRE;
    printf("Particion desmontada.\n");

//   PARTITION p=Montados[pIndex].part;//BuscarParticion(Montados[pIndex].path,Montados[pIndex].nombre);




    char result[10];
    ReadConsole("Desea ver las particiones montadas? [Y/N].\n",result,9);

    if(result[0]=='y'||result[0]=='Y')
    ImprimirMontados();
};
int BuscarEnMontados(const char* id){

   int r=-1;

    if(id==NULL){printf("No se encontro la particion montada.\n");return -1;}
    if(strlen(id)<4){printf("Id muy corto.\n");return -1;}
    int i;

    for(i=0;i<MAX_MONTADOS;i++){
        if(Montados[i].status==OCUPADO){
            if(Montados[i].letra==id[2]&&Montados[i].numero==id[3]-48)
            return i;
        }
    }
    return r;
};
void ImprimirMontados(){
    int i;
    PARTITION_MOUNTED temp;
    printf("----------------------Montados----------------------\n");
    printf("    ID          Nombre          Path\n");
    for(i=0;i<MAX_MONTADOS;i++){
        temp=Montados[i];
        if(temp.status==OCUPADO)

        printf("    vd%c%d      %s              %s.\n",temp.letra,temp.numero,temp.nombre,temp.path);
    }
    printf("--------------------Ultima Linea--------------------\n");
};
int mainMenu1()
{
    InicializarMontados();
    Limpiar();
    //printf("bloque carpeta: %ld\n",sizeof(BLOQUE_CARPETA));
    //printf("bloque archivo: %ld\n",sizeof(BLOQUE_ARCHIVO));
    //printf("bloque apuntadores: %ld\n",sizeof(BLOQUE_APUNTADORES));
    //printf("super bloque: %ld\n",sizeof(SUPER_BLOQUE));
    //printf("mbr: %ld\n",sizeof(MBR));
    //printf("atoi: %d\n",atoi("hola-123#")+120);
    Console();
   /* system("mkdir -m a=rwx -p /home/esteban/Archivos/dir1/dir4");
    FILE* f=fopen("/home/esteban/Archivos/dir1/dir4/hola.qeaze","w");
    fwrite("hola",6,1,f);
    fclose(f);
    FILE* f1=fopen("/home/esteban/Archivos/dir11/dir4/hola.qeaze","w");
    if(f1==NULL){printf("noexiste la carpeta\n");}
    else{
    printf("si existe la carpeta\n");
    }*/
    return 0;
}
