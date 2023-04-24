#include "vehicules.h"


voiture* create_voiture(int id){
    database_t * database = open_database("./../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }

    char* sql_commmand = "select autonomie_km, puissance_batterie_Wh,nom_vehicule from vehicules where id = ?;";
    prepare_request_database(database,sql_commmand);
    sqlite3_bind_int(database->stmt, 1, id);
    sqlite3_step(database->stmt);

    //Traitement de la sortie
    double autonomie = sqlite3_column_double(database->stmt, 0);
    int puissance = sqlite3_column_int(database->stmt, 1);
    char* name = (char*) sqlite3_column_text(database->stmt, 2);

    voiture* vehicule = (voiture*) malloc(sizeof(voiture));
    vehicule->nom = malloc(sizeof(char)*(strlen(name)+1));
    strcpy(vehicule->nom, name);
    vehicule->id = id;
    vehicule->autonomie = autonomie;
    vehicule->autonomie_actuelle = autonomie;
    vehicule->puissance = puissance;
    vehicule->puissance_actuelle = puissance;
    end_request_database(database);
    close_database(database);
    return vehicule;
}

void destroy_voiture(voiture* vehicule){
    free(vehicule->nom);
    free(vehicule);
}

void update_charge(voiture* vehicule, double distance_parcourue){
    vehicule->autonomie_actuelle = vehicule->autonomie_actuelle - distance_parcourue;
    update_puissance(vehicule);
}

void update_puissance(voiture* vehicule){
    double autonomie_actuelle = vehicule->autonomie_actuelle + vehicule->reserve_equivalent_autonomie;
    double autonomie = vehicule->autonomie;
    int puissance = vehicule->puissance;
    double puissance_actuelle = (autonomie_actuelle/autonomie)*puissance;
    vehicule->puissance_actuelle = (int) (puissance_actuelle);
}

void update_autonomie(voiture * vehicule){
    double puissance_actuelle = vehicule->puissance_actuelle;
    double puissance = vehicule->puissance;
    double autonomie = vehicule->autonomie;
    double autonomie_actuelle = (puissance_actuelle/puissance)*autonomie;
    autonomie_actuelle = autonomie_actuelle - (vehicule->reserve_equivalent_autonomie);
    vehicule->autonomie_actuelle = autonomie_actuelle;
}

void recharge(voiture* vehicule, int puissance_borne){
    if (vehicule->temps_recharge_max_minutes == 0){
        vehicule->puissance_actuelle = vehicule->puissance;
        update_autonomie(vehicule);
        return;
    }
    double charge_par_heure = (double)(puissance_borne);
    double charge = charge_par_heure*(double)(vehicule->temps_recharge_max_minutes)/60.0;
    vehicule->puissance_actuelle = vehicule->puissance_actuelle + charge;
    if (vehicule->puissance_actuelle > vehicule->puissance){
        vehicule->puissance_actuelle = vehicule->puissance;
    }
    update_autonomie(vehicule);
}

void print_info(voiture* vehicule){
    printf("Nom du vehicule : %s\n", vehicule->nom);
    printf("ID : %d\n", vehicule->id);
    printf("Autonomie : %f\n", vehicule->autonomie);
    printf("Autonomie actuelle : %f\n", vehicule->autonomie_actuelle);
    printf("Puissance : %d\n", vehicule->puissance);
    printf("Puissance actuelle : %d\n", vehicule->puissance_actuelle);
    printf("Temps de recharge max : %d\n", vehicule->temps_recharge_max_minutes);
    printf("Equivalent autonomie de la réserve : %f\n", vehicule->reserve_equivalent_autonomie);
}

void set_autonomie(voiture* vehicule, double autonomie){
    vehicule->autonomie_actuelle = autonomie-vehicule->reserve_equivalent_autonomie;
    update_puissance(vehicule);
}

long double simulation_recharge(voiture* vehicule, int puissance, double distance_parcourue){
    if (vehicule->temps_recharge_max_minutes == 0){
        return (long double)(vehicule->autonomie);
    }
    if (distance_parcourue>vehicule->autonomie_actuelle){
        return (long double)(20000.0L);
    }
    voiture * vehicule_simul = create_voiture(vehicule->id);
    vehicule_simul->reserve_equivalent_autonomie = vehicule->reserve_equivalent_autonomie;
    vehicule_simul->temps_recharge_max_minutes = vehicule->temps_recharge_max_minutes;
    vehicule_simul->autonomie_actuelle = vehicule->autonomie_actuelle;
    vehicule_simul->puissance_actuelle = vehicule->puissance_actuelle;
    vehicule_simul->autonomie = vehicule->autonomie;
    vehicule_simul->puissance = vehicule->puissance;
    update_charge(vehicule_simul, distance_parcourue);
    recharge(vehicule_simul, puissance);
    long double autonomie = vehicule_simul->autonomie_actuelle;
    destroy_voiture(vehicule_simul);
    return autonomie;
}
