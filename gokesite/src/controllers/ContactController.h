#pragma once
#include "../../include/crow_all.h"

using namespace crow;

#include "../../include/headeronly_src/sqlite3pp.h"
#include "../entities/Contact.h"

class ContactController
{
private:
    /* data */
public:
    ContactController(){}
    ~ContactController(){}

    std::vector<Contact> get(sqlite3pp::database &db)
    {
        std::vector<Contact> contacts;
        contacts.reserve(10);
        sqlite3pp::query qry(db, "SELECT name, id, phone FROM contacts");

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

    crow::json::wvalue process(const request &req, sqlite3pp::database &db)
    {
        auto method = method_name(req.method);
        crow::json::wvalue dto;

        vector<crow::json::rvalue> contacts;
        contacts.reserve(10);

        if (req.method == HTTPMethod::POST)
        {
            auto formdata = req.body;

            cout << formdata << endl;

            // json in class Contact
            Contact c{};
            c.fromJson(formdata);

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

                Contact contact{id, name, phone};

                contacts.push_back(json::load(contact.toJson()));
            }
        }
        else if (req.method == HTTPMethod::PUT)
        {
        }
        else if (req.method == HTTPMethod::DELETE)
        {
        }
        else if (req.method == HTTPMethod::GET)
        {
            ContactController ctr{};
            auto qry = ctr.get(db);
            for (auto &&c : qry)
            {
                contacts.push_back(json::load(c.toJson()));
            }
        }

        
        dto["contacts"] = contacts;

        return dto;
    }
};
