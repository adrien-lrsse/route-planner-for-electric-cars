#include "database_reader.h"
#include "coordinates.h"


database_t * open_database(const char *filename) { // ouvre la connexion vers la base de données
   sqlite3 *db;
   int rc;

   rc = sqlite3_open(filename, &db);

   struct database_t * my_database = malloc(sizeof(struct database_t));
   my_database->db = db;
   my_database->stmt = NULL;

   if (rc) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      my_database->opened_correctly = false;
   } 
   else {
      //fprintf(stderr, "Opened database successfully\n");
      my_database->opened_correctly = true;
   }

   return my_database;
}

int close_database(database_t * database) { // ferme la connexion vers la base de données et supprime la structure database_t
   int rc = sqlite3_close(database->db);
   if (rc) {
      fprintf(stderr, "Can't close database: %s\n", sqlite3_errmsg(database->db));
      return 0;
   } else {
      //fprintf(stderr, "Closed database successfully\n");
   }

   free(database);

   return 1;
}

int prepare_request_database(database_t * database, char * request) { // prépare la requête à éxécuter
   int rc = sqlite3_prepare_v2(database->db, request, -1, &database->stmt, NULL);

   if (rc) {
      return 0;
   }
   return 1;
}

int request_database(database_t * database) { // execute la requete préparée et affiche les résultats
   int rc;
   printf("--------------------\n");
   printf("Begin of all returned values\n");

   while ((rc = sqlite3_step(database->stmt)) != SQLITE_DONE) {
         printf("%f\n", sqlite3_column_double(database->stmt, 0));
         printf("%f\n", sqlite3_column_double(database->stmt, 1));
         printf("%d\n", sqlite3_column_int(database->stmt, 2));
   }

   printf("End of all returned values\n");
   printf("--------------------\n");

   if (rc != SQLITE_DONE) {
      fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(database->db));
      return 0;
   } else {
      fprintf(stderr, "Executed statement successfully\n");
   }

   return 1;
}

int end_request_database(database_t * database) { // libère la mémoire utilisée par la requête
   sqlite3_finalize(database->stmt);
   return 1;
}


// coord_pt getCoordonnees(int i){
//    coord_pt res;
//    database_t * database = open_database("./../data/database.db");
//    if (!database->opened_correctly) {
//        exit(0);
//    }
//    char* sql_commmand = "SELECT consolidated_longitude, consolidated_latitude, id_unique FROM bornes WHERE id_unique = ?";
//    prepare_request_database(database,sql_commmand);
//    sqlite3_bind_int(database->stmt, 1, i); // on bind la valeur de "?" dans la requête
//    sqlite3_step(database->stmt);
//    res.longitude = sqlite3_column_double(database->stmt, 0);
//    res.latitude = sqlite3_column_double(database->stmt,1);
//    res.indice = sqlite3_column_int(database->stmt,2);
//    end_request_database(database);
//    close_database(database);
//    return res;
// }


// int main() {
//    coord_pt new = getCoordonnees(52);
//    printf("%f,%f,%d\,",new.longitude,new.latitude,new.indice);
//  }
