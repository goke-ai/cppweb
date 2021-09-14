#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
// #include <sqlite3.h>
#include "../sqlite3.h"

namespace db
{
   struct SqlData
   {
      const char *msg = "Select Callback function called";
      std::vector<std::string> columns;
      std::vector<std::vector<std::string>> data;
   };

   int connect(const char *dbName = "test.db")
   {
      sqlite3 *db;
      char *zErrMsg = 0;
      int rc;

      rc = sqlite3_open(dbName, &db);

      if (rc)
      {
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         // std::cout << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
         return (0);
      }
      else
      {
         fprintf(stderr, "Opened database successfully\n");
         // std::cout << "Opened database successfully\n";
      }
      auto v = sqlite3_close(db);

      return v;
   }

   static int createSql_callback(void *NotUsed, int argc, char **argv, char **azColName)
   {
      int i;
      for (i = 0; i < argc; i++)
      {
         printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
      }
      printf("\n");
      return 0;
   }

   int createSql(const char *dbName = "test.db",
                 /* Create SQL statement */
                 const char *sql = "CREATE TABLE COMPANY("
                                   "ID INT PRIMARY KEY     NOT NULL,"
                                   "NAME           TEXT    NOT NULL,"
                                   "AGE            INT     NOT NULL,"
                                   "ADDRESS        CHAR(50),"
                                   "SALARY         REAL );")
   {
      sqlite3 *db;
      char *zErrMsg = 0;
      int rc;
      // char *sql;

      /* Open database */
      rc = sqlite3_open(dbName, &db);

      if (rc)
      {
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         return (0);
      }
      else
      {
         fprintf(stdout, "Opened database successfully\n");
      }

      /* Execute SQL statement */
      rc = sqlite3_exec(db, sql, createSql_callback, 0, &zErrMsg);

      if (rc != SQLITE_OK)
      {
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      }
      else
      {
         fprintf(stdout, "Table created successfully\n");
      }
      sqlite3_close(db);
      return 0;
   }

   static int insertSql_callback(void *NotUsed, int argc, char **argv, char **azColName)
   {
      int i;
      for (i = 0; i < argc; i++)
      {
         printf("%s = %s, ", azColName[i], argv[i] ? argv[i] : "NULL");
      }
      printf("\n");
      return 0;
   }

