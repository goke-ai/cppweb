#pragma once

#include "crow_all.h"

void sendFile(crow::response &res, std::string filePath)
{
    res.set_static_file_info(CROW_STATIC_DIRECTORY + filePath);
    res.end();
}

void sendImage(crow::response &res, std::string &filePath)
{
    sendFile(res, "images/" + filePath);
}

void sendStyle(crow::response &res, std::string &filePath)
{
    sendFile(res, "styles/" + filePath);
}

void sendScript(crow::response &res, std::string &filePath)
{
    sendFile(res, "scripts/" + filePath);
}

std::string getView(std::string filePath)
{
    return crow::mustache::load(filePath).render();
}

std::string getView(std::string filePath, crow::mustache::context &ctx)
{
    return crow::mustache::load(filePath).render(ctx);
}