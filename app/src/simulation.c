#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "borne.h"
#include "vehicules.h"
#include "database_reader.h"
#include "coordinates.h"
#include "simulation.h"
#include "itinerary.h"
#include "etape.h"
// #include "borne.h"

int TOTAL_BORNES = 17319;
int TOTAL_TICK = 144+100;

borne_simulation* load_bornes(void){
    //charge l'ensemble des bornes de la BD
    borne_simulation* tableau_bornes = (borne_simulation*) malloc(TOTAL_BORNES*sizeof(borne_simulation));
    database_t * database = open_database("../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }

    // préparation de la requête
    char* sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique, nbre_pdc FROM bornes";
    prepare_request_database(database, sql_commmand); 
    
    int rc;
    int compteur = 0;
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat
        tableau_bornes[compteur].coordonnees.longitude = sqlite3_column_double(database->stmt, 0);
        tableau_bornes[compteur].coordonnees.latitude = sqlite3_column_double(database->stmt, 1);
        tableau_bornes[compteur].id = sqlite3_column_int(database->stmt,2);
        tableau_bornes[compteur].capacite_max = sqlite3_column_int(database->stmt, 3);
        //liste des passages des voitures initialisées
        tableau_bornes[compteur].list_passages = (passage_voiture_head*) malloc(sizeof(passage_voiture_head));
        tableau_bornes[compteur].list_passages->head = NULL;
        compteur++;
    }

    end_request_database(database);
    close_database(database);
    return tableau_bornes;
}


void simulation(void){    
    FILE * fichier;
    size_t fichier_size;
    char * buffer;
    size_t res;
    borne_simulation* tab_bornes = load_bornes();

    fichier = fopen ( "../data/forks.txt" , "r+" );
    if (fichier==NULL) {
        fprintf (stderr, "File error\n"); 
        exit (1);
    }

    fseek (fichier , 0 , SEEK_END);
    fichier_size = ftell (fichier);
    rewind (fichier);

    buffer = (char*) malloc (sizeof(char)*fichier_size);
    if (buffer == NULL) {
        fputs ("Memory error",stderr); 
        exit (2);
    }

    res = fread (buffer,1,fichier_size,fichier);
    if (res != fichier_size) {
        fputs ("Reading error",stderr); 
        exit (3);
    }

    int* trajets_finis = (int*)calloc(TOTAL_TICK,sizeof(int));

    int borne_id, tick_arrive, temps_recharge;
    int id_voiture = 0;
    int nb_total_voiture = 0;
    int decalage = 0;
    char * strtok_res;
    strtok_res = strtok(buffer, "||");
    sscanf(strtok_res, "%d$$", &nb_total_voiture);
    strtok_res = strtok (NULL, "||");
    decalage_t* tab_decalage = (decalage_t*) malloc(nb_total_voiture*sizeof(decalage_t));
    for (int i = 0;i<nb_total_voiture;i++) {tab_decalage[i].borne_finale = -1;}

    while (strtok_res != NULL)
    {
        sscanf(strtok_res, "%d$%d$%d",&borne_id,&tick_arrive,&temps_recharge);
        // if (temps_recharge == -2) {printf("%d\n",borne_id);}
        if (temps_recharge != -1 && temps_recharge != -2) {//l'étape dans le trajet n'est ni l'arrivée ni le départ
            borne_id--;
            tab_decalage[id_voiture].borne_finale = borne_id;
            tab_decalage[id_voiture].decalage_value = ajout_passage(tab_bornes,id_voiture,tick_arrive+decalage,temps_recharge,borne_id);//garde la valeur de la sortie à la borne
            decalage = decalage + tab_decalage[id_voiture].decalage_value - (tick_arrive+decalage+temps_recharge);
            // if (id_voiture==618) {printf("borne:%d decalage:%d\n",borne_id,decalage);}
            // if (id_voiture == 74) {affichage_liste_passages(tab_bornes[borne_id].list_passages);printf("%d\n",decalage);}
            
        }
        else if (temps_recharge == -2 && borne_id != 404) {
            // printf("decalage : %d; voiture: %d\n",decalage, id_voiture);
            // if (id_voiture == 74) {printf("---- voiture %d ----\n",id_voiture);}
            // if (id_voiture == 21) {affichage_liste_passages(tab_bornes[tab_decalage[id_voiture].borne_finale].list_passages);}
            tab_decalage[id_voiture].tick_etape_finale = tick_arrive;
            id_voiture++;
            decalage = 0;
        }

        strtok_res = strtok (NULL, "||");
    }

    int tick_final_reel = 0;
    int tick_arrivee_reel = 0;
    for (int i = 0;i<nb_total_voiture;i++) {
        if (tab_decalage[i].borne_finale != -1) {
            // printf("------------------------\n");
            // printf("(%d) in list : %d ; previous : %d\n",i,find_tick_sortie(tab_bornes[tab_decalage[i].borne_finale].list_passages,i),tab_decalage[i].decalage_value);
            tick_final_reel = (find_tick_sortie(tab_bornes[tab_decalage[i].borne_finale].list_passages,i) - tab_decalage[i].decalage_value);
            tick_arrivee_reel = tick_final_reel + tab_decalage[i].tick_etape_finale;
            // if (i == 618) {printf("(%d) %d\n",i,tick_arrivee_reel);}
            trajets_finis[tick_arrivee_reel]++;
            // printf("(%d) to : %d ; tf : %d\n",i,tab_decalage[i].tick_etape_finale,tick_arrivee_reel);
            tick_final_reel = 0;
            tick_arrivee_reel = 0;
        }
    }

    export(tab_bornes, trajets_finis);

    fclose (fichier);
    free (buffer);
    free(trajets_finis);
    free(tab_decalage);
    destroy_tab(tab_bornes);
}

