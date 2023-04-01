#ifndef DATABASE_READER_H
#define DATABASE_READER_H

#include <stdio.h>
#include <sqlite3.h>
#include <stdbool.h>

database_t * open_database(const char *filename);
int close_database(database_t * database);
typedef struct database_t {
   sqlite3 *db;
   sqlite3_stmt *stmt;
   bool opened_corectly;
} database_t;

#endif /* DATABASE_READER_H */