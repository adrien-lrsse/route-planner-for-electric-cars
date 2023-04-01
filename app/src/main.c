#include "main.h"

int main (){
    mat_adj* mat = create_mat_adj(55320); // création de la matrice
    
    database_t * database = open_database("./../data/database.db"); // ouverture de la base de données
    database_t * database_deuxieme = open_database("./../data/database.db"); // on ouvre une 2e base de données pour pouvoir faire des requêtes en parallèle
    if (!database->opened_correctly || !database_deuxieme->opened_correctly) {
        exit(0); // vérification que les databases ont été correctement ouvertes
    }

    char * request = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes"; // requête sql à éxécuter
    prepare_request_database(database, request); // préparation de la requête
    
    int rc;
    double longitude;
    double latitude;
    int id_unique;
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) { // pour chaque résultat
        longitude = sqlite3_column_double(database->stmt, 0); // on récupère les valeurs
        latitude = sqlite3_column_double(database->stmt, 1);
        id_unique = sqlite3_column_int(database->stmt, 2);
        char * request_tmp = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes where id_unique >= ?"; // requête sql à éxécuter pour le 2e train de valeurs
        prepare_request_database(database_deuxieme, request_tmp); 
        sqlite3_bind_int(database_deuxieme->stmt, 1, id_unique); // on bind la valeur de "?" dans la requête
        int rc_tmp;
        double longitude_tmp;
        double latitude_tmp;
        int id_unique_tmp;
        while ((rc_tmp = sqlite3_step(database_deuxieme->stmt)) != SQLITE_DONE) { // pour chaque valeur de la 2e requête, on calcule la distance avec la valeur actuelle
            longitude_tmp = sqlite3_column_double(database_deuxieme->stmt, 0); // on récupère les valeurs
            latitude_tmp = sqlite3_column_double(database_deuxieme->stmt, 1);
            id_unique_tmp = sqlite3_column_int(database_deuxieme->stmt, 2);
            double value = distance(longitude, latitude, longitude_tmp, latitude_tmp); // on calcule la distance
            define(mat,id_unique_tmp,id_unique,value); // on définit la valeur dans la matrice
        }
        end_request_database(database_deuxieme);
    }

    end_request_database(database);
    mat_destroy(mat);
    close_database(database);
    close_database(database_deuxieme);
    return 0;
}