int ajout_passage(borne_simulation* list_bornes, int id_voiture, int tick, int duree_charge, int borneId){
    // le tick en paramètre est le tick auquel la voiture arrive à la station

    int tick_recharge = tick+duree_charge;

    passage_voiture_head* file_d_attente = (passage_voiture_head*) malloc(sizeof(passage_voiture_head));
    file_d_attente->head = NULL;

    passage_voiture_head_append(file_d_attente, id_voiture, 1, list_bornes[borneId].capacite_max-1, tick);
    passage_voiture_head_append(file_d_attente, id_voiture, 0, list_bornes[borneId].capacite_max, tick_recharge);

    passage_voiture_head* list_passages_head = list_bornes[borneId].list_passages;//pointe la structure

    passage_voiture* tampon = creer_passage(0,0,0,0);
    // printf("%d\n", list_passages_head == NULL);
    if (list_passages_head->head == NULL) {
        list_passages_head->head = passage_voiture_head_pop(file_d_attente);
        list_passages_head->head->next_passage = passage_voiture_head_pop(file_d_attente);
        free(tampon);
        free(file_d_attente);
        // if (borneId == 4920) {affichage_liste_passages(list_bornes[borneId].list_passages);}
        return tick_recharge;
    }

    passage_voiture* passage_file_attente = file_d_attente->head;
    passage_voiture* current = list_passages_head->head;//1er élément
    int passage_ajoute = 0;
    int compteur = 0;
    

    if (passage_file_attente->tick <= current->tick) {
        tampon->next_passage = passage_voiture_head_pop(file_d_attente);
        tampon->next_passage->next_passage = current;
        list_passages_head->head = tampon->next_passage;
        tampon->next_passage = NULL; //useless but just in case
        passage_file_attente = file_d_attente->head;
        current = list_passages_head->head;
        // place_in_list++;
    }
    while (current->next_passage != NULL && file_d_attente->head != NULL){ //tant que la file d'attente n'est pas vide
        if (current->places_restantes == -1 && current->status_passage == 1) {//si une voiture est arrivée et qu'elle n'a pas de places (diapo 31-32)
            current->status_passage = 2;
            current->places_restantes = 0;
            passage_voiture_head_append(file_d_attente,current->id_voiture,3,-1,current->tick);
            passage_voiture_head_append(file_d_attente,current->id_voiture,4,-1,remove_passage0(list_bornes[borneId].list_passages,current->id_voiture));
        }
        if (current->next_passage != NULL) {
        switch (current->next_passage->status_passage) {//mets à jour le nombre de places d'après
        case 1:
            current->next_passage->places_restantes = current->places_restantes-1;
            break;
        case 0:
            current->next_passage->places_restantes = current->places_restantes+1;
            break;
        default:
            break;
        }}
    // if (borneId == 6  && id_voiture == 104) {affichage_liste_passages(list_bornes[borneId].list_passages);affichage_liste_passages(file_d_attente);}

        passage_file_attente = file_d_attente->head;// réinitialise le pointeur à la tête de la file d'attente
        passage_ajoute = 0; //booleen indiquant si un passage a été ajouté dans la liste
        compteur = 0;
        while (passage_file_attente->next_passage != NULL && current->next_passage != NULL) { //regarde chaque élement de la file d'attente
            if (passage_ajoute == 0) { // distinguer le cas où un élément a été ajouté et traiter les cas de modification de la liste
            if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 0) {//si l'élément dans la file d'attente a un tick plus faible que le prochain élément de la liste et que la voiture doit partir (diapo 10)
                if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
                connect(file_d_attente,compteur,current,0);
                passage_ajoute = 1;
                compteur--;
            }
            else if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 1 && current->places_restantes > 0) {//si l'élément de la file d'attente a un tick plus faible, que la voiture arrive et qu'il reste de la place (diapo 36-37)
                if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
                connect(file_d_attente,compteur,current,1);
                passage_ajoute = 1;
                compteur--;
            }
            else if (passage_file_attente->status_passage == 3 && current->places_restantes > 0) {
                if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
                connect(file_d_attente,compteur,current,3);
                passage_ajoute = 1;
                compteur--;
            }
            }

            //tests qui devront toujours avoir lieu même si un passage a été ajouté
            if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 1 && current->places_restantes == 0) {//une voiture arrive mais il n'y plus de place
                tampon->next_passage = creer_passage(passage_file_attente->id_voiture, 2, 0, current->tick);
                change_sortie_attente(file_d_attente,passage_file_attente->id_voiture);
                tampon->next_passage->next_passage = current->next_passage;
                current->next_passage = tampon->next_passage;
                tampon->next_passage = NULL;
                passage_file_attente->status_passage = 3;
            }

            if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
            compteur++;
        }
        //traiter le dernier élément
        if (current->next_passage != NULL) {
        if (passage_ajoute == 0) { // distinguer le cas où un élément a été ajouté et traiter les cas de modification de la liste
        if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 0) {//si l'élément dans la file d'attente a un tick plus faible que le prochain élément de la liste et que la voiture doit partir (diapo 10)
            if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
            connect(file_d_attente,compteur,current,0);
            passage_ajoute = 1;
            compteur--;
        }
        else if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 1 && current->places_restantes > 0) {//si l'élément de la file d'attente a un tick plus faible, que la voiture arrive et qu'il reste de la place (diapo 36-37)
            if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
            connect(file_d_attente,compteur,current,1);
            passage_ajoute = 1;
            compteur--;
        }
        else if (passage_file_attente->status_passage == 3 && current->places_restantes > 0) {
            if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
            connect(file_d_attente,compteur,current,3);
            passage_ajoute = 1;
            compteur--;
        }
        }

        //tests qui devront toujours avoir lieu même si un passage a été ajouté
        if (current->next_passage->tick >= passage_file_attente->tick && passage_file_attente->status_passage == 1 && current->places_restantes == 0) {//une voiture arrive mais il n'y plus de place
            tampon->next_passage = creer_passage(passage_file_attente->id_voiture, 2, 0, current->tick);
            change_sortie_attente(file_d_attente,passage_file_attente->id_voiture);
            tampon->next_passage->next_passage = current->next_passage;
            current->next_passage = tampon->next_passage;
            tampon->next_passage = NULL;
            passage_file_attente->status_passage = 3;
        }
        if (current->next_passage != NULL) {current = current->next_passage;}
        }
    }
    if (current->next_passage == NULL && file_d_attente->head != NULL) {//si on et au bout de la liste des passages et que la file d'attente n'est pas vide
        if (current->places_restantes == -1 && current->status_passage == 1) {//si une voiture est arrivée et qu'elle n'a pas de places (diapo 31-32)
            current->status_passage = 2;
            current->places_restantes = 0;
            passage_voiture_head_append(file_d_attente,current->id_voiture,3,-1,current->tick);
            passage_voiture_head_append(file_d_attente,current->id_voiture,4,-1,remove_passage0(list_bornes[borneId].list_passages,current->id_voiture));
            
        }

        passage_file_attente = file_d_attente->head;// réinitialise le pointeur à la tête de la file d'attente
        passage_ajoute = 0; //booleen indiquant si un passage a été ajouté dans la liste
        compteur = 0;
        while (passage_file_attente->next_passage != NULL) { //regarde chaque élement de la file d'attente
    // if (borneId == 5106) {affichage_liste_passages(list_bornes[borneId].list_passages,place_in_list);affichage_liste_passages(file_d_attente,-1);}
            if (passage_ajoute == 0) { // distinguer le cas où un élément a été ajouté et traiter les cas de modification de la liste
            if (passage_file_attente->status_passage == 0) {//si l'élément dans la file d'attente a un tick plus faible que le prochain élément de la liste et que la voiture doit partir (diapo 10)
                if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
                connect(file_d_attente,compteur,current,0);
                passage_ajoute = 1;
                compteur--;
            }
            else if (passage_file_attente->status_passage == 1 && current->places_restantes > 0) {//si l'élément de la file d'attente a un tick plus faible, que la voiture arrive et qu'il reste de la place (diapo 36-37)
                if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
                connect(file_d_attente,compteur,current,1);
                passage_ajoute = 1;
                compteur--;
            }
            else if (passage_file_attente->status_passage == 3 && current->places_restantes > 0) {
                if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
                connect(file_d_attente,compteur,current,3);
                passage_ajoute = 1;
                compteur--;
            }
            }

            //tests qui devront toujours avoir lieu même si un passage a été ajouté
            if (passage_file_attente->status_passage == 1 && current->places_restantes == 0) {//une voiture arrive mais il n'y plus de place
                tampon->next_passage = creer_passage(passage_file_attente->id_voiture, 2, 0, current->tick);
                change_sortie_attente(file_d_attente,passage_file_attente->id_voiture);
                tampon->next_passage->next_passage = current->next_passage;
                current->next_passage = tampon->next_passage;
                tampon->next_passage = NULL;
                passage_file_attente->status_passage = 3;
            }

            if (passage_file_attente->next_passage == NULL && file_d_attente->head != NULL) {
                passage_file_attente = file_d_attente->head;
                compteur = -1;
            }
            // else if (passage_file_attente->next_passage != NULL) {
            //     passage_file_attente = passage_file_attente->next_passage;
            //     }
            compteur++;
            if (passage_ajoute == 1) {current = current->next_passage;passage_ajoute = 0;}
            // printf("file:%d liste:%d\n",passage_file_attente->next_passage==NULL,current->next_passage == NULL);
        }
        //traiter le dernier élément
        if (passage_ajoute == 0) { // distinguer le cas où un élément a été ajouté et traiter les cas de modification de la liste
        if (passage_file_attente->status_passage == 0) {//si l'élément dans la file d'attente a un tick plus faible que le prochain élément de la liste et que la voiture doit partir (diapo 10)
            if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
            connect(file_d_attente,0,current,0);
            passage_ajoute = 1;
            compteur--;
        }
        else if (passage_file_attente->status_passage == 1 && current->places_restantes > 0) {//si l'élément de la file d'attente a un tick plus faible, que la voiture arrive et qu'il reste de la place (diapo 36-37)
            if (passage_file_attente->next_passage != NULL) {passage_file_attente = passage_file_attente->next_passage;}
            connect(file_d_attente,0,current,1);
            passage_ajoute = 1;
            compteur--;
        }
        }
    }
    else if (current->next_passage != NULL && file_d_attente->head == NULL) {
        switch (current->next_passage->status_passage) {//mets à jour le nombre de places d'après
        case 1:
            current->next_passage->places_restantes = current->places_restantes-1;
            break;
        
        case 0:
            current->next_passage->places_restantes = current->places_restantes+1;
            break;

        default:
            break;
        }
    }
    passage_destroy(tampon);
    passage_list_destroy(file_d_attente);
    // if (borneId == 6 && id_voiture == 104) {affichage_liste_passages(list_bornes[borneId].list_passages,-1);}
    return find_tick_sortie(list_bornes[borneId].list_passages,id_voiture);
}

