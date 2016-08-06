#include "reportes.h"


void do_mbrReport(const char* pathDisco,const char*pathSalida)
{
    printf("Creando Reporte de MBR de disco.\n");
    MBR mbr=ReadMBR(pathDisco);

    if(mbr.mbr_tamano!=-1)
    {

        /**escribiendo reporte**/
        printf("%s.\n",CCAT("mkdir -m a=rwx -p ",getParentPath(pathSalida)));
        system(CCAT("mkdir -m a=rwx -p ",getParentPath(pathSalida)));
        char* dot="";
        dot=CCAT(getParentPath(pathSalida),"MBRReport");
        dot=CCAT(dot,".dot");

        FILE*report=fopen(dot,"w");
        if(report)
        {
            PARTITION temp;
            fprintf(report,"graph g{\n");
            fprintf(report,"\tsubgraph cluster0 {\n");
            //fprintf(report,"\t\tnode [shape=record,width=.1,height=.1];\n");
            //fprintf(report,"\t\tnode0 [label = \"{|");
            fprintf(report,"\t\tnode [shape=record];\n");
            //fprintf(report,"\t\tnode0 [label = < <table> <tr> <td>Estado</td><td>Nombre</td><td>Byte Inicio</td><td>Num.Bloques</td><td>Bloques Libres</td><td>Tipo</td><td>Ajuste</td><td>Tamano</td> </tr>");
            fprintf(report,"\t\tnode0 [label = < <table> <tr> <td>CAMPO</td><td>VALOR</td></tr>");

            fprintf(report,"<tr><td>Tamano</td><td>%d</td></tr>",mbr.mbr_tamano);

            struct tm *tlocal = localtime(&mbr.mbr_fecha_creacion);
            char output[128];
            strftime(output,128,"%d/%m/%Y %H:%M:%S",tlocal);

            fprintf(report,"<tr><td>Fecha Creacion</td><td>%s</td></tr>",output);
            fprintf(report,"<tr><td>Signature</td><td>%d</td></tr>",mbr.mbr_disk_signature);

            int i;
            for(i=0; i<4; i++)
            {
                temp=mbr.mbr_partition[i];
                if(temp.part_status==OCUPADO)
                {
                    fprintf(report,"<tr><td>part_status_%d</td><td>%c</td></tr>",i,temp.part_status);
                    fprintf(report,"<tr><td>part_type_%d</td><td>%c</td></tr>",i,temp.part_type);
                    fprintf(report,"<tr><td>part_fit_%d</td><td>%c</td></tr>",i,temp.part_fit);
                    fprintf(report,"<tr><td>part_start_%d</td><td>%d</td></tr>",i,temp.part_start);
                    fprintf(report,"<tr><td>part_size_%d</td><td>%d</td></tr>",i,temp.part_size);
                    fprintf(report,"<tr><td>part_name_%d</td><td>%s</td></tr>",i,temp.part_name);

                }
            }
            fprintf(report," </table> >];\n");
            //fprintf(report,"\t\tstyle=filled;\n\t\tcolor=lightgrey;\n");
            fprintf(report,"\t\tlabel = \"MBR de: %s.\";\n",fileName(pathDisco));
            fprintf(report,"\t}\n}");

            fclose(report);

            char*command=CCAT("dot -Tjpeg \"",dot);
            //char* o=CCAT(getParentPath(pathSalida),"MBR.jpeg\"");
            //o=CCAT("\" -o \"",o);
            //command=CCAT(command,o);
            command=CCAT(command,"\" -o \"");
            command=CCAT(command,pathSalida);
            command=CCAT(command,"\"");

            printf("%s\n",command);
            system(command);
            printf("Reporte de MBR creado exitosamente.\n");
        }
        else
        {
            printf("No se Pudo abrir el archivo: %s.\n",dot);
        }
    }
    else
    {
        printf("Error al acceder al disco.\n");
    }
}
void do_diskReport(const char*pathDisco,const char*pathSalida)
{
    printf("Creando Reporte de disco...\n");
    MBR mbr =ReadMBR(pathDisco);
    if(mbr.mbr_tamano==-1)
    {
        printf("Error al acceder al disco.\n");
        return;
    }

    printf("%s.\n",CCAT("mkdir -m a=rwx -p ",getParentPath(pathSalida)));
    system(CCAT("mkdir -m a=rwx -p ",getParentPath(pathSalida)));
    char* dot="";
    dot=CCAT(getParentPath(pathSalida),"DiskReport");
    dot=CCAT(dot,".dot");

    FILE*report=fopen(dot,"w");
    if(report==NULL)
    {
        printf("Error al crear el reporte.(.dot)\n");
        return;
    }


    /**escribiendo reporte**/
    fprintf(report,"graph g {\n");
    fprintf(report,"\tnodesep=.05;\n\trankdir=TB;\n");
    fprintf(report,"\tnode [shape=record, width = 1.5, height = 3];\n");
    fprintf(report,"\tnode0 [label = \"MBR\"];\n");

    int cotaInf=sizeof(MBR);
    int cotaSup=mbr.mbr_tamano;
    int TAM=0;
    int nodeCounter=1;
    int i;
    PARTITION partActual;
    for(i=0; i<4; i++)
    {
        partActual=mbr.mbr_partition[i];
        if(partActual.part_status==OCUPADO)
        {

            TAM=partActual.part_start-cotaInf;
            ///escribir GVnodo espacio libre
            if(TAM>0)
                fprintf(report,"\tnode%d [width = %f, label = \"Libre\"];\n",nodeCounter++,10.0*TAM/mbr.mbr_tamano);
            ///ecribir SVnodo particion
            float psize=35.0*partActual.part_size/mbr.mbr_tamano;
            //float apz=partActual.part_size;
            printf("size %f.\n",psize);
            fprintf(report,"\tnode%d [width = %f, label = \"%s\"];\n",nodeCounter++, psize, partActual.part_name);

            cotaInf=partActual.part_start+partActual.part_size;
        }
    }
    if(cotaSup-cotaInf>0)
    {
        ///escribir ultimo espacio nlibre
        fprintf(report,"\tnode%d [width = %f, label = \"Libre\"];\n",nodeCounter++,35.0*TAM/mbr.mbr_tamano);
    }

    fprintf(report,"}");


    fclose(report);
    char*command=CCAT("dot -Tjpeg \"",dot);
    command=CCAT(command,"\" -o \"");
    command=CCAT(command,pathSalida);
    command=CCAT(command,"\"");

    printf("%s\n",command);
    system(command);
    printf("Reporte de Disco creado exitosamente.\n");

}

