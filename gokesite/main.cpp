#include "crow_all.h"
#include "headeronly_src/sqlite3pp.h"
#include "json.hpp"

#include "contact.h"

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using namespace std;
using namespace crow;
using namespace crow::mustache;
namespace pt = boost::property_tree;

int sqlite3ppX()
{
    try
    {
        sqlite3pp::database db("../app_data/test.db");
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

void getView(response &res, const string &filename, context &x)
{
    res.set_header("Content-Type", "text/html");
    auto text = load("../public/" + filename + ".html").render(x);
    res.write(text);
    res.end();
}

void sendFile(response &res, string filename, string contentType)
{
    ifstream in("../public/" + filename, ifstream::in);
    if (in)
    {
        ostringstream contents;
        contents << in.rdbuf();
        in.close();
        res.set_header("Content-Type", contentType);
        res.write(contents.str());
    }
    else
    {
        res.code = 404;
        res.write("Not found");
    }
    res.end();
}

void sendHtml(response &res, string filename)
{
    sendFile(res, filename + ".html", "text/html");
}

void sendImage(response &res, string filename)
{
    sendFile(res, "images/" + filename, "image/jpeg");
}

void sendScript(response &res, string filename)
{
    sendFile(res, "scripts/" + filename, "text/javascript");
}

void sendStyle(response &res, string filename)
{
    sendFile(res, "styles/" + filename, "text/css");
}

void notFound(response &res, const string &message)
{
    res.code = 404;
    res.write(message + ": Not Found.");
    res.end();
}

int main(int argc, char const *argv[])
{
    crow::SimpleApp app;
    set_base(".");

    // createCompany();
    // insertCompany();
    // queryCompany();
    // sqlite3ppX();

    sqlite3pp::database db("../app_data/test.db");

    CROW_ROUTE(app, "/styles/<string>")
    ([](const request &req, response &res, string filename) {
        sendStyle(res, filename);
    });

    CROW_ROUTE(app, "/scripts/<string>")
    ([](const request &req, response &res, string filename) {
        sendScript(res, filename);
    });

    CROW_ROUTE(app, "/images/<string>")
    ([](const request &req, response &res, string filename) {
        sendImage(res, filename);
    });

    CROW_ROUTE(app, "/contact/<string>")
    ([&db](const request &req, response &res, string name) {
        auto sql = "SELECT id, name, phone FROM contacts WHERE name LIKE'" + name + "'";
        sqlite3pp::query qry(db, sql.c_str());

        crow::json::wvalue dto;
        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);
        // vector<Contact> contacts;
        // contacts.reserve(10);

        for (auto &&q : qry)
        {
            int id = 0;
            std::string name, phone;
            q.getter() >> id >> name >> phone;

            Contact contact{id, name, phone};

            contacts.push_back(json::load(contact.toJson()));
        }

        if (contacts.empty())
        {
            return notFound(res, "Contact");
        }

        dto["contact"] = contacts;
        getView(res, "contact", dto);
    });

    CROW_ROUTE(app, "/contacts")
    ([&db](const request &req, response &res) {
        sqlite3pp::query qry(db, "SELECT id, name, phone FROM contacts");

        crow::json::wvalue dto;
        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);

        for (auto &&q : qry)
        {
            int id = 0;
            std::string name, phone;
            q.getter() >> id >> name >> phone;

            Contact contact{id, name, phone};

            contacts.push_back(json::load(contact.toJson()));
        }

        dto["contacts"] = contacts;
        getView(res, "contacts", dto);
    });

    CROW_ROUTE(app, "/about")
    ([](const request &req, response &res) {
        sendHtml(res, "about");
    });

    CROW_ROUTE(app, "/query")
    ([](const request &req, response &res) {
        auto name = req.url_params.get("name");
        auto phone = req.url_params.get("phone");
        ostringstream os;
        os << "Hello " << (name ? name : "") << " your phone#" << (phone ? phone : "") << endl;
        res.write(os.str());
        res.end();
    });

    // json endpoint
    CROW_ROUTE(app, "/api/contacts")
    ([&db](const request &req) {
        sqlite3pp::query qry(db, "SELECT id, name, phone FROM contacts");

        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);

        for (auto &&q : qry)
        {
            int id = 0;
            std::string name, phone;
            q.getter() >> id >> name >> phone;

            Contact contact{id, name, phone};

            contacts.push_back(json::load(contact.toJson()));
        }

        crow::json::wvalue dto;
        dto["contacts"] = contacts;
        return crow::response{dto};
    });

    CROW_ROUTE(app, "/api/contactx").methods(HTTPMethod::Get, HTTPMethod::Post, HTTPMethod::Put)([&db](const request &req) {
        auto method = method_name(req.method);
        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);

        if (method == "POST")
        {
            auto formdata = req.body;

            cout << formdata << endl;

            Contact c{};
            c.fromJson(formdata);
            cout << c.getId() << " " << c.getName() << " " << c.getPhone() << endl;

            auto j2 = nlohmann::json::parse(req.body);

            auto id = j2["id"].get<int>();
            auto name = j2["name"].get<std::string>();
            auto phone = j2["phone"].get<std::string>();

            cout << id << " " << name << " " << phone << endl;
            sqlite3pp::transaction xct(db);
            {
                sqlite3pp::command cmd(
                    db,
                    "INSERT INTO contacts (id, name, phone) VALUES (:id, :name, :phone);");
                {
                    cout << cmd.bind(":id", to_string(id), sqlite3pp::nocopy) << endl;
                    cout << cmd.bind(":name", name, sqlite3pp::nocopy) << endl;
                    cout << cmd.bind(":phone", phone, sqlite3pp::nocopy) << endl;
                    cout << cmd.execute_all() << endl;
                }
            }
            xct.commit();

            auto sql = "SELECT id, name, phone FROM contacts WHERE Id =" + to_string(id);
            sqlite3pp::query qry(db, sql.c_str());

            for (auto &&q : qry)
            {
                int id = 0;
                std::string name, phone;
                q.getter() >> id >> name >> phone;

                Contact contact{id, name, phone};

                contacts.push_back(json::load(contact.toJson()));
            }
        }
        else if (method == "PUT")
        {
        }
        else
        {
            sqlite3pp::query qry(db, "SELECT id, name, phone FROM contacts");

            for (auto &&q : qry)
            {
                int id = 0;
                std::string name, phone;
                q.getter() >> id >> name >> phone;

                Contact contact{id, name, phone};

                contacts.push_back(json::load(contact.toJson()));
            }
        }

        crow::json::wvalue dto;
        dto["contacts"] = contacts;
        return crow::response{dto};
    });

    // rest client
    CROW_ROUTE(app, "/rest_test").methods(HTTPMethod::Get, HTTPMethod::Post, HTTPMethod::Put)([](const request &req, response &res) {
        auto method = method_name(req.method);
        res.set_header("Content-Type", "text/plain");
        res.write(method + "rest_test");
        res.end();
    });

    // default
    CROW_ROUTE(app, "/")
    ([](const request &req, response &res) {
        sendHtml(res, "index");
    });

    char *port = getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL ? stoi(port) : 18080);
    cout << "PORT = " << iPort << "\n";

    app.port(iPort).multithreaded().run();

    return 0;
}
