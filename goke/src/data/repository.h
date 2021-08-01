#include <vector>
#include "../global.h"
#include "../entities/contact.h"
#include "../../../../goke/src/gokeCPP/hello_wnd_lnx/goke_core/query.h"

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

            char *dbname = "goke.db";
            char *sql = "SELECT * from CONTACTS";

            auto sqlData = std::make_unique<db::SqlData>();

            db::selectSql(sqlData.get(), dbname, sql);            

            gDB.clear();                

            int id=0;
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

        Contact add(Contact &c)
        {
            auto m = gq::max(gDB, [](auto a, auto b)
                             { return b.getId() > a.getId(); });
            c.setId(m.getId() + 1);
            gDB.push_back(c);

            return c;
        }

        void update(int id, Contact &c)
        {
            gq::replace(gDB, c, [&](auto a)
                        { return a.getId() == id; });
        }

        void remove(int id)
        {
            gq::remove(gDB, [&](auto a)
                       { return a.getId() == id; });
        }
    };

} // namespace repository
