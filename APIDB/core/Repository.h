#pragma once
#include <iostream>
#include <nlohmann/json.hpp>

using namespace std;

using json = nlohmann::json;

class Repository {
public:
	string db_path;
	json db; // list type [{"collection1": []}, {"collection2": []}]

	void init();

	void create_collection(string collection);
	json get_all(string collection);
	void create(string collection, json object);
	json find_saved_filter(string collection, json filter);
	json filter(string collection, json query);
	void update(string collection, json query, json update_data);
	bool remove(string collection, json query);
};

