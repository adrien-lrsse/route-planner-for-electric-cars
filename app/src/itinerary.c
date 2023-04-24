#include "database_reader.h"
#include "coordinates.h"
#include "borne.h"
#include "vehicules.h"
#include "itinerary.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "etape.h"
#include <unistd.h>

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b)  ((a) < (b) ? (a) : (b))

#define LARGEUR_BANDE(a,b) (fabsl(b-a))


list_position* getBorneFromDistance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee){
    // Entrée : Coord  de départ et Coord d'arrivée
    // Sortie : Liste de toutes les bornes avec comme données : 
    // pour chaque borne i : (i : distance_départ_borne, distance_borne_arrivée)

    list_position* retour = list_create(); // Création de la liste de borne qui sera retourné.


  

    
    // Ouverture de la base de données
    database_t * database = open_database("../data/database_clean.db");
    if (!database->opened_correctly) {
        exit(0);
    }
    char* sql_commmand;
    if (LARGEUR_BANDE(latitude_depart,latitude_arrivee) || LARGEUR_BANDE(longitude_arrivee,longitude_depart)){
        sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes WHERE (consolidated_longitude BETWEEN (?-0.5)  AND (?+0.5)) AND (consolidated_latitude BETWEEN (?-0.5)  AND (?+0.5)) ";

    } else {
        sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes WHERE (consolidated_longitude BETWEEN ?  AND ?) AND (consolidated_latitude BETWEEN ?  AND ?) ";

    }

    // Préparation de la requête

    prepare_request_database(database, sql_commmand); // préparation de la requête
    sqlite3_bind_double(database->stmt, 1, MIN(longitude_depart,longitude_arrivee)); //bind des valeurs "?"
    sqlite3_bind_double(database->stmt, 2, MAX(longitude_arrivee,longitude_depart));
    sqlite3_bind_double(database->stmt, 3, MIN(latitude_depart,latitude_arrivee));
    sqlite3_bind_double(database->stmt, 4,MAX(latitude_arrivee,latitude_depart)); 

    int rc;
    if ((rc = sqlite3_step(database->stmt)) == SQLITE_DONE)
    {
        printf("Pas de borne dans cette zone\n\n\n");
        end_request_database(database);
        close_database(database);
        return retour;
    }
    else
    {

    // Déclaration des variables nécessaires pour la lecture des résultats de la requête
    long double longitude;
    long double latitude;
    long double distance_depart_borne;
    long double distance_borne_arrivee;
    int id_unique;
    
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat

        //Lecture du résultat de la requête
        longitude = sqlite3_column_double(database->stmt, 0); // la longitude
        latitude = sqlite3_column_double(database->stmt, 1); // la latitude
        id_unique = sqlite3_column_int(database->stmt, 2); // l'identifiant unique
        // Traitement
        distance_depart_borne = distance(longitude_depart,latitude_depart,longitude,latitude);  // calcul de la distance départ -> borne
        distance_borne_arrivee = distance(longitude,latitude,longitude_arrivee,latitude_arrivee); // calcul de la distance borne -> arrivée
        add(&retour,distance_depart_borne,distance_borne_arrivee,id_unique, 0); // ajout des résultat de la borne i à la liste de retour
    }
    // Fermeture de la base

    end_request_database(database);
    close_database(database);
    return retour;
        
    }
}


