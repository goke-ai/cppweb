#pragma once

#include "../includes/crow_all.h"

namespace entities
{

    class Contact
    {

        int _id;
        std::string _name;
        std::string _phone;

    public:

        Contact(){}
        
        Contact(int id, std::string name, std::string phone)
        {
            _id = id;
            _name = name;
            _phone = phone;
        }

        auto toJson()
        {
            crow::json::wvalue x;
            x["id"] = _id;
            x["name"] = _name;
            x["phone"] = _phone;

            return x;
        }

        static Contact fromJson(crow::json::rvalue j)
        {
            int id = j["id"].i();
            std::string name = j["name"].s();
            std::string phone = j["phone"].s();

            return Contact{id, name, phone};
        }

        static Contact fromJson(std::string jsonString)
        {
            auto j = crow::json::load(jsonString);

            return Contact::fromJson(j);
        }

        int getId()
        {
            return _id;
        }
        void setId(int id)
        {
            _id = id;
        }

        std::string getName()
        {
            return _name;
        }
        void setName(std::string name)
        {
            _name = name;
        }

        std::string getPhone()
        {
            return _phone;
        }
        void setPhone(std::string phone)
        {
            _phone = phone;
        }
    };
}