void connect(passage_voiture_head* one_file, int element_number, passage_voiture* base_element, int type_ajout) {
    // get element element_number of list one_file and insert it after base_element
    passage_voiture* tampon = creer_passage(0,0,0,0);
    tampon->next_passage = passage_voiture_head_pop_i(one_file, element_number);
    tampon->next_passage->next_passage = base_element->next_passage;
    switch (type_ajout)
    {
    case 0:
        tampon->next_passage->places_restantes = base_element->places_restantes+1; // actualise les places de l'élément ajouté (car la modification de parcours est faite avant le traitement des élements de la file)
        break;
    
    case 1:
        tampon->next_passage->places_restantes = base_element->places_restantes-1;
        break;
    
    case 3: 
        tampon->next_passage->status_passage = 1; //retour de 3 à 1 pour le type de passage
        if (update_passage(one_file,tampon->next_passage->id_voiture,base_element->tick-tampon->next_passage->tick) == 0) {//si la valeur de retour est 0, cela veut dire que le passage de sortie n'est pas dans la file d'attente et qu'il faudra le supprimer de la liste des passages (diapo 39)
            printf("Simulation error : the list did not have the elements needed\n");
        }
        tampon->next_passage->tick = base_element->tick;
        tampon->next_passage->places_restantes = base_element->places_restantes-1;
    default:
        break;
    }
    base_element->next_passage = tampon->next_passage;
    if (tampon->next_passage->tick < base_element->tick) {tampon->next_passage->tick = base_element->tick;}
    
    free(tampon);
}