list_position* getBorneFromDistance_type_temps(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee){
    // Entrée : Coord  de départ et Coord d'arrivée
    // Sortie : Liste de toutes les bornes avec comme données : 
    // pour chaque borne i : (i : distance_départ_borne, distance_borne_arrivée)

    list_position* retour = list_create(); // Création de la liste de borne qui sera retourné.


  

    
    // Ouverture de la base de données
    database_t * database = open_database("../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }
    char* sql_commmand;
    if (LARGEUR_BANDE(latitude_depart,latitude_arrivee) || LARGEUR_BANDE(longitude_arrivee,longitude_depart)){
        sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique, puissance_nominale FROM bornes WHERE (consolidated_longitude BETWEEN (?-0.5)  AND (?+0.5)) AND (consolidated_latitude BETWEEN (?-0.5)  AND (?+0.5)) ";

    } else {
        sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique, puissance_nominale FROM bornes WHERE (consolidated_longitude BETWEEN ?  AND ?) AND (consolidated_latitude BETWEEN ?  AND ?) ";

    }

    // Préparation de la requête

    prepare_request_database(database, sql_commmand); // préparation de la requête
    sqlite3_bind_double(database->stmt, 1, MIN(longitude_depart,longitude_arrivee)); //bind des valeurs "?"
    sqlite3_bind_double(database->stmt, 2, MAX(longitude_arrivee,longitude_depart));
    sqlite3_bind_double(database->stmt, 3, MIN(latitude_depart,latitude_arrivee));
    sqlite3_bind_double(database->stmt, 4,MAX(latitude_arrivee,latitude_depart)); 

    int rc;
    if ((rc = sqlite3_step(database->stmt)) == SQLITE_DONE)
    {
        printf("Pas de borne dans cette zone\n\n\n");
        end_request_database(database);
        close_database(database);
        return retour;
    }
    else
    {

    // Déclaration des variables nécessaires pour la lecture des résultats de la requête
    long double longitude;
    long double latitude;
    long double distance_depart_borne;
    long double distance_borne_arrivee;
    int id_unique;
    int puissance_nominale;
    
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat

        //Lecture du résultat de la requête
        longitude = sqlite3_column_double(database->stmt, 0); // la longitude
        latitude = sqlite3_column_double(database->stmt, 1); // la latitude
        id_unique = sqlite3_column_int(database->stmt, 2); // l'identifiant unique
        puissance_nominale = sqlite3_column_int(database->stmt, 3); // puissance de la borne

        // Traitement
        distance_depart_borne = distance(longitude_depart,latitude_depart,longitude,latitude);  // calcul de la distance départ -> borne
        distance_borne_arrivee = distance(longitude,latitude,longitude_arrivee,latitude_arrivee); // calcul de la distance borne -> arrivée

        add(&retour,distance_depart_borne,distance_borne_arrivee,id_unique, puissance_nominale); // ajout des résultat de la borne i à la liste de retour
    }
    // Fermeture de la base

    end_request_database(database);
    close_database(database);
    return retour;
        
    }
}


void add(list_position** list, double depart, double arrivee, int id_unique, int puissance){
    // Entrée : une liste, la distance départ -> borne, la distance borne -> arrivée, l'id de la borne en question.
    // Sortie : ajoute de l'élement (id, départ->borne, borne->arrivé) à list

    // cas ou la liste est vide
    if (list_is_empty(*list)){
        list_position* new_item = malloc(sizeof(list_position));
        new_item->next = NULL;
        new_item->node.indice_unique = id_unique;
        new_item->node.distance_depart = depart;
        new_item->node.distance_arrivee = arrivee;
        new_item->node.puissance = puissance;
        *list = new_item;
    }
    // cas ou la liste est non vide
    else {
        list_position* i = *list;
        while(i->next != NULL){
            i = i->next;
        }
        list_position* new_item = malloc(sizeof(list_position));
        new_item->next = NULL;
        new_item->node.indice_unique = id_unique;
        new_item->node.distance_depart = depart;
        new_item->node.distance_arrivee = arrivee;
        new_item->node.puissance = puissance;
        i->next = new_item;
    }
    
}


void print_position(list_position* list){
    list_position* i = list;
    while(i->next != NULL){
        printf("%d : distance_depart : %f / distance_arrive : %f\n", i->node.indice_unique,i->node.distance_depart,i->node.distance_arrivee);
        i = i->next;
    }   
}


list_position* list_create(void){
    // création d'une liste
    return NULL;
}


void list_destroy(list_position* one_list){
    // permet de free les adresses allouées pour one_list
    list_position* i = one_list;
    while (i!=NULL){
        list_position* temp = i;
        i = i->next;
        free(temp);
    } 
}


bool list_is_empty(list_position* one_list){
    // Entrée : une liste de type list_position
    // Permet de savoir si une liste est vide ou non
    // Sortie : un booléen
    return (one_list == NULL);
}


