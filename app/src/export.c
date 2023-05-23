#include "export.h"


void export_etape(etape* one_etape){
    FILE *fichier = fopen("../data/etape.txt","w");
    if (etape_is_empty(one_etape)){
        fprintf(fichier,"404\n");
        fclose(fichier);
    }
    else 
    {
    for (int i = 0; i < one_etape->size; i++)
    {      
        fprintf(fichier,"%d$%s$%Lf$%Lf$%f$%f$%f$\n",i,(one_etape->list+i)->borne.name,(one_etape->list+i)->borne.coordonnees.longitude,(one_etape->list+i)->borne.coordonnees.latitude,(one_etape->list+i)->distance_debut,(one_etape->list+i)->distance_fin,(one_etape->list+i)->autonomie_borne);
    }
    fclose(fichier);
    }
    return;
}

void thread_export(etape* one_etape){
    FILE* fichier = fopen("../data/forks.txt","a");
    if (etape_is_empty(one_etape)){
        fprintf(fichier,"\n404");
        fclose(fichier);
    }
    else{
        int global_tick = 0;
        fprintf(fichier, "\n");
        for (int i = 0; i < one_etape->size; i++){  
            int id_borne = (one_etape->list+i)->borne.id;
            int tick_deplacement = temps_trajet(one_etape->list+i)+global_tick;
            int duree_charge = (one_etape->list+i)->tick_recharge;
            if (i == one_etape->size-1){
                duree_charge = -2;
            }
            fprintf(fichier,"%d$%d$%d||",id_borne,tick_deplacement,duree_charge);
            global_tick = tick_deplacement+duree_charge;
        }
        fclose(fichier);
    }
    return;
}

void thread_export_init(void){
    FILE *fichier = fopen("../data/forks.txt","w");
    fclose(fichier);
    return;
}