void change_sortie_attente(passage_voiture_head* file_d_attente, int id_voiture) {
    passage_voiture* current = file_d_attente->head;
    while (current->next_passage != NULL && (current->id_voiture != id_voiture || current->status_passage != 0)) {
        current = current->next_passage;
    }
    if (current->id_voiture == id_voiture && current->status_passage == 0) {
        current->status_passage = 4;
    }
}

int find_tick_sortie(passage_voiture_head* liste_passages, int id_voiture) {
    passage_voiture* current = liste_passages->head;
    while (current->next_passage != NULL && (current->id_voiture != id_voiture || current->status_passage != 0)) {
        current = current->next_passage;
    }
    if (current->id_voiture == id_voiture && current->status_passage == 0) {return current->tick;}
    return 0;
}

int remove_passage0(passage_voiture_head* liste_passages, int id_voiture) {
    passage_voiture* current = liste_passages->head;
    int return_value = 0;
    if (current->next_passage == NULL) {return -1;}
    while (current->next_passage->next_passage != NULL && (current->next_passage->status_passage != 0 || current->next_passage->id_voiture != id_voiture)) {
        current = current->next_passage;
    }
    // printf("(remove_passage0) %d %d -- %d\n",current->next_passage->id_voiture, id_voiture, current->next_passage->status_passage);
    if (current->next_passage->id_voiture == id_voiture && current->next_passage->status_passage == 0) {
        passage_voiture* tampon =  creer_passage(0,0,0,0);
        tampon->next_passage = current->next_passage;
        current->next_passage = current->next_passage->next_passage;
        return_value = tampon->next_passage->tick;
        free(tampon->next_passage);
        free(tampon);
    }
    return return_value;
}

