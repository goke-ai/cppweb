#define CROW_MAIN

#include <sstream>

#include "include/crow_all.h"
#include "include/web.h"
#include "include/db.h"
#include "middlewares/middlewares.h"
#include "apis/contactsApiController.h"

// using namespace entities;
using namespace apis;
using namespace repository;

int main()
{
#if defined(CROW_ENABLE_DEBUG)
    std::cout << "DEBUG Starting main ... \n";
#else
    std::cout << "Starting main ... \n";
#endif // DEBUG

    crow::App<ExampleMiddleware> app;

    app.get_middleware<ExampleMiddleware>().setMessage("hello");

    crow::mustache::set_base("templates");

    // global objects
    db::connect();
    db::createSql();
    db::insertSql();

    auto sqlData = std::make_unique<db::SqlData>();

    db::selectSql(sqlData.get());
    for (auto &&v : sqlData->columns)
    {
        std::cout << v << ", ";
    }
    std::cout << "\n";

    for (auto &&v : sqlData->data)
    {
        for (auto &&w : v)
        {
            std::cout << w << ", ";
        }
        std::cout << "\n";
    }

    db::updateSql();
    db::deleteSql();

    // migration
    const char *dbname = "app_data/goke.db";
    const char *sql = "CREATE TABLE CONTACTS("
                      "ID INT PRIMARY KEY     NOT NULL,"
                      "NAME           TEXT    NOT NULL,"
                      "PHONE        CHAR(50));";
    db::createSql(dbname, sql);
    // seeding
    sql = "INSERT INTO CONTACTS (ID, NAME, PHONE) "
          "VALUES(1, 'Goke', '+234803'); "
          "INSERT INTO CONTACTS (ID, NAME, PHONE) "
          "VALUES(2, 'Lillian', '+2348103'); "
          "INSERT INTO CONTACTS (ID, NAME, PHONE) "
          "VALUES(3, 'Lolade', '+2348104'); "
          "INSERT INTO CONTACTS (ID, NAME, PHONE) "
          "VALUES(4, 'Gbola', '+2348024'); ";
    db::insertSql(dbname, sql);

    // repository
    auto repoContact = std::make_unique<ContactRepository>();

    //

#pragma region static_file
    CROW_ROUTE(app, "/styles/<string>")
    ([](crow::response &res, std::string filename)
     { sendStyle(res, filename); });

    CROW_ROUTE(app, "/css/<string>")
    ([](crow::response &res, std::string filename)
     { sendCSS(res, filename); });

    CROW_ROUTE(app, "/scripts/<string>")
    ([](crow::response &res, std::string filename)
     { sendScript(res, filename); });

    CROW_ROUTE(app, "/js/<string>")
    ([](crow::response &res, std::string filename)
     { sendJS(res, filename); });

    CROW_ROUTE(app, "/icons/<string>")
    ([](crow::response &res, std::string filename)
     { sendFile(res, "icons/" + filename); });

    CROW_ROUTE(app, "/images/<string>")
    ([](crow::response &res, std::string filename)
     { sendImage(res, filename); });

    //
    CROW_ROUTE(app, "/data/<string>")
    ([](crow::response &res, std::string filename)
     { sendFile(res, "data/" + filename); });

    //
    CROW_ROUTE(app, "/manifest")
    ([](crow::response &res)
     {
         std::string filename{"manifest.json"};
         sendPWA(res, filename);
     });

    CROW_ROUTE(app, "/sw_assets")
    ([](crow::response &res)
     {
         std::string filename{"sw_assets.js"};
         sendPWA(res, filename);
     });

    CROW_ROUTE(app, "/sw")
    ([](crow::response &res)
     {
         std::string filename{"sw.js"};
         sendPWA(res, filename);
     });

    CROW_ROUTE(app, "/sw_index")
    ([](crow::response &res)
     {
         std::string filename{"sw_index.js"};
         sendPWA(res, filename);
     });

    CROW_ROUTE(app, "/vapidPublicKey")
    ([]()
     {
         std::string VAPID_PUBLIC_KEY = "xxsfg";
         return VAPID_PUBLIC_KEY;
     });

    CROW_ROUTE(app, "/register")
        .methods("POST"_method)(
            [&]()
            {
                return crow::response(201);
            });

    CROW_ROUTE(app, "/sendNotification")
        .methods("POST"_method)(
            [&](const crow::request &req)
            {
                auto x = crow::json::load(req.body);
                if (!x)
                {
                    crow::json::wvalue w;
                    w["error"] = 400;
                    return w;
                }

                auto subscription = x["subscription"].s();
                auto payload = x["payload"].s();
                /*

                const options = {
                TTL: req.body.ttl
                };

                setTimeout(function() {
                    webPush.sendNotification(subscription, payload, options)
                    .then(function() {
                        res.sendStatus(201);
                    })
                    .catch(function(error) {
                        console.log(error);
                        res.sendStatus(500);
                    });
                    }, req.body.delay * 1000);
                });

                */
            });

#pragma endregion static_file

#pragma region pages

    CROW_ROUTE(app, "/dashboard")
    ([]
     { return getView("dashboard.html"); });

    CROW_ROUTE(app, "/home")
    ([]
     {
         crow::mustache::context ctx;
         ctx["name"] = "Olagoke Oladokun";
         //  return crow::mustache::load("index.html").render();
         return getView("home.html", ctx);
     });

    CROW_ROUTE(app, "/about")
    ([]
     {
         crow::mustache::context ctx;
         return getView("about.html");
     });

    CROW_ROUTE(app, "/contacts/")
    ([&]
     {
         //
         ContactsApiController ctr{repoContact.get()};
         crow::mustache::context ctx = ctr.getContacts();

         return getView("contacts/index.html", ctx);
     });

#pragma endregion pages

#pragma region api

    CROW_ROUTE(app, "/api/contacts")
    ([&]
     {
         ContactsApiController ctr{repoContact.get()};
         return ctr.getContacts();
     });

    CROW_ROUTE(app, "/api/contacts/<string>")
    ([&](std::string searchText)
     {
         ContactsApiController ctr{repoContact.get()};
         return ctr.getContacts(searchText);
     });

    CROW_ROUTE(app, "/api/contacts/<int>")
    ([&](int id)
     {
         ContactsApiController ctr{repoContact.get()};
         return ctr.getContact(id);
     });

    CROW_ROUTE(app, "/api/contacts")
        .methods("POST"_method)(
            [&](const crow::request &req)
            {
                ContactsApiController ctr{repoContact.get()};
                return ctr.postContact(req);
            });

    CROW_ROUTE(app, "/api/contacts/<int>")
        .methods(crow::HTTPMethod::Put)(
            [&](const crow::request &req, int id)
            {
                ContactsApiController ctr{repoContact.get()};
                return ctr.putContact(id, req);
            });

    CROW_ROUTE(app, "/api/contacts/<int>")
        .methods(crow::HTTPMethod::Delete)(
            [&](const crow::request &req, int id)
            {
                ContactsApiController ctr{repoContact.get()};
                return ctr.deleteContact(id, req);
            });

#pragma endregion api

#pragma region old

#if defined(CROW_ENABLE_DEBUG)

    // CROW_ROUTE(app, "/")
    //     .name("hello")([]
    //                    { return "Hello World!"; });

    // CROW_ROUTE(app, "/about")
    // ([]()
    //  { return "About Crow example."; });

    // a request to /path should be forwarded to /path/
    CROW_ROUTE(app, "/path/")
    ([]()
     { return "Trailing slash test case.."; });

    // simple json response
    // To see it in action enter {ip}:18080/json
    CROW_ROUTE(app, "/json")
    ([]
     {
         crow::json::wvalue x;
         x["message"] = "Hello, World!";
         return x;
     });

    CROW_ROUTE(app, "/json2")
    ([]
     {
         crow::json::wvalue x;
         x["zmessage"] = "Hello, World!";
         x["amessage"] = "Hello, World2!";
         return x;
     });

    CROW_ROUTE(app, "/json3")
    ([]
     {
         crow::json::wvalue x;
         x["msg"][0] = "Hello, World!";
         x["msg"][1] = "Hello, World2!";
         return x;
     });

    CROW_ROUTE(app, "/json4")
    ([]
     {
         crow::json::wvalue x;
         x["msg"][0]["name"] = "Hello, World!";
         x["msg"][1]["name"] = "Hello, World2!";
         return x;
     });

    // To see it in action enter {ip}:18080/hello/{integer_between -2^32 and 100} and you should receive
    // {integer_between -2^31 and 100} bottles of beer!
    CROW_ROUTE(app, "/hello/<int>")
    ([](int count)
     {
         if (count > 100)
             return crow::response(400);
         std::ostringstream os;
         os << count << " bottles of beer!";
         return crow::response(os.str());
     });

    // To see it in action submit {ip}:18080/add/1/2 and you should receive 3 (exciting, isn't it)
    CROW_ROUTE(app, "/add/<int>/<int>")
    ([](crow::response &res, int a, int b)
     {
         std::ostringstream os;
         os << a + b;
         res.write(os.str());
         res.end();
     });

    // Compile error with message "Handler type is mismatched with URL paramters"
    //CROW_ROUTE(app,"/another/<int>")
    //([](int a, int b){
    //return crow::response(500);
    //});

    // more json example

    // To see it in action, I recommend to use the Postman Chrome extension:
    //      * Set the address to {ip}:18080/add_json
    //      * Set the method to post
    //      * Select 'raw' and then JSON
    //      * Add {"a": 1, "b": 1}
    //      * Send and you should receive 2

    // A simpler way for json example:
    //      * curl -d '{"a":1,"b":2}' {ip}:18080/add_json
    CROW_ROUTE(app, "/add_json")
        .methods("POST"_method)([](const crow::request &req)
                                {
                                    auto x = crow::json::load(req.body);
                                    if (!x)
                                        return crow::response(400);
                                    auto sum = x["a"].i() + x["b"].i();
                                    std::ostringstream os;
                                    os << sum;
                                    return crow::response{os.str()};
                                });

    // Example of a request taking URL parameters
    // If you want to activate all the functions just query
    // {ip}:18080/params?foo='blabla'&pew=32&count[]=a&count[]=b
    CROW_ROUTE(app, "/params")
    ([](const crow::request &req)
     {
         std::ostringstream os;

         // To get a simple string from the url params
         // To see it in action /params?foo='blabla'
         os << "Params: " << req.url_params << "\n\n";
         os << "The key 'foo' was " << (req.url_params.get("foo") == nullptr ? "not " : "") << "found.\n";

         // To get a double from the request
         // To see in action submit something like '/params?pew=42'
         if (req.url_params.get("pew") != nullptr)
         {
             double countD = boost::lexical_cast<double>(req.url_params.get("pew"));
             os << "The value of 'pew' is " << countD << '\n';
         }

         // To get a list from the request
         // You have to submit something like '/params?count[]=a&count[]=b' to have a list with two values (a and b)
         auto count = req.url_params.get_list("count");
         os << "The key 'count' contains " << count.size() << " value(s).\n";
         for (const auto &countVal : count)
         {
             os << " - " << countVal << '\n';
         }

         // To get a dictionary from the request
         // You have to submit something like '/params?mydict[a]=b&mydict[abcd]=42' to have a list of pairs ((a, b) and (abcd, 42))
         auto mydict = req.url_params.get_dict("mydict");
         os << "The key 'dict' contains " << mydict.size() << " value(s).\n";
         for (const auto &mydictVal : mydict)
         {
             os << " - " << mydictVal.first << " -> " << mydictVal.second << '\n';
         }

         return crow::response{os.str()};
     });

    CROW_ROUTE(app, "/large")
    ([]
     { return std::string(512 * 1024, ' '); });

    // Take a multipart/form-data request and print out its body
    CROW_ROUTE(app, "/multipart")
    ([](const crow::request &req)
     {
         crow::multipart::message msg(req);
         CROW_LOG_INFO << "body of the first part " << msg.parts[0].body;
         return "it works!";
     });

    // CROW_ROUTE(app, "/file")
    // ([](const crow::request &, crow::response &res)
    //  {
    //      //replace cat.jpg with your file path
    //      std::string filePath{"images/pix.jpg"};
    //      //  res.set_static_file_info(CROW_STATIC_DIRECTORY + filePath);
    //      //  res.end();
    //      sendImage(res, filePath);
    //  });

#endif // CROW_ENABLE_DEBUG

#pragma endregion old

#pragma region catch_all

    CROW_CATCHALL_ROUTE(app)
    ([](const crow::request &req)
     {
         // enable .html or .htm
         if (goke::string::endsWith(req.url, ".html") || goke::string::endsWith(req.url, ".htm"))
         {
             return getView(req.url);
         }

         //  // handle .js
         //  if (goke::string::ends_with(req.url, ".js"))
         //  {
         //      sendFile(res, req.url);
         //  }

         //  crow::mustache::context ctx;
         //  ctx["name"] = "Olagoke Oladokun";
         //  return crow::mustache::load("index.html").render();
         //  return getView("index.html", ctx);

         return getView("index.html");
     });

#pragma endregion catch_all

    // enables all log
    app.loglevel(crow::LogLevel::Debug);
    //crow::logger::setHandler(std::make_shared<ExampleLogHandler>());

    char *port = getenv("PORT");
    uint16_t iPort = static_cast<uint16_t>(port != NULL ? std::atoi(port) : 18080);
    std::cout << "PORT = " << iPort << "\n";
    std::cout << "http::/localhost:" << iPort << "\n";

    app.port(iPort)
        .multithreaded()
        .run();
}
