#include "Repository.h"
#include <iostream>
#include <fstream>
#include <cstdint>
#include<ctime>
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include "tools.h"

#include <string>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;
using namespace std;

void Repository::init() {
	cout << "Initalizing db collections" << endl;
	path p(db_path);

	for (auto i = directory_iterator(p); i != directory_iterator(); i++) {
		if (!is_directory(i->path())) {
			string collection_file = i->path().filename().string();
			cout << collection_file << endl;

			size_t lastindex = collection_file.find_last_of(".");
			string collection = collection_file.substr(0, lastindex);

			db[collection] = get_json(db_path + collection_file);
		}
		else
			continue;
	}
	cout << "DB was initialized" << endl;
}

void Repository::create_collection(string collection) {
	// create collection function
	string path = db_path + collection + ".json";
	if (!file_exists(path)) {
		write_to_file(path, "{"" + collection + "": []}");
		json obj;
		db[collection] = obj;
		db[collection][collection] = json::array();
	}
}

json Repository::get_all(string collection) {
	// select all function
	return db[collection][collection];
}

void Repository::create(string collection, json object) {
	// create function (insert)
	string path = db_path + collection + ".json";

	json j = db[collection];

	// create id to object
	object["id"] = get_uuid();

	j[collection].insert(j[collection].begin(), object);
	write_to_file(path, j.dump(), true);
}

void Repository::update(string collection, json filter, json update_data) {
	json j = db[collection];

	for (auto& element : j[collection]) {
		bool flag = true;
		for (auto& f : filter.items()) {
			string key = f.key();

			if (element.contains(key)) {
				if (element[key] != f.value()) {
					flag = false;
					break;
				}
			}
			else {
				flag = false;
				break;
			}
		}

		if (flag) {
			// update
			for (auto& u : update_data.items()) {
				element[u.key()] = u.value();
			}
		}
	}
	// save
	write_to_file(db_path + collection + ".json", j.dump(), true);
}

bool Repository::remove(string collection, json filter) {
	json j = db[collection];

	int idx = 0;
	for (auto& element : j[collection]) {
		bool flag = true;
		for (auto& f : filter.items()) {
			string key = f.key();

			if (element.contains(key)) {
				if (element[key] != f.value()) {
					flag = false;
					break;
				}
			}
			else {
				flag = false;
				break;
			}
		}

		if (flag) {
			// delete
			//j[collection].erase(idx);
			element = NULL;
		}
		idx++;
	}

	// save
	//write_to_file(db_path + collection + ".json", j.dump(), true);
	db[collection] = j[collection];
	return true;
}

json Repository::find_saved_filter(string collection, json filter) {

	json filters = db["filters"];

	try {
		// try to find filter result by filter options
		string filters_keys_str = "";
		for (auto& f : filter.items()) {
			filters_keys_str += f.key() + f.value().dump();
		}

		string key = filters_keys_str + "$::" + collection;

		json result = filters["filters"][key];

		// if collection was not updated from the moment of last filter save
		int last_updated = db[collection]["last_updated"];

		if (last_updated && result["timestamp"] > last_updated) {
			return result;
		}

	} catch (string e) {
		cout << e << endl;;
	}

	return NULL;
}

json Repository::filter(string collection, json filter) {
	// filter data function

	json j = db[collection];

	json res = { {collection, {}} };
	res[collection] = json::array();

	for (auto& element : j[collection]) {
		bool flag = true;
		for (auto& f : filter.items()) {
			string key = f.key();

			if (!element.contains(key) || element[key] != f.value()) {
				flag = false;
				break;
			}
		}

		if (flag) {
			res[collection].insert(res[collection].begin(), element);
		}
	}


	// save result
	string filters_keys_str = "";
	for (auto& f : filter.items()) {
		filters_keys_str += f.key() + f.value().dump();
	}

	string key = filters_keys_str + "$::" + collection;

	json filters = db["filters"];
	filters["filters"][key] = { {"result", res}, {"text", res.dump()}, {"timestamp", time(0)} };

	write_to_file(db_path + "filters.json", filters.dump(), true);

	return res;
}