int length(passage_voiture_head* one_list) {
    if (one_list->head == NULL) {return 0;}
    passage_voiture* current = one_list->head;
    int size = 1;
    while (current->next_passage != NULL) {
        size++;
        current = current->next_passage;
    }
    return size;
}

void affichage_liste_passages(passage_voiture_head* one_list, int place) {
    if (one_list->head == NULL) {printf("----Empty----\n");return;}
    passage_voiture* current = one_list->head;
    int i = 0;
    printf("id|tick|status|places\n");
    while (current->next_passage != NULL) {
        printf("%d|%d|%d|%d",current->id_voiture,current->tick,current->status_passage,current->places_restantes);
        if (i == place) {printf("  <---\n");}
        else {printf("\n");}
        current = current->next_passage;
        i++;
    }
    printf("%d|%d|%d|%d",current->id_voiture,current->tick,current->status_passage,current->places_restantes);
    if (i == place) {printf("  <---\n");}
    else {printf("\n");}
    printf("-------------------------------------\n");
}

int update_passage(passage_voiture_head* file_d_attente, int id_voiture, int difference){
    //décale tous les passages de la voiture id_voiture
    if (file_d_attente->head == NULL) {return 0;}
    passage_voiture* current = file_d_attente->head;
    int present = 0;
    while (current->next_passage != NULL) {
        if (current->id_voiture == id_voiture) {
            present = 1;
            if (current->status_passage == 4) {current->tick = current->tick+difference; current->status_passage = 0;}
        }
        current = current->next_passage;
    }
    if (current->id_voiture == id_voiture) {
        present = 1;
        if (current->status_passage == 4) {current->tick = current->tick+difference; current->status_passage = 0;}
    }
    return present;
}

passage_voiture* creer_passage(int id_voiture, int status_passage, int places_restantes, int tick){
    passage_voiture* new_passage = (passage_voiture*) malloc(sizeof(passage_voiture));
    new_passage->next_passage = NULL;
    new_passage->status_passage = status_passage;
    new_passage->places_restantes = places_restantes;
    new_passage->tick = tick;
    new_passage->id_voiture = id_voiture;
    return new_passage;
}

void passage_voiture_head_append(passage_voiture_head* one_list, int id_voiture, int status_passage, int places_restantes, int tick) {
    passage_voiture* new_passage = creer_passage(id_voiture, status_passage, places_restantes, tick);
    if (one_list->head != NULL) {
        passage_voiture* current = one_list->head;
        while (current->next_passage != NULL){
            current = current->next_passage;
        }
        current->next_passage = new_passage;
    }
    else {
        one_list->head = new_passage;
    }
}

