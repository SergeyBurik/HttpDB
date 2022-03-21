#pragma once
#include "../core/Repository.h"
#include "web-server/server_http.hpp"

using namespace std;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;

class Controller{
public:
	Repository repo;
	void create_collection(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request);
	void filter(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request);
	void create(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request);
	void remove(shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request);
};