void do_ebrReport(const char* pathDisco, const char*pathSalida)
{
    printf("Creando Reporte de EBR de particion.\n");
    EBR ebr=ReadEBR(pathDisco);

    if(ebr.part_size!=-1)
    {

        /**escribiendo reporte**/
        printf("%s.\n",CCAT("mkdir -m a=rwx -p ",getParentPath(pathSalida)));
        system(CCAT("mkdir -m a=rwx -p ",getParentPath(pathSalida)));
        char* dot="";
        dot=CCAT(getParentPath(pathSalida),"EBRReport");
        dot=CCAT(dot,".dot");

        FILE*report=fopen(dot,"w");
        if(report)
        {
            PARTITION temp;
            fprintf(report,"graph g{\n");
            fprintf(report,"\tsubgraph cluster0 {\n");
            //fprintf(report,"\t\tnode [shape=record,width=.1,height=.1];\n");
            //fprintf(report,"\t\tnode0 [label = \"{|");
            fprintf(report,"\t\tnode [shape=record];\n");
            //fprintf(report,"\t\tnode0 [label = < <table> <tr> <td>Estado</td><td>Nombre</td><td>Byte Inicio</td><td>Num.Bloques</td><td>Bloques Libres</td><td>Tipo</td><td>Ajuste</td><td>Tamano</td> </tr>");
            fprintf(report,"\t\tnode0 [label = < <table> <tr> <td>CAMPO</td><td>VALOR</td></tr>");

            fprintf(report,"<tr><td>Tamano</td><td>%d</td></tr>",temp.part_size);

            if(temp.part_status==OCUPADO)
            {
                fprintf(report,"<tr><td>part_status_%d</td><td>%c</td></tr>",1,temp.part_status);
                fprintf(report,"<tr><td>part_fit_%d</td><td>%c</td></tr>",1,temp.part_fit);
                fprintf(report,"<tr><td>part_start_%d</td><td>%d</td></tr>",1,temp.part_start);
                fprintf(report,"<tr><td>part_size_%d</td><td>%d</td></tr>",1,temp.part_size);
                fprintf(report,"<tr><td>part_name_%d</td><td>%s</td></tr>",1,temp.part_name);

            }

            fprintf(report," </table> >];\n");
            //fprintf(report,"\t\tstyle=filled;\n\t\tcolor=lightgrey;\n");
            fprintf(report,"\t\tlabel = \"EBR de: %s.\";\n",fileName(pathDisco));
            fprintf(report,"\t}\n}");

            fclose(report);

            char*command=CCAT("dot -Tjpeg \"",dot);
            //char* o=CCAT(getParentPath(pathSalida),"MBR.jpeg\"");
            //o=CCAT("\" -o \"",o);
            //command=CCAT(command,o);
            command=CCAT(command,"\" -o \"");
            command=CCAT(command,pathSalida);
            command=CCAT(command,"\"");

            printf("%s\n",command);
            system(command);
            printf("Reporte de EBR creado exitosamente.\n");
        }
        else
        {
            printf("No se Pudo abrir el archivo: %s.\n",dot);
        }
    }
    else
    {
        printf("Error al acceder al disco.\n");
    }
}





