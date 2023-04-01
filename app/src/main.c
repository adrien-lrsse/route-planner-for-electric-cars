#include "coordinates.h"
#include "struct_mat_adj.h"
#include "database_reader.h"

int main (){
    mat_adj* mat = create_mat_adj(10);
    
    database_t * database = open_database("./../data/database.db");
    if (!database->opened_correctly) {
        exit(0);
    }

    char * request = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes";
    prepare_request_database(database, request);
    //request_database(database);
    end_request_database(database);

    mat_destroy(mat);
    close_database(database);
    return 0;
}
