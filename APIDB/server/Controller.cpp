#include "Controller.h"
#include "web-server/server_http.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

#include <iostream>
#include <boost/asio.hpp>

#include "Controller.h"
#include "../core/tools.h"

using namespace std;
using json = nlohmann::json;

using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;


void Controller::create_collection(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    try {
        // create
        json req = json::parse(request->content.string());

        repo.create_collection(req["name"]);

        response->write(SimpleWeb::StatusCode::success_ok);
    } catch (const exception& e) {
        response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
    }
}

void Controller::create(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    try {
        // create
        string collection = request->path_match[1].str();

        json req = json::parse(request->content.string());

        repo.create(collection, req);
        
        response->write(SimpleWeb::StatusCode::success_ok);
    } catch (const exception& e) {
        response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
    }
}


void Controller::remove(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    try {
        // filter
        string collection = request->path_match[1].str();
        SimpleWeb::CaseInsensitiveMultimap query_fields = request->parse_query_string();

        json filter;
        bool res;
        // query fields to json
        filter = parse_filter_types(query_fields);
        res = repo.remove(collection, filter);

        response->write(SimpleWeb::StatusCode::success_ok, "OK");
    }
    catch (const exception& e) {
        response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
    }

}

void Controller::filter(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
    try {
        // filter
        string collection = request->path_match[1].str();
        SimpleWeb::CaseInsensitiveMultimap query_fields = request->parse_query_string();

        json filter;
        string res;    
        // query fields to json
        if (query_fields.size()) {
            filter = parse_filter_types(query_fields);

            // try to find filter result in memory
            json saved_filter = repo.find_saved_filter(collection, filter);        

            if (saved_filter != NULL) {
                res = saved_filter["text"];
            } else {
                res = repo.filter(collection, filter)[collection].dump();
            }
        }
        else {
            res = repo.get_all(collection).dump();
        }

        response->write(SimpleWeb::StatusCode::success_ok, res);
    }
    catch (const exception& e) {
        response->write(SimpleWeb::StatusCode::client_error_bad_request, e.what());
    }
}


