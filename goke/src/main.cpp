#define CROW_MAIN

#include "includes/crow_all.h"
#include "includes/web.h"
#include "db.h"
#include "middlewares/middlewares.h"
// #include "entities/contact.h"
#include "apiControllers/contactsApiController.h"
// #include "data/repository.h"
// #include "../../goke/src/gokeCPP/hello_wnd_lnx/goke_core/query.h"

#include <sstream>

// using namespace entities;
using namespace apiControllers;
// using namespace repository;

int main()
{
    crow::App<ExampleMiddleware> app;

    app.get_middleware<ExampleMiddleware>().setMessage("hello");

    crow::mustache::set_base("templates");

    // global objects
    db::connect();

    // repository
    auto repoContact = std::make_unique<ContactRepository>();

    //

#pragma region static_file

    CROW_ROUTE(app, "/styles/<string>")
    ([](const crow::request &req, crow::response &res, std::string filename)
     { sendStyle(res, filename); });

    CROW_ROUTE(app, "/scripts/<string>")
    ([](const crow::request &req, crow::response &res, std::string filename)
     { sendScript(res, filename); });

    CROW_ROUTE(app, "/images/<string>")
    ([](const crow::request &req, crow::response &res, std::string filename)
     { sendImage(res, filename); });

#pragma endregion static_file

#pragma region pages

    CROW_ROUTE(app, "/")
    ([]
     {
         crow::mustache::context ctx;
         ctx["name"] = "Olagoke Oladokun";
         //  return crow::mustache::load("index.html").render();
         return getView("index.html", ctx);
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
        .methods(crow::HTTPMethod::PUT)(
            [&](const crow::request &req, int id)
            {
                ContactsApiController ctr{repoContact.get()};
                return ctr.putContact(id, req);
            });

    CROW_ROUTE(app, "/api/contacts/<int>")
        .methods(crow::HTTPMethod::DELETE)(
            [&](const crow::request &req, int id)
            {
                ContactsApiController ctr{repoContact.get()};
                return ctr.deleteContact(id, req);
            });

#pragma endregion api

#pragma region old

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
                                    int sum = x["a"].i() + x["b"].i();
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

#pragma endregion old

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