#include "Server.h"
#include "web-server/server_http.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "Controller.h"
#include "../core/tools.h"

using namespace std;
using namespace boost::property_tree;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;


class Workers {
public:
    boost::asio::io_service service;

private:
    boost::asio::io_service::work work;
    std::vector<std::thread> threads;

public:
    Workers(size_t number_of_threads) : work(service) {
        for (size_t c = 0; c < number_of_threads; ++c) {
            threads.emplace_back([this] {
                log("DEBUG", "Worker started");
                service.run();
            });
        }
    }
};


void Server::start(int port, Repository repo, bool debug) {
    HttpServer server;
    server.config.port = port;
    
    Workers workers(4);

    Controller controller;
    controller.repo = repo;


    // GET /
    server.resource["^/$"]["GET"] = [&workers](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
        log("DEBUG", "Request GET /");
     
        workers.service.post([response] {
            response->write(SimpleWeb::StatusCode::success_ok);
        });
    };

    // POST /collection/create
    server.resource["^/collection/create"]["POST"] = [&](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        log("DEBUG", "Request GET /collection/create");

        workers.service.post([response, request, &controller] {
            controller.create_collection(response, request);
        });
    };

    // GET /{collection}/?key=value
    server.resource["^/([a-z]+)"]["GET"] = [&workers, &controller](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        string collection = request->path_match[1].str();
        log("DEBUG", "Request GET /" + collection + "/?filter={\"key\":\"value\"}");

        workers.service.post([response, request, &controller] {
            controller.filter(response, request);
        });

    };


    // POST /{collection}
    server.resource["^/([a-z]+)"]["POST"] = [&](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        log("DEBUG", "Request POST /{collection}");

        workers.service.post([response, request, &controller] {
            controller.create(response, request);
        });
    };

    log("INFO", "Server started on port: " + to_string(port));

    server.start();
}