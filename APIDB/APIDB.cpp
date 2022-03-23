// APIDB.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.

#include "core/Repository.h"
#include "server/Server.h"
#include <cstdlib>
#include "core/tools.h"


using namespace std;

void exiting() {
	// TODO: save db
	cout << "Saving data before exit.." << endl;
}

int main() {
	// initialize repository controller
	Repository repo;
	repo.db_path = "core/db/";
	repo.init();

	// init and start server
	Server server;
	server.start(8777, repo, true);

	return 0;
}
