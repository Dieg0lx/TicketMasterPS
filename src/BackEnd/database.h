#ifndef DATABASE_H
#define DATABASE_H

#include "sqlite3.h" 

extern sqlite3 *db;


int db_init(const char* filename);

void db_close();

#endif