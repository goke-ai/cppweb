#include "libs/crow_all.h"
#include "libs/headeronly_src/sqlite3pp.h"
#include "libs/json.hpp"

#include "data/sqlite_adapter.h"
#include "entities/Contact.h"
#include "controllers/ContactController.h"

#include <sqlite3.h>
#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <unordered_set>
#include <mutex>

using namespace std;
using namespace crow;
using namespace crow::mustache;
namespace pt = boost::property_tree;

void getView(response &res, const string &filename, context &x)
{
    res.set_header("Content-Type", "text/html");
    auto text = load("public/" + filename + ".html").render(x);
    res.write(text);
    res.end();
}

void sendFile(response &res, string filename, string contentType)
{
    ifstream in("public/" + filename, ifstream::in);
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
    sendFile(res, "images/" + filename, "image/gif");
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
    std::mutex mtx;
    std::unordered_set<crow::websocket::connection *> users;

    crow::SimpleApp app;
    set_base(".");

    // createCompany();
    // insertCompany();
    // queryCompany();
    // sqlite3ppX();

    sqlite3pp::database db("app_data/test.db");

    CROW_ROUTE(app, "/ws")
        .websocket()
        .onopen([&](crow::websocket::connection &conn) {
            std::lock_guard<std::mutex> _(mtx);
            users.insert(&conn);
        })
        .onclose([&](crow::websocket::connection &conn, const string &reason) {
            std::lock_guard<std::mutex> _(mtx);
            users.erase(&conn);
        })
        .onmessage([&](crow::websocket::connection & /*conn*/, const string &data, bool is_binary) {
            std::lock_guard<std::mutex> _(mtx);
            for (auto &&user : users)
            {
                if (is_binary)
                {
                    user->send_binary(data);
                }
                else
                {
                    user->send_text(data);
                }
            }
        });

    CROW_ROUTE(app, "/chat")
    ([](const request &req, response &res) {
        sendHtml(res, "chat");
    });

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
    CROW_ROUTE(app, "/api/contacts").methods(HTTPMethod::Get, HTTPMethod::Post, HTTPMethod::Put)([&db](const request &req) {
        auto method = method_name(req.method);
        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);

        if (req.method == HTTPMethod::Post)
        {
            auto formdata = req.body;

            cout << formdata << endl;

            // json in class Contact
            Contact c{};
            c.fromJson(formdata);
            cout << c.getId() << " " << c.getName() << " " << c.getPhone() << endl;

            auto id = c.getId();
            auto name = c.getName();
            auto phone = c.getPhone();

            // json from nlohmann
            // auto j2 = nlohmann::json::parse(req.body);

            // auto id = j2["id"].get<int>();
            // auto name = j2["name"].get<std::string>();
            // auto phone = j2["phone"].get<std::string>();

            // display Contact data
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
        else if (req.method == HTTPMethod::Put)
        {
        }
        else if (req.method == HTTPMethod::Delete)
        {
        }
        else if (req.method == HTTPMethod::Get)
        {
            ContactController ctr{};
            auto qry = ctr.get(db);
            for (auto &&q : qry)
            {
                contacts.push_back(json::load(q.toJson()));
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
    cout << "http::/localhost:" << iPort << "\n";

    app.port(iPort).multithreaded().run();

    return 0;
}
