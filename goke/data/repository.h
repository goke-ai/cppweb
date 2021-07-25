#include <vector>
#include "../entities/contact.h"
#include "../../../goke/src/gokeCPP/hello_wnd_lnx/query/query.h"

using namespace entities;

namespace gq = goke::query;

namespace repository
{
    class ContactRepository
    {
    private:
        /* data */
        std::vector<Contact> DB = {Contact(1, "Goke", "+234803"),
                                           Contact(2, "Lillian", "+2348103"),
                                           Contact(3, "Lolade", "+2348104"),
                                           Contact(4, "Gbola", "+2348024")};

    public:
        ContactRepository(/* args */) {}
        ~ContactRepository() {}

        std::vector<Contact>& getDB()
        {
            return DB;
        }

        Contact getById(int id)
        {
            Contact c = gq::first(DB, [=](auto a)
                                  { return a.getId() == id; });

            return c;
        }


        Contact add(Contact &c)
        {
            auto m = gq::max(DB, [](auto a, auto b)
                             { return b.getId() > a.getId(); });
            c.setId(m.getId() + 1);
            DB.push_back(c);

            return c;
        }

        void update(int id, Contact &c)
        {
            gq::replace(DB, c, [&](auto a)
                        { return a.getId() == id; });
        }
        void remove(int id)
        {
            gq::remove(DB, [&](auto a)
                       { return a.getId() == id; });
        }
    };

} // namespace repository
