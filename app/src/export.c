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

void thread_export(etape* one_etape, trajet* one_trajet){
    FILE* fichier = fopen("../data/forks.txt","a");
    if (etape_is_empty(one_etape)){
        fprintf(fichier,"\n404");
        fclose(fichier);
    }
    else{
        int global_tick = 0;
        int id_borne = 0;
        int tick_deplacement = 0;
        int duree_charge = 0;
        fprintf(fichier, "\n");
        for (int i = 0; i < one_etape->size; i++){  
            id_borne = (one_etape->list+i)->borne.id;
            tick_deplacement = temps_trajet(one_etape->list+i);
            if (i<one_etape->size-1){
                if (tick_deplacement == 1 && (one_etape->list+i)->borne.id != -1 && (one_etape->list+i)->borne.id != 0 && (one_etape->list+i+1)->borne.id > 0){
                    // printf("one_etape->list+i)->borne.id = %d\n", (one_etape->list+i)->borne.id);
                    // printf("one_etape->list+i)->distance_debut = %f\n", (one_etape->list+i)->distance_debut);
                    // printf("%Lf ", one_trajet->depart->longitude);
                    // printf("%Lf ", one_trajet->depart->latitude);
                    // printf("%Lf ", one_trajet->arrivee->longitude);
                    // printf("%Lf ", one_trajet->arrivee->latitude);
                    // printf("%d ", one_trajet->id_voiture);
                    // printf("%d ", one_trajet->pourcentage_mini_voulu);
                    // printf("%d ", one_trajet->temps_max_attente_borne);
                    // printf("%d ", one_trajet->type);
                    // printf("%d ", one_trajet->pourcentage_autonomie_initiale);
                    // printf("\n");
                    printf("coucou\n");
                    fprintf(fichier, "\r404 // ERREUR ticks trop proches : '%Lf %Lf %Lf %Lf %d %d %d %d %d'", one_trajet->depart->longitude, one_trajet->depart->latitude, one_trajet->arrivee->longitude, one_trajet->arrivee->latitude, one_trajet->id_voiture, one_trajet->pourcentage_mini_voulu, one_trajet->temps_max_attente_borne, one_trajet->type, one_trajet->pourcentage_autonomie_initiale);
                    fclose(fichier);
                    return;
                }
            }
            tick_deplacement += global_tick;
            duree_charge = (one_etape->list+i)->tick_recharge;
            if (i == one_etape->size-1){
                duree_charge = -2;
            }
            fprintf(fichier,"%d$%d$%d||",id_borne,tick_deplacement,duree_charge);
            global_tick = tick_deplacement+duree_charge;
            if (global_tick > 160) {
                fprintf(fichier, "\r404$-1$-1|| // TRAJET TROP LONG");
                fclose(fichier);
                return;
            }
        }
        fclose(fichier);
    }
    return;
}

void    thread_export_init(void){
    FILE *fichier = fopen("../data/forks.txt","w");
    fclose(fichier);
    return;
}