borne_and_distance plus_proche(list_position* one_list, double autonomie, list_bornes_visitees* bornes_visitees){
    // Entrée : liste de borne avec leur distance, autonomie du véhicule
    // Sortie : point le plus proche de l'arrivée qui respecte l'autonomie du véhicule
    long double max_distance = -1.0L;
    long double min_distance = 200000.0L;
    borne_and_distance selectionner;
    int ind = -1;
    list_position* i = one_list;
    while (i->next!=NULL)
    {
        // si on trouve un point plus proche de l'arrivée et qui ne nécessite pas plus de kilomètre que l'autonomie, et qui n'a pas déjà été visité, alors c'est le nouveau point le proche
        if(i->node.distance_depart<autonomie && i->node.distance_arrivee<min_distance && borne_deja_visitee(bornes_visitees, i->node.indice_unique)==0){
            max_distance = i->node.distance_depart;
            min_distance = i->node.distance_arrivee;
            ind = i->node.indice_unique;
            
        }
        i = i->next;
    }
    if (ind == -1){
        selectionner.borne.id = -1;
    }
    else {
    selectionner.borne =  getInfo(ind); // recupère les infos du points
    selectionner.distance_debut = max_distance;
    selectionner.distance_fin = min_distance;
    }
    return selectionner;
    
}


borne_and_distance plus_proche_type_temps(list_position* one_list, double autonomie, list_bornes_visitees* bornes_visitees, voiture* one_car){
    // Entrée : liste de borne avec leur distance, autonomie du véhicule
    // Sortie : point le plus proche de l'arrivée qui respecte l'autonomie du véhicule
    long double max_distance = -1.0L;
    long double min_distance = 200000.0L;
    long double mini = 200000.0L;
    double autonomie_apres_charge = 0.0;
    long double dist_fin_apres_charge = 0.0L;
    borne_and_distance selectionner;
    int ind = -1;
    list_position* i = one_list;
    while (i->next!=NULL)
    {
        // si on trouve un point plus proche de l'arrivée et qui ne nécessite pas plus de kilomètre que l'autonomie, et qui n'a pas déjà été visité, alors c'est le nouveau point le proche
        if(i->node.distance_depart<autonomie && borne_deja_visitee(bornes_visitees, i->node.indice_unique)==0){
            autonomie_apres_charge = simulation_recharge(one_car, i->node.puissance, i->node.distance_depart);
            dist_fin_apres_charge = i->node.distance_arrivee - autonomie_apres_charge;
            if (dist_fin_apres_charge<0){
                dist_fin_apres_charge = 0;
            }
            if (dist_fin_apres_charge<mini){
                mini = dist_fin_apres_charge;
                max_distance = i->node.distance_depart;
                min_distance = i->node.distance_arrivee;
                ind = i->node.indice_unique;
            }
        }
        i = i->next;
    }
    if (ind == -1){
        selectionner.borne.id = -1;
    }
    else {
    selectionner.borne =  getInfo(ind); // recupère les infos du points
    selectionner.distance_debut = max_distance;
    selectionner.distance_fin = min_distance;
    }
    return selectionner;
    
}


