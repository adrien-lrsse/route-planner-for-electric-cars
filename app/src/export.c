#include "export.h"


void export_etape(etape* one_etape){
    FILE *fichier = fopen("../data/etape.txt","w");
    for (int i = 0; i < one_etape->size; i++)
    {      
        fprintf(fichier,"%d$%s$%Lf$%Lf$%f$%f$\n",i,(one_etape->list+i)->borne.name,(one_etape->list+i)->borne.coordonnees.longitude,(one_etape->list+i)->borne.coordonnees.latitude,(one_etape->list+i)->distance_debut,(one_etape->list+i)->distance_fin);
    }
    fclose(fichier);
}
