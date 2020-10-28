#if !defined(__SQLITE_ADAPTER__)
#define __SQLITE_ADAPTER__

#include <sqlite3.h>
#include <iostream>
#include "../include/headeronly_src/sqlite3pp.h"

using namespace std;

int sqlite3ppX()
{
    try
    {
        sqlite3pp::database db("app_data/test.db");
        /* Create SQL statement */
        auto sql = "CREATE TABLE contacts("
                   "ID INT PRIMARY KEY     NOT NULL,"
                   "NAME           TEXT    NOT NULL,"
                   "PHONE            TEXT     NOT NULL );";

        db.execute(sql);

        sqlite3pp::transaction xct(db);
        {
            sqlite3pp::command cmd(
                db,
                "INSERT INTO contacts (id, name, phone) VALUES (1, 'Godwin', '1234');"
                "INSERT INTO contacts (id, name, phone) VALUES (2, 'Anosike', '5678');"
                "INSERT INTO contacts (id, name, phone) VALUES (3, 'Bagbanaram', '9012');");
            {
                // cout << cmd.bind(":name", "user", sqlite3pp::copy) << endl;
                cout << cmd.execute_all() << endl;
            }
        }
        xct.commit();

        cout << "===SELECT================" << endl;
        sqlite3pp::transaction xct1(db, true);
        {
            sqlite3pp::query qry(db, "SELECT id, name, phone FROM contacts");

            for (int i = 0; i < qry.column_count(); ++i)
            {
                cout << qry.column_name(i) << "\t";
            }
            cout << endl;

            for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
            {
                for (int j = 0; j < qry.column_count(); ++j)
                {
                    cout << (*i).get<char const *>(j) << "\t";
                }
                cout << endl;
            }
            cout << endl;

            qry.reset();

            for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
            {
                int id;
                char const *name, *phone;
                std::tie(id, name, phone) = (*i).get_columns<int, char const *, char const *>(0, 1, 2);
                cout << id << "\t" << name << "\t" << phone << endl;
            }
            cout << endl;

            qry.reset();

            for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
            {
                int id = 0;
                std::string name, phone;
                // (*i).getter() >> sqlite3pp::ignore >> name >> phone;
                (*i).getter() >> id >> name >> phone;
                cout << id << "\t" << name << "\t" << phone << endl;
            }
        }
        xct1.commit();
    }
    catch (std::exception &ex)
    {
        cout << ex.what() << endl;
    }

    return 0;
}

static int callback(void *data, int argc, char **argv, char **azColName)
{
    int i;
    fprintf(stderr, "%s: \n", (const char *)data);

    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int createCompany()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    // char *sql;

    /* Open database */
    rc = sqlite3_open("../app_data/test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    char sql[] = "CREATE TABLE COMPANY("
                 "ID INT PRIMARY KEY     NOT NULL,"
                 "NAME           TEXT    NOT NULL,"
                 "AGE            INT     NOT NULL,"
                 "ADDRESS        CHAR(50),"
                 "SALARY         REAL );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

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

int insertCompany()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    /* Open database */
    rc = sqlite3_open("../app_data/test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    char sql[] = "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                 "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
                 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
                 "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
                 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                 "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
                 "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
                 "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

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

int queryCompany()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("../app_data/test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    char sql[] = "SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

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

int updateCompany()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("../app_data/test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    char sql[] = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
                 "SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

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

int deleteCompany()
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("../app_data/test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return (0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    char sql[] = "DELETE from COMPANY where ID=2; "
                 "SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);

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


#endif // __SQLITE_ADAPTER__