passage_voiture* passage_voiture_head_pop(passage_voiture_head* one_list){
    passage_voiture* first_element = one_list->head;
    if (first_element->next_passage == NULL){//dernier élément de la liste
        one_list->head = NULL;
    }
    else {
        one_list->head = first_element->next_passage;
    }
    first_element->next_passage = NULL;
    return first_element;
}

passage_voiture* passage_voiture_head_pop_i(passage_voiture_head* one_list, int i){
    passage_voiture* current = one_list->head;
    passage_voiture* retour = one_list->head;
    if (i == 0 || one_list->head->next_passage == NULL) {//premier élément ou que la liste ne comporte qu'un élément
        retour = passage_voiture_head_pop(one_list);
        return retour;
    }
    int compteur = 0;
    while (current->next_passage != NULL && compteur < i-1) {
        compteur++;
        current = current->next_passage;
    }
    retour = current->next_passage;
    current->next_passage = current->next_passage->next_passage;
    return retour;
}

int temps_recharge(voiture* voiture, borne* borne) {
    if (voiture->temps_recharge_max_minutes != 0){
        int i = round(voiture->temps_recharge_max_minutes/10);
        if (i == 0){ i = 1; }
        return i;
    }
    int puissance_a_charger = voiture->puissance-voiture->puissance_actuelle;
    double temps = (double) puissance_a_charger/borne->puissance_nominale;
    int nombre_tick = (int) round(temps/6);
    if (nombre_tick == 0) {
        nombre_tick = 1;
    }
    return nombre_tick;
}

int temps_trajet(borne_and_distance* proche) {
    double duree = proche->distance_debut/100; //distance/vitesse
    int nombre_ticks = (int) round(duree*6);
    if (nombre_ticks == 0) {
        nombre_ticks = 1;
    }
    return nombre_ticks;
}

void affiche_export_data_list(export_data_list* one_list, int n) {
    if (one_list->head == NULL) {return;}
    export_data_el* current = one_list->head;
    printf("id|status\n");
    while (current->next != NULL) {
        printf("%d|%d\n",n,current->status_borne);
        current = current->next;
    }
    printf("%d|%d\n",n,current->status_borne);
}

void export(borne_simulation* bornes, int* trajets_finis){
    export_data* tab_export = (export_data*) malloc(TOTAL_TICK*sizeof(export_data));
    for (int i=0;i<TOTAL_TICK;i++) {
        tab_export[i].total_entries=0;
        tab_export[i].list_passages = malloc(sizeof(passage_voiture_head));
        tab_export[i].list_passages->head = NULL;
    }
    passage_voiture* current;
    list_int_head* voitures_en_attente = (list_int_head*) malloc(sizeof(list_int_head));
    voitures_en_attente->head = NULL;
    int tick;
    int status;
    // printf("\n EXPORT \n");
    // affichage_liste_passages(bornes[5106].list_passages,-1);
    for (int i=0;i<TOTAL_BORNES;i++) {//boucle pour voir l'ensemble des listes de passage
        // if (i%1000 == 0) {printf("%d %%\n",i*100/TOTAL_BORNES);}
        tick = 0;
        if (bornes[i].list_passages->head != NULL) { //si il y a au moins un passage
            // if (i==6) {affichage_liste_passages(bornes[i].list_passages);}
            current = bornes[i].list_passages->head;
            tick = current->tick;
            status = current->status_passage;
            while (current->next_passage != NULL && current->next_passage->tick == current->tick) {
                switch (current->status_passage) {
                case 2:
                    if (!int_est_dans(voitures_en_attente,current->id_voiture)) {list_int_append(voitures_en_attente,current->id_voiture);}
                    break;
                case 1:
                    if (int_est_dans(voitures_en_attente, current->id_voiture)) {remove_list_int(voitures_en_attente, current->id_voiture);}
                    break;
                default:
                    break;
                }
                current = current->next_passage;
            }
            switch (current->status_passage) {
            case 2:
                if (!int_est_dans(voitures_en_attente,current->id_voiture)) {list_int_append(voitures_en_attente,current->id_voiture);}
                status = 2;
                break;
            case 1:
                if (int_est_dans(voitures_en_attente, current->id_voiture)) {remove_list_int(voitures_en_attente, current->id_voiture);}
                if (voitures_en_attente->head == NULL) {status = 1;}
                else {status = 2;}
                break;
            case 0:
                if (current->places_restantes == bornes[i].capacite_max) {status = 0;}
                break;
            }

            while (tick<TOTAL_TICK && current->next_passage != NULL) {//////////////////////////
                // if (i == 5106) {printf("%d ",status);}
                // if (i == 6) {printf("vide:%d\n",voitures_en_attente->head == NULL);}

                // if (status == 2) {printf("borne :%d\n",i);}
                // if (i == 11414) {printf("(%d) : %d \n",tick,status);}
                // if (status == 2) {printf("%d\n",i);}
                if (status != 0 && current->tick <= tick) {
                    data_append(&tab_export[tick], &bornes[i].coordonnees, status);
                }
                switch (current->status_passage) {
                case 2:
                    if (!int_est_dans(voitures_en_attente,current->id_voiture)) {list_int_append(voitures_en_attente,current->id_voiture);}
                    status = 2;
                    break;
                case 1:
                    if (int_est_dans(voitures_en_attente, current->id_voiture)) {remove_list_int(voitures_en_attente, current->id_voiture);}
                    if (voitures_en_attente->head == NULL) {status = 1;}
                    else {status = 2;}
                    break;
                case 0:
                    if (current->places_restantes == bornes[i].capacite_max) {status = 0;}
                    break;
                }
                while (current->next_passage != NULL && current->next_passage->tick <= tick+1) { // A Changer
                    switch (current->status_passage) {
                    case 2:
                        if (!int_est_dans(voitures_en_attente,current->id_voiture)) {list_int_append(voitures_en_attente,current->id_voiture);}
                        status = 2;
                        break;
                    case 1:
                        if (int_est_dans(voitures_en_attente, current->id_voiture)) {remove_list_int(voitures_en_attente, current->id_voiture);}
                        if (voitures_en_attente->head == NULL) {status = 1;}
                        else {status = 2;}
                        break;
                    case 0:
                        if (current->places_restantes == bornes[i].capacite_max) {status = 0;}
                        break;
                    }
                        // if (tick == 39) {affiche_export_data_list(tab_export[39].list_passages, 39);}
                    current = current->next_passage;
                }
                switch (current->status_passage) {
                case 2:
                    if (!int_est_dans(voitures_en_attente,current->id_voiture)) {list_int_append(voitures_en_attente,current->id_voiture);}
                    status = 2;
                    break;
                case 1:
                    if (int_est_dans(voitures_en_attente, current->id_voiture)) {remove_list_int(voitures_en_attente, current->id_voiture);}
                    if (voitures_en_attente->head == NULL) {status = 1;}
                    else {status = 2;}
                    break;
                case 0:
                    if (current->places_restantes == bornes[i].capacite_max) {status = 0;}
                    break;
                }
                tick++;
            }
            while (tick<current->tick) { //dernier élément à traiter
                if (status != 0) {
                    data_append(&tab_export[tick], &bornes[i].coordonnees, status);
                }
                // if (tick == 39) {affiche_export_data_list(tab_export[39].list_passages, 39);}
                tick++;
            }
            if (voitures_en_attente->head != NULL) {
                destroy_list_int_el(voitures_en_attente->head);
                voitures_en_attente->head = NULL;
            }
            // tab_export[tick].total_entries++;
        }

    }

    export_data_el* current_w;
    FILE *fichier = fopen("../data/simulation.txt","w");
    int total_trajet_finis = 0;
    for (int i=0;i<TOTAL_TICK;i++) {
        if (tab_export[i].list_passages->head != NULL) {
            current_w = tab_export[i].list_passages->head;
            total_trajet_finis = total_trajet_finis + trajets_finis[i];
            fprintf(fichier,"#%d$0$0#", total_trajet_finis);
            while (current_w->next != NULL) {
                fprintf(fichier,"%Lf$%Lf$%d#",current_w->coordonnees->longitude, current_w->coordonnees->latitude, current_w->status_borne);
                current_w = current_w->next;
            }
            fprintf(fichier,"%Lf$%Lf$%d#",current_w->coordonnees->longitude, current_w->coordonnees->latitude, current_w->status_borne);
        }
        else{
            total_trajet_finis = total_trajet_finis + trajets_finis[i];
            fprintf(fichier,"#%d$0$0#", total_trajet_finis);
        }
        fprintf(fichier,"\n");
    }

    fclose(fichier);
    free(voitures_en_attente);
    destroy_data(tab_export);
    return;
}

void destroy_data_el(export_data_el* one_element) {
    if (one_element->next != NULL) {
        destroy_data_el(one_element->next);
    }
    free(one_element);
}

