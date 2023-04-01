#include "database_reader.h"

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
   } else {
      fprintf(stderr, "Opened database successfully\n");
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
      fprintf(stderr, "Closed database successfully\n");
   }

   free(database);

   return 1;
}

int prepare_request_database(database_t * database, char * request) {
   int rc = sqlite3_prepare_v2(database->db, request, -1, &database->stmt, NULL);

   if (rc) {
      fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(database->db));
      return 0;

   } else {
      fprintf(stderr, "Prepared statement successfully\n");
   }

   return 1;
}

int request_database(database_t * database) {
   int rc = sqlite3_step(database->stmt);

   printf("--------------------\n");
   printf("Begin of all returned values\n");
   printf("%s\n", sqlite3_column_text(database->stmt, 0));

   while ((rc = sqlite3_step(database->stmt)) == SQLITE_ROW) {
      printf("%s\n", sqlite3_column_text(database->stmt, 0));
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

int end_request_database(database_t * database) {
   sqlite3_finalize(database->stmt);
   return 1;
}

// int main() {
//    database_t * database = open_database("./../data/database.db");
//    if (!database->opened_correctly) {
//       exit(0);
//    }
//    printf("Version of sqlite3 : %s\n", sqlite3_libversion());
//    char* sql_command = "SELECT * FROM bornes where id_unique like 13;\n";
//    prepare_request_database(database, sql_command);
//    request_database(database);
//    end_request_database(database);
//    close_database(database);
//    return 0;
// }
