#pragma once

#include "../../include/headeronly_src/sqlite3pp.h"
#include "../entities/Contact.h"

#include "../../include/crow_all.h"

using namespace crow;

class ContactController
{
private:
    /* data */
public:
    ContactController() {}
    ~ContactController() {}

    crow::json::wvalue process(const request &req, sqlite3pp::database &db)
    {
        auto method = method_name(req.method);
        crow::json::wvalue dto;

        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);

        if (req.method == HTTPMethod::POST)
        {
            postApi(req, db, contacts);
        }
        // else if (req.method == HTTPMethod::PUT)
        // {
        //      putApi(req, db, contacts);
        // }
        // else if (req.method == HTTPMethod::DELETE)
        // {
        // }
        else if (req.method == HTTPMethod::GET)
        {
            getApi(req, db, contacts);
        }

        dto["contacts"] = contacts;

        return dto;
    }

    crow::json::wvalue process(int id, const request &req, sqlite3pp::database &db)
    {
        auto method = method_name(req.method);
        crow::json::wvalue dto;

        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);

        // if (req.method == HTTPMethod::POST)
        // {
        //     postApi(req, db, contacts);
        // }
        if (req.method == HTTPMethod::PUT)
        {
             putApi(id, req, db, contacts);
        }
        else if (req.method == HTTPMethod::DELETE)
        {
            deleteApi(id, req, db);
        }
        else if (req.method == HTTPMethod::GET)
        {
            getApi(req, db, contacts);
        }

        dto["contacts"] = contacts;

        return dto;
    }

    void getApi(const request &req, sqlite3pp::database &db, vector<crow::json::rvalue> &contacts)
    {
        ContactController ctr{};
        auto qry = ctr.get(db);
        for (auto &&c : qry)
        {
            contacts.push_back(json::load(c.toJson()));
        }
    }

    void postApi(const request &req, sqlite3pp::database &db, vector<crow::json::rvalue> &contacts)
    {
        auto formdata = req.body;

        cout << formdata << endl;

        // json in class Contact
        // auto c = std::make_unique<Contact>();
        // c->fromJson(formdata);

        auto&& c = Contact::fromJson(formdata);

        auto id = c.getId();
        auto name = c.getName();
        auto phone = c.getPhone();

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

            auto p = std::make_unique<Contact>(id, name, phone);
            contacts.push_back(json::load(p->toJson()));
        }
    }

    void putApi(int id, const request &req, sqlite3pp::database &db, vector<crow::json::rvalue> &contacts)
    {
        auto formdata = req.body;

        cout << formdata << endl;

        // json in class Contact

        auto&& c = Contact::fromJson(formdata);

        auto id_ = c.getId();
        auto name = c.getName();
        auto phone = c.getPhone();

        // display Contact data
        cout << id_ << " : " << id << " " << name << " " << phone << endl;

        if (id != id_)
        {
            return;
        }
        
        sqlite3pp::transaction xct(db);
        {
            sqlite3pp::command cmd(
                db,
                "UPDATE contacts SET name=:name, phone=:phone WHERE id=:id;");
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

            auto p = std::make_unique<Contact>(id, name, phone);
            contacts.push_back(json::load(p->toJson()));
        }
    }

    void deleteApi(int id, const request &req, sqlite3pp::database &db)
    {
        auto sql = "DELETE FROM contacts WHERE Id =" + to_string(id);
        db.execute(sql.c_str());        
    }

    std::vector<Contact> get(sqlite3pp::database &db)
    {
        std::vector<Contact> contacts;
        contacts.reserve(10);
        sqlite3pp::query qry(db, "SELECT id, name, phone FROM contacts");

        for (auto &&q : qry)
        {
            int id = 0;
            std::string name, phone;
            q.getter() >> id >> name >> phone;

            Contact contact{id, name, phone};

            contacts.push_back(contact);
        }

        return contacts;
    }
};
