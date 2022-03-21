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
	Repository repo;
	repo.db_path = "core/db/";
	repo.init();

	Server server;
	server.start(8777, repo, true);

	//std::atexit(exiting);

	return 0;
}
