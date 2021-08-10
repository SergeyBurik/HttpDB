#pragma once
#include "../core/Repository.h"
class Server {
public:
	void start(int port, Repository repo, bool debug = false);
};

