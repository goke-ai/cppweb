#pragma once

#include "../include/crow_all.h"
#include "../../../../../goke/src/gokeCPP/hello_wnd_lnx/goke_core/query.h"
#include "../../../../../goke/src/gokeCPP/hello_wnd_lnx/goke_core/string.h"
#include "../entities/contact.h"
#include "../data/repository.h"

using namespace entities;
using namespace repository;

namespace gq = goke::query;
namespace gs = goke::string;

namespace apis
{
    class ContactsApiController
    {
    private:
        /* data */
        ContactRepository *_rp;

    public:
        ContactsApiController(ContactRepository *rp)
        {
            _rp = rp;
        }
        ~ContactsApiController() {}

        crow::json::wvalue getContacts()
        {
            crow::json::wvalue x;
            auto db = _rp->getDB();

            for (size_t i = 0; i < db.size(); i++)
            {
                auto c{db[i]};
                x["contacts"][i] = c.toJson();
            }

            return x;
        }

        crow::json::wvalue getContacts(std::string searchText)
        {
            crow::json::wvalue x;
            auto db = _rp->getDB();

            auto y = gq::filter(db, [&](Contact a)
                                {
                                    auto s1 = gs::tolower(a.getName());
                                    auto s2 = gs::tolower(searchText);

                                    auto p = s1.find(s2);
                                    return p != std::string::npos;
                                });

            for (size_t i = 0; i < y.size(); i++)
            {
                auto c{y[i]};
                x["contacts"][i] = c.toJson();
            }

            return x;
        }

        crow::json::wvalue getContact(int id)
        {
            auto db = _rp->getDB();
            auto f = gq::first(db, [=](auto a)
                               { return a.getId() == id; });
            return f.toJson();
        }

        crow::json::wvalue postContact(const crow::request &req)
        {
            auto x = crow::json::load(req.body);
            if (!x)
            {
                crow::json::wvalue w;
                w["error"] = 400;
                return w;
            }

            auto c = Contact::fromJson(x);

            c = _rp->add(c);

            return c.toJson();
        }

        crow::json::wvalue putContact(int id, const crow::request &req)
        {
            // model valid
            auto x = crow::json::load(req.body);
            if (!x)
            {
                crow::json::wvalue w;
                w["error"]["message"] = "Bad request";
                w["error"]["code"] = 400;
                return w;
            }
            auto c = Contact::fromJson(x);

            if (c.getId() != id)
            {
                crow::json::wvalue w;
                w["error"]["message"] = "Not found";
                w["error"]["code"] = 404;
                return w;
            }

            _rp->update(id, c);

            crow::json::wvalue ok;
            ok["message"] = "Success";
            ok["code"] = 200;

            return ok;
        }

        crow::json::wvalue deleteContact(int id, const crow::request &req)
        {
            _rp->remove(id);

            crow::json::wvalue ok;
            ok["message"] = "Success";
            ok["code"] = 200;

            return ok;
        }
    };

} // namespace controlers