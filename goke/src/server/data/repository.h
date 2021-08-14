#pragma once

#include <vector>
#include "../include/global.h"
#include "../include/db.h"
#include "../entities/contact.h"
#include "../../../../../goke/src/gokeCPP/hello_wnd_lnx/goke_core/query.h"
 
using namespace entities;

namespace gq = goke::query;

namespace repository
{
    class ContactRepository
    {
    private:
        /* data */
    public:
        ContactRepository(/* args */) {}
        ~ContactRepository() {}

        std::vector<Contact> &getDB()
        {

            char *dbname = "app_data/goke.db";
            char *sql = "SELECT * from CONTACTS";

            auto sqlData = std::make_unique<db::SqlData>();

            db::selectSql(sqlData.get(), dbname, sql);

            gDB.clear();

            int id = 0;
            std::string name;
            std::string phone;
            for (auto &&v : sqlData->data)
            {
                id = std::atoi(v[0].c_str());
                name = v[1];
                phone = v[2];
                auto c = Contact(id, name, phone);
                gDB.push_back(c);
            }

            return gDB;
        }

        Contact getById(int id)
        {
            Contact c = gq::first(gDB, [=](auto a)
                                  { return a.getId() == id; });

            return c;
        }

        int getUniqueID()
        {
            char *dbname = "app_data/goke.db";
            const char *sql = "SELECT MAX(ID), COUNT(*) FROM CONTACTS;";
            auto sqlData = std::make_unique<db::SqlData>();
            db::selectSql(sqlData.get(), dbname, sql);
            auto v = sqlData->data[0];
            auto maxId = std::atoi(v[0].c_str());
            auto count = std::atoi(v[1].c_str());
            return maxId + 1;
        }

        Contact add(Contact &c)
        {
            char *dbname = "app_data/goke.db";

            c.setId(getUniqueID());

            std::stringstream ss;
            ss << "INSERT INTO CONTACTS (ID, NAME, PHONE) "
               << "VALUES(" << c.getId() << ", '" << c.getName() << "', '" << c.getPhone() << "'); ";
            auto ssql = ss.str();
            auto sql = ssql.c_str();

            db::insertSql(dbname, sql);

            return c;
        }

        void update(int id, Contact &c)
        {
            char *dbname = "app_data/goke.db";

            std::stringstream ss;
            ss << "UPDATE CONTACTS set NAME = '" << c.getName() << "', PHONE ='" << c.getPhone() << "' where ID=" << id << "; ";
            auto ssql = ss.str();
            auto sql = ssql.c_str();

            db::updateSql(dbname, sql);
        }

        void remove(int id)
        {
            char *dbname = "app_data/goke.db";

            std::stringstream ss;
            ss << "DELETE FROM CONTACTS where ID=" << id << ";";
            auto ssql = ss.str();
            auto sql = ssql.c_str();

            db::deleteSql(dbname, sql);
        }
    };

} // namespace repository
