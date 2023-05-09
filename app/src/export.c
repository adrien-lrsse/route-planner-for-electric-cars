#include "export.h"


void export_etape(etape* one_etape, int my_pid){
    char pid[10];
    sprintf(pid,"%d",my_pid);
    char path[100] = "../data/trajets/etape_";
    strcat(path,pid);
    strcat(path,".txt");
    FILE* fichier = fopen(path,"w");
    if (etape_is_empty(one_etape)){
        fprintf(fichier,"404\n");
        fclose(fichier);
    }
    else 
    {
    for (int i = 0; i < one_etape->size; i++)
    {      
        fprintf(fichier,"%d$%s$%Lf$%Lf$%f$%f$\n",i,(one_etape->list+i)->borne.name,(one_etape->list+i)->borne.coordonnees.longitude,(one_etape->list+i)->borne.coordonnees.latitude,(one_etape->list+i)->distance_debut,(one_etape->list+i)->distance_fin);
    }
    fclose(fichier);
    }
}