void destroy_data_list(export_data_list* one_data_list) {
    if (one_data_list->head != NULL) {
        destroy_data_el(one_data_list->head);
    }
    free(one_data_list);
}

void destroy_data(export_data* tab){
    for (int i=0;i<TOTAL_TICK;i++) {
        destroy_data_list(tab[i].list_passages);
    }
    free(tab);
}

int est_dans(export_data* data, coord_pt* coordonnes) {
    if (data->list_passages == NULL) {
        return 0;
    }
    export_data_el* current = data->list_passages->head;
    while (current->next != NULL) {
        if (current->coordonnees->latitude == coordonnes->latitude && current->coordonnees->longitude == coordonnes->longitude) {
            return current->status_borne;
        }
        current = current->next;
    }
    return 0;
}

void list_int_append(list_int_head* one_list, int value){
    list_int* new_element = (list_int*) malloc(sizeof(list_int));
    new_element->entier = value;
    new_element->next_value = NULL;
    if (one_list->head == NULL) {
        one_list->head = new_element;
    }
    else {
        list_int* current = one_list->head;
        while (current->next_value != NULL) {
            current = current->next_value;
        }
        current->next_value = new_element;
    }
}

void update_data(export_data* data, coord_pt* coordonnees, int status_borne){
    if (data->list_passages->head != NULL) {
        export_data_el* current = data->list_passages->head;
        while (current->next != NULL && !(current->coordonnees->latitude == coordonnees->latitude && current->coordonnees->longitude == coordonnees->longitude)) {
            current = current->next;
        }
        if (current->coordonnees->latitude == coordonnees->latitude && current->coordonnees->longitude == coordonnees->longitude) {
            current->status_borne = status_borne;
        }
    }
}

int int_est_dans(list_int_head * one_list, int value) {
    if (one_list->head == NULL) {return 0;}
    list_int* current = one_list->head;
    while (current->next_value != NULL && current->entier != value) {
        current = current->next_value;
    }
    if (current->entier == value) {
        return 1;
    }
    return 0;
}

void remove_list_int(list_int_head* one_list, int value){
    if (one_list->head == NULL) {return;}
    list_int* tampon = (list_int*) malloc(sizeof(list_int));
    tampon->next_value = NULL;
    if (one_list->head->entier == value) {
        tampon->next_value = one_list->head;
        one_list->head = one_list->head->next_value;
        free(tampon->next_value);
    }
    else {
        list_int* current = one_list->head;
        while (current->next_value != NULL && current->next_value->entier != value) {
            current = current->next_value;
        }
        if (current->next_value != NULL && current->next_value->entier == value) {
            tampon->next_value = current->next_value;
            current->next_value = current->next_value->next_value;
            free(tampon->next_value);
        }
    }
    free(tampon);
}

void destroy_list_int(list_int_head* one_list) {
    if (one_list->head != NULL) {
        destroy_list_int_el(one_list->head);
    }
    free(one_list);
}

void destroy_list_int_el(list_int* one_element) {
    if (one_element->next_value != NULL) {
        destroy_list_int_el(one_element->next_value);
    }
    free(one_element);
}

void data_append(export_data* data, coord_pt* coordonnees, int status_passage) {
    export_data_el* current = data->list_passages->head;
    export_data_el* new_state = (export_data_el*) malloc(sizeof(export_data_el));
    new_state->coordonnees = coordonnees;
    new_state->next = NULL;
    new_state->status_borne = status_passage;
    if (data->list_passages->head != NULL) {
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_state;
    }
    else {
        data->list_passages->head = new_state;
    }
}

void destroy_tab(borne_simulation* tab){
    for (int i=0;i<TOTAL_BORNES;i++){
        passage_list_destroy(tab[i].list_passages);
    }
    free(tab);
}

void tab_tick_destroy(list_int* tab_tick) {
    if (tab_tick != NULL) {
        if (tab_tick->next_value != NULL) {
            tab_tick_destroy(tab_tick->next_value);
            free(tab_tick);
        }
    }
}

void passage_destroy(passage_voiture* passage) {
    if (passage->next_passage != NULL) {
        passage_destroy(passage->next_passage);
    }
    free(passage);
}

void passage_list_destroy(passage_voiture_head* passages){
    if (passages->head != NULL) {
        if (passages->head->next_passage != NULL) {
            passage_destroy(passages->head->next_passage);
        }
        free(passages->head);
    }
    free(passages);
}

// int main(void) {
//     simulation();
//     return 0;
// }