   int insertSql(const char *dbName = "test.db",
                 /* Create SQL statement */
                 const char *sql = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                                   "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
                                   "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                                   "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
                                   "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                                   "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 ); "
                                   "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                                   "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 ); ")
   {
      sqlite3 *db;
      char *zErrMsg = 0;
      int rc;
      // char *sql;

      /* Open database */
      rc = sqlite3_open(dbName, &db);

      if (rc)
      {
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         return (0);
      }
      else
      {
         fprintf(stderr, "Opened database successfully\n");
      }

      /* Execute SQL statement */
      rc = sqlite3_exec(db, sql, insertSql_callback, 0, &zErrMsg);

      if (rc != SQLITE_OK)
      {
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      }
      else
      {
         fprintf(stdout, "Records created successfully\n");
      }
      sqlite3_close(db);
      return 0;
   }

   static int selectSql_callback(void *data, int argc, char **argv, char **azColName)
   {
      int i;
      // fprintf(stderr, "%s: \n", (const char *)data);
      auto sqlData = (SqlData *)data;
      fprintf(stderr, "%s: \n", sqlData->msg);

      std::vector<std::string> v;

      for (i = 0; i < argc; i++)
      {
         printf("%s = %s, ", azColName[i], argv[i] ? argv[i] : "NULL");

         std::ostringstream ss;
         ss << (argv[i] ? argv[i] : "[NULL]");

         if (std::size(sqlData->columns) <= i)
         {
            sqlData->columns.push_back(azColName[i]);
         }
         else
         {
            sqlData->columns[i] = azColName[i];
         }

         v.push_back(ss.str());
      }
      sqlData->data.push_back(v);
      printf("\n");
      return 0;
   }

   int selectSql(SqlData *sqlData, const char *dbName = "test.db",
                 /* Create SQL statement */
                 const char *sql = "SELECT * from COMPANY")
   {
      sqlite3 *db;
      char *zErrMsg = 0;
      int rc;
      // char *sql;
      // const char *data = "Select Callback function called";
      // SqlData *sqlData = new SqlData();

      /* Open database */
      rc = sqlite3_open(dbName, &db);

      if (rc)
      {
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         return (0);
      }
      else
      {
         fprintf(stderr, "Opened database successfully\n");
      }

      /* Execute SQL statement */
      // rc = sqlite3_exec(db, sql, selectSql_callback, (void *)data, &zErrMsg);
      rc = sqlite3_exec(db, sql, selectSql_callback, (void *)sqlData, &zErrMsg);

      if (rc != SQLITE_OK)
      {
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      }
      else
      {
         fprintf(stdout, "Operation done successfully\n");
      }

      for (auto &&v : sqlData->columns)
      {
         std::cout << v << '\t';
      }
      std::cout << "\n";

      for (auto &&v : sqlData->data)
      {
         for (auto &&w : v)
         {
            std::cout << w << '\t';
         }
         std::cout << "\n";
      }

      sqlite3_close(db);

      return 0;
   }

   static int updateSql_callback(void *data, int argc, char **argv, char **azColName)
   {
      int i;
      fprintf(stderr, "%s: \n", (const char *)data);

      for (i = 0; i < argc; i++)
      {
         printf("%s = %s, ", azColName[i], argv[i] ? argv[i] : "NULL");
      }
      printf("\n");
      return 0;
   }

   int updateSql(const char *dbName = "test.db",
                 /* Create merged SQL statement */
                 const char *sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
                                   "SELECT * from COMPANY"

   )
   {
      sqlite3 *db;
      char *zErrMsg = 0;
      int rc;
      // char *sql;
      const char *data = "Update Callback function called";

      /* Open database */
      rc = sqlite3_open(dbName, &db);

      if (rc)
      {
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         return (0);
      }
      else
      {
         fprintf(stderr, "Opened database successfully\n");
      }

      /* Execute SQL statement */
      rc = sqlite3_exec(db, sql, updateSql_callback, (void *)data, &zErrMsg);

      if (rc != SQLITE_OK)
      {
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      }
      else
      {
         fprintf(stdout, "Operation done successfully\n");
      }
      sqlite3_close(db);
      return 0;
   }

   static int deleteSql_callback(void *data, int argc, char **argv, char **azColName)
   {
      int i;
      fprintf(stderr, "%s: \n", (const char *)data);

      for (i = 0; i < argc; i++)
      {
         printf("%s = %s, ", azColName[i], argv[i] ? argv[i] : "NULL");
      }
      printf("\n");
      return 0;
   }

   int deleteSql(const char *dbName = "test.db",
                 /* Create merged SQL statement */
                 const char *sql = "DELETE from COMPANY where ID=2; "
                                   "SELECT * from COMPANY"

   )
   {
      sqlite3 *db;
      char *zErrMsg = 0;
      int rc;
      // char *sql;
      const char *data = "Delete Callback function called";

      /* Open database */
      rc = sqlite3_open(dbName, &db);

      if (rc)
      {
         fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
         return (0);
      }
      else
      {
         fprintf(stderr, "Opened database successfully\n");
      }

      /* Execute SQL statement */
      rc = sqlite3_exec(db, sql, deleteSql_callback, (void *)data, &zErrMsg);

      if (rc != SQLITE_OK)
      {
         fprintf(stderr, "SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      }
      else
      {
         fprintf(stdout, "Operation done successfully\n");
      }
      sqlite3_close(db);
      return 0;
   }

}