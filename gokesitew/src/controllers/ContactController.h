#include "../include/headeronly_src/sqlite3pp.h"
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
};
