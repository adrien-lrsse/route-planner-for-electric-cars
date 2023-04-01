#include "main.h"

int main (){
    mat_adj* mat = create_mat_adj(55320);
    
    database_t * database = open_database("./../data/database.db");
    database_t * database_deuxieme = open_database("./../data/database.db");
    if (!database->opened_correctly || !database_deuxieme->opened_correctly) {
        exit(0);
    }

    char * request = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes";
    prepare_request_database(database, request);
    
    int rc;
    double longitude;
    double latitude;
    int id_unique;
    while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) {
        longitude = sqlite3_column_double(database->stmt, 0);
        latitude = sqlite3_column_double(database->stmt, 1);
        id_unique = sqlite3_column_int(database->stmt, 2);
        char * request_tmp = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes where id_unique >= ?";
        prepare_request_database(database_deuxieme, request_tmp);
        sqlite3_bind_int(database_deuxieme->stmt, 1, id_unique);
        int rc_tmp;
        double longitude_tmp;
        double latitude_tmp;
        int id_unique_tmp;
        while ((rc_tmp = sqlite3_step(database_deuxieme->stmt)) != SQLITE_DONE) {
            assert(id_unique_tmp >= id_unique);
            longitude_tmp = sqlite3_column_double(database_deuxieme->stmt, 0);
            latitude_tmp = sqlite3_column_double(database_deuxieme->stmt, 1);
            id_unique_tmp = sqlite3_column_int(database_deuxieme->stmt, 2);
            double value = distance(longitude, latitude, longitude_tmp, latitude_tmp);
            define(mat,id_unique_tmp,id_unique,value);
        }
        end_request_database(database_deuxieme);
    }

    end_request_database(database);
    mat_destroy(mat);
    close_database(database);
    close_database(database_deuxieme);
    return 0;
}
