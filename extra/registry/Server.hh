#pragma once

#include <iostream>
#include <mongoose.h>
#include <algorithm>
#include <iomanip>
#include <unistd.h>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <fstream>
#include <string>

#include <functional>

//-------------------------------------------------------------------------
// Request
//-------------------------------------------------------------------------

struct Request {

    enum Type { JSON_OBJECT=0, OCTET_STREAM=1};

    Request(mg_connection *conn, const std::vector<std::string> &params);

    void respondJson(std::string msg_content);

    void respondOctetStream(const void *ptr, int size);

private:

    mg_connection *conn;

public:

    int response_size;
    const std::vector<std::string> &params;
    int cache_max_age_in_seconds;
    std::string allowed_origin;
};

//-------------------------------------------------------------------------
// RequestHandler
//-------------------------------------------------------------------------

using RequestHandler = std::function<void (Request&)>;

//-------------------------------------------------------------------------
// Server
//-------------------------------------------------------------------------

struct Server {

    Server();

    void registerHandler(std::string name, RequestHandler handler);

    void start(int mongoose_threads);
    void stop();
    bool toggleTiming(bool b);
    const std::string currentDateTime();

    void *mg_callback(mg_event event, mg_connection *conn);


    int port;
    int mongoose_threads;
private:

    std::unordered_map<std::string, RequestHandler> handlers;

    bool done;
    bool is_timing;
    std::ofstream timing_of;
};
