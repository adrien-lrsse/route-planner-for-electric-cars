#ifndef DATABASE_READER_H
#define DATABASE_READER_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>
#include <stdlib.h>
#include "coordinates.h"

typedef struct database_t {
   sqlite3 *db;
   sqlite3_stmt *stmt;
   bool opened_correctly;
} database_t;


database_t * open_database(const char *filename);
int close_database(database_t * database);
int prepare_request_database(database_t * database, char * request);
int request_database(database_t * database);
int end_request_database(database_t * database);
coord_pt getCoordonnees(int i);


#endif /* DATABASE_READER_H */
