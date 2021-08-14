#include "../include/crow_all.h"

class ExampleLogHandler : public crow::ILogHandler
{
public:
    void log(std::string /*message*/, crow::LogLevel /*level*/) override
    {
        //            cerr << "ExampleLogHandler -> " << message;
    }
};

struct ExampleMiddleware
{
    std::string message;

    ExampleMiddleware() : message("foo")
    {
    }

    void setMessage(const std::string &newMsg)
    {
        message = newMsg;
    }

    struct context
    {
    };

    void before_handle(crow::request &req, crow::response & /*res*/, context & /*ctx*/)
    {
        CROW_LOG_DEBUG << " + MESSAGE: " << message << " - " << req.raw_url;
    }

    void after_handle(crow::request &req, crow::response & /*res*/, context & /*ctx*/)
    {
        CROW_LOG_DEBUG << " - MESSAGE: " << message << " - " << req.raw_url;
    }
};