borne getInfo(int i){
    // Entrée : Identifiant unique d'une borne
    // Sortie : Information sur la borne i
    borne res;

    // Ouverture de la base
    database_t * database = open_database("./../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }

    // Préparation de la requête
    char* name;
    long double longitude;
    long double latitude;
    int puissance_nominale;

    char* sql_commmand = "SELECT nom_station, consolidated_longitude,consolidated_latitude,puissance_nominale FROM bornes WHERE id_unique = ?";
    prepare_request_database(database,sql_commmand);
    sqlite3_bind_int(database->stmt, 1, i); // on bind la valeur de "?" dans la requête
    sqlite3_step(database->stmt);

    // Traitement de la sortie
    name =  (char*)sqlite3_column_text(database->stmt, 0); // nom de la station
    longitude = sqlite3_column_double(database->stmt, 1); // longitude de la station
    latitude = sqlite3_column_double(database->stmt,2); // latitude de la station
    puissance_nominale = sqlite3_column_int(database->stmt,3); // puissance nominale de la station

    // Copie de la chaine de caractères
    char* nom_station = calloc((strlen(name)+1),sizeof(char));
    strcpy(nom_station,name);
    res.name = nom_station;

    res.coordonnees.latitude = latitude;
    res.coordonnees.longitude = longitude;
    res.id = i;
    res.puissance_nominale = puissance_nominale;

    //Fermeture de la base
    end_request_database(database);
    close_database(database);

    return res;
}


etape* get_liste_etape_itineaire(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, int type, list_bornes_visitees* bornes_visitees){
    /*
    Arguments : 
        - latitude_depart : latitude du point de départ
        - longitude_depart : longitude du point de départ
        - latitude_arrivee : latitude du point d'arrivée
        - longitude_arrivee : longitude du point d'arrivée
        - autonomie : autonomie du véhicule
        - temps_max_attente_borne : temps maximum d'attente à une borne
        - type : 1 pour distance, 2 pour temps
        - bornes_visitees: liste des bornes déjà visitées par l'algorithme
    */
    if (type == 1){
        return get_liste_etape_itineaire_type_distance(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, one_car, bornes_visitees);
    }
    else if (type == 2){
        return get_liste_etape_itineaire_type_temps(latitude_depart, longitude_depart, latitude_arrivee, longitude_arrivee, one_car, bornes_visitees);
    }
    else {
        return NULL;
    }
}


etape* get_liste_etape_itineaire_type_distance(long double latitude_depart,long double longitude_depart,long double latitude_arrivee,long double longitude_arrivee,voiture* one_car, list_bornes_visitees* bornes_visitees){
    // Entrée : Coordonnées de départ et d'arrivée'
    // Sortie : Liste des étapes pour atteindre l'arrivée

    // Initialise la distance borne -> arrivée (au début borne = départ)
    double distance_fin = distance(longitude_depart,latitude_depart,longitude_arrivee,latitude_arrivee); 
    
    borne_and_distance depart;
    depart.distance_fin = distance_fin;
    depart.distance_debut = 0.0;
    depart.borne.coordonnees.latitude = latitude_depart;
    depart.borne.coordonnees.longitude = longitude_depart;
    depart.borne.name = "Départ";
    depart.borne.id = -1;

    borne_and_distance arrivee_etape;
    arrivee_etape.distance_fin = 0.0;
    arrivee_etape.distance_debut = distance_fin;
    arrivee_etape.borne.coordonnees.latitude = latitude_arrivee;
    arrivee_etape.borne.coordonnees.longitude = longitude_arrivee;
    arrivee_etape.borne.name = "Arrivée";



    etape* lst_etape = etape_create(); // valeur de retour
    borne_and_distance proche;
    etape_add(lst_etape,depart);
    bool arrivee = false;
    // Tant que la distance borne -> arrivé n'est pas nulle il reste au moins une étape
    while (distance_fin != 0.0 && !arrivee)
    {
        if(distance_fin<one_car->autonomie_actuelle){
            etape_add(lst_etape,arrivee_etape);
            arrivee = true;
        } 
        else{
            // Calcul du point le proche de l'arrivée atteignable avec l'autonomie du véhicule en fonction du point traité
            list_position* resultat = getBorneFromDistance(latitude_depart,longitude_depart,latitude_arrivee,longitude_arrivee);
            if (list_is_empty(resultat))
            {
                list_destroy(resultat);
                etape_destroy(lst_etape);
                return etape_create();

            }
            else {
                proche = plus_proche(resultat,one_car->autonomie_actuelle, bornes_visitees);
                if (proche.borne.id == -1){
                    list_destroy(resultat);
                    etape_destroy(lst_etape);
                    return etape_create();
                }
                else {
                    etape_add(lst_etape,proche);

                    //ajouter cette borne à la liste des bornes visitées
                    list_bornes_visitees_append(bornes_visitees, proche.borne.id);

                    // printf("Distance parcourue %f km \n",proche.distance_debut);
                    update_charge(one_car,proche.distance_debut);
                    recharge(one_car,proche.borne.puissance_nominale);
                    // printf("La voiture a une autonomie de %f km\n",one_car->autonomie_actuelle);
                    // printf("La borne a une puissance de : %d kW\n",proche.borne.puissance_nominale);
                    // printf("----------------------------------------------------\n");

                    // Nouvelle distance de fin : borne_atteinte -> arrivée
                    distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,longitude_arrivee,latitude_arrivee);
                    latitude_depart = proche.borne.coordonnees.latitude;
                    longitude_depart = proche.borne.coordonnees.longitude;

            
                    // On détruit l'ancienne list_position
                    list_destroy(resultat);
                    free(proche.borne.name);
                }
            }
            
        }

    }


    return lst_etape;
}


etape* get_liste_etape_itineaire_type_temps(long double latitude_depart, long double longitude_depart, long double latitude_arrivee, long double longitude_arrivee, voiture* one_car, list_bornes_visitees* bornes_visitees){
    // Entrée : Coordonnées de départ et d'arrivée'
    // Sortie : Liste des étapes pour atteindre l'arrivée

    // Initialise la distance borne -> arrivée (au début borne = départ)
    double distance_fin = distance(longitude_depart,latitude_depart,longitude_arrivee,latitude_arrivee); 
    

    borne_and_distance depart;
    depart.distance_fin = distance_fin;
    depart.distance_debut = 0.0;
    depart.borne.coordonnees.latitude = latitude_depart;
    depart.borne.coordonnees.longitude = longitude_depart;
    depart.borne.name = "Départ";
    depart.borne.id = -1;

    borne_and_distance arrivee_etape;
    arrivee_etape.distance_fin = 0.0;
    arrivee_etape.distance_debut = distance_fin;
    arrivee_etape.borne.coordonnees.latitude = latitude_arrivee;
    arrivee_etape.borne.coordonnees.longitude = longitude_arrivee;
    arrivee_etape.borne.name = "Arrivée";


    etape* lst_etape = etape_create(); // valeur de retour
    borne_and_distance proche;
    etape_add(lst_etape,depart);
    bool arrivee = false;
    // Tant que la distance borne -> arrivé n'est pas nulle il reste au moins une étape
    while (distance_fin != 0.0 && !arrivee)
    {
        if(distance_fin<one_car->autonomie_actuelle){
            etape_add(lst_etape,arrivee_etape);
            arrivee = true;
        } 
        else{
            // Calcul du point le proche de l'arrivée atteignable avec l'autonomie du véhicule en fonction du point traité
            list_position* resultat = getBorneFromDistance_type_temps(latitude_depart,longitude_depart,latitude_arrivee,longitude_arrivee);
            if (list_is_empty(resultat))
            {
                list_destroy(resultat);
                etape_destroy(lst_etape);
                return etape_create();

            }
            else {
                proche = plus_proche_type_temps(resultat,one_car->autonomie_actuelle, bornes_visitees, one_car);
                if (proche.borne.id == -1){
                    list_destroy(resultat);
                    etape_destroy(lst_etape);
                    return etape_create();
                }
                else {
                    etape_add(lst_etape,proche);

                    //ajouter cette borne à la liste des bornes visitées
                    list_bornes_visitees_append(bornes_visitees, proche.borne.id);

                    // printf("Distance parcourue %f km \n",proche.distance_debut);
                    update_charge(one_car,proche.distance_debut);
                    recharge(one_car,proche.borne.puissance_nominale);
                    // printf("La voiture a une autonomie de %f km\n",one_car->autonomie_actuelle);
                    // printf("La borne a une puissance de : %d kW\n",proche.borne.puissance_nominale);
                    // printf("----------------------------------------------------\n");

                    // Nouvelle distance de fin : borne_atteinte -> arrivée
                    distance_fin = distance(proche.borne.coordonnees.longitude,proche.borne.coordonnees.latitude,longitude_arrivee,latitude_arrivee);
                    latitude_depart = proche.borne.coordonnees.latitude;
                    longitude_depart = proche.borne.coordonnees.longitude;

            
                    // On détruit l'ancienne list_position
                    list_destroy(resultat);
                    free(proche.borne.name);
                }
            }
            
        }

    }

    return lst_etape;
}
