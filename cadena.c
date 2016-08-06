#include "cadena.h"
char* CCAT(const char* a,const char*b){
    if((a!=NULL) && (b!=NULL)){
        int tam;
        tam=strlen(a)+strlen(b)+1;
        char*srt_ptr;
        srt_ptr=(char*)malloc(tam);
        sprintf(srt_ptr, "%s%s",a,b);
        return srt_ptr;
    }
    return "";
};
char* CaCAT(const char* a,const char b){
    if((a!=NULL) ){
        printf("a: -%s-\n",a);
        int tam;
        tam=strlen(a)+2;
        char*srt_ptr;
        srt_ptr=(char*)malloc(tam);
        strcpy(srt_ptr,a);
        printf("srt_ptr: -%s-\n",srt_ptr);
        //sprintf(srt_ptr, "%s%c",a,b);
        sprintf(srt_ptr, "%c",b);
        printf("srt_ptr: -%s-\n",srt_ptr);
        return srt_ptr;
    }
    return "";
};
char* CaCAT2(const char* a,const char b){
    if((a!=NULL) ){
       int tam=strlen(a)+1;
       printf("tamaño a: %d\n",tam);
       char s[tam+1];
       strcpy(s,a);
       s[tam-1]=b;
       s[tam]='\0';

       printf("s: -%s-",s);
       char* r=s;
       return r;
    }
    return "";
};
char getLast(const char* cad){
    char c=cad[strlen(cad)-1];
   // printf("este es el ultimo caracter: -%c-\n",c);
    return c;
};
char* subString(const char*cad,int start,int length){
    int largo=strlen(cad);
    if((start>=0)&&(length<largo)){
        printf("tam cadena: %i\n",(length-start));
        //char *s = malloc(sizeof(length-start));
        char a[length-start+1];
        int i;
        for(i=start;i<(length-start);i++){
            a[i-start]=cad[i];
            //s[i-start]=cad[i];
            //printf("char: -%c-\n",s[i-start]);
        }
        char*s=a;
        printf("substring: -%s-\n",s);
        return s;
    }
    return "";
};
int toInt(const char* str){
    if(str==NULL)return 0;
    char *ptr;
    long ret;

    ret = strtol(str, &ptr, 10);
    return (int)ret;
};
char* getParentPath(const char*fullPath){
    if(fullPath==NULL)return "";
    char buff[strlen(fullPath)*2];
    strcpy(buff,fullPath);
    char* tok=strtok(buff,"/");
    char* salida="/";
    if(tok==NULL)return "";
    char*next=strtok(NULL,"/");

    int safecounter=0;
    while(next!=NULL&&safecounter<100){
        salida=CCAT(salida,tok);
        salida=CCAT(salida,"/");
        tok=next;
        next=strtok(NULL,"/");
        safecounter++;
    }
    return salida;
};
char* fileName(const char*fullPath){
    if(fullPath==NULL)return "";
    char buff[strlen(fullPath)*1];
    strcpy(buff,"");
    strcpy(buff,fullPath);
    char* tok=strtok(buff,"/");
    char* salida=tok;
    int safecounter=0;
    while(tok!=NULL&&safecounter<100){
        salida=tok;
        tok=strtok(NULL,"/");
        safecounter++;
    }
    return salida;
};
char* time_t_to_string(time_t t){
        struct tm *tlocal = localtime(&t);
        char output[128];
        strftime(output,128,"%d/%m/%Y %H:%M:%S",tlocal);
        char* r=output;
        return r;
};
