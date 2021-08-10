#include "Server.h"
#include "web-server/server_http.hpp"

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include "Controller.h"

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
                service.run();
            });
        }
    }
};


void Server::start(int port, Repository repo, bool debug) {
    HttpServer server;
    server.config.port = 8777;
    
    Workers workers(4);

    Controller controller;
    controller.repo = repo;

    // GET /
    server.resource["^/$"]["GET"] = [&workers](std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request) {
        //controller.main(response, request);
        workers.service.post([response] {
            response->write(SimpleWeb::StatusCode::success_ok);
        });
    };

    // POST /collection/create
    server.resource["^/collection/create"]["POST"] = [&](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        workers.service.post([response, request, &controller] {
            controller.create_collection(response, request);
        });
    };

    // GET /{collection}/?filter={"key":"value"}
    server.resource["^/([a-z]+)"]["GET"] = [&workers, &controller](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {

        workers.service.post([response, request, &controller] {
            controller.filter(response, request);
        });

    };

    server.resource["^/([a-z]+)"]["POST"] = [&](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        controller.create(response, request);
    };

    cout << "Server started on port: " << port << endl;
    server.start();
}