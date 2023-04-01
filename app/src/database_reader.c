#include <database_reader.h>

database_t * open_database(const char *filename) { // ouvre la connexion vers la base de données
   sqlite3 *db;
   int rc;

   rc = sqlite3_open16(filename, &db);

   database_t * database;
   database->db = db;
   database->stmt = NULL;

   if (rc) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      database->opened_corectly = false;
   } else {
      fprintf(stderr, "Opened database successfully\n");
      database->opened_corectly = true;
   }

   return database;
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

int request_database(database_t * database, char * request) { // exécute la requête donnée en argument
   int rc = sqlite3_prepare16_v2(database->db, request, -1, &database->stmt, NULL);

   if (rc) {
      fprintf(stderr, "Failed to prepare statement: %s", sqlite3_errmsg(database->db));

   } else {
      fprintf(stderr, "Prepared statement successfully");
   }

   return 1;
}
