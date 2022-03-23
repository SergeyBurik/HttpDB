#include "web-server/server_http.hpp"
#include <nlohmann/json.hpp>
#include <sys/stat.h>
#include <random>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#define SYSERROR() GetLastError()
#else
#include <errno.h>
#include "tools.h"
#define SYSERROR()  errno
#endif


using namespace std;

using json = nlohmann::json;
using HttpServer = SimpleWeb::Server<SimpleWeb::HTTP>;


bool file_exists(const string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

string read_file(string path) {
	// reading file
	string res;
	ifstream file(path);

	while (getline(file, res)) {
		cout << res << endl;
	}
	file.close();

	return res;
}

json get_json(string path) {
	// parsing json
	ifstream ifs(path);
	return json::parse(ifs);
}

template<typename Base, typename T>
inline bool instanceof(const T*) {
	// checking data types
	return is_base_of<Base, T>::value;
}


int get_collection_property(json db, string collection, string property) {
	return db[collection][property];
}

json parse_filter_types(SimpleWeb::CaseInsensitiveMultimap query_fields) {
	// parsing data types of filter keys
	json filter;
	for (auto field : query_fields) {
		// try to find type defenition
		string type = field.first.substr(field.first.find(":") + 1, field.first.length());
		string key = field.first.substr(0, field.first.find(":"));
		if (type == "int") {
			filter[key] = stoi(field.second);
		}
		else if (type == "float") {
			filter[key] = stof(field.second);
		}
		else if (type == "double") {
			filter[key] = stod(field.second);
		}
		else if (type == "string") {
			filter[key] = field.second;
		}
		else {
			filter[key] = field.second;
		}
	}
	return filter;
}

void log(string level, string msg) {
	cout << "[" << level << "]" << ": " << msg << endl;
}


string get_uuid() {
	// generate random uuid
	static random_device dev;
	static mt19937 rng(dev());

	uniform_int_distribution<int> dist(0, 15);

	const char* v = "0123456789abcdef";
	const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

	string res;
	for (int i = 0; i < 16; i++) {
		if (dash[i]) res += "-";
		res += v[dist(rng)];
		res += v[dist(rng)];
	}
	return res;
}

void write_to_file(string path, string content, bool rewrite = false) {
	// writing data to file
	std::wofstream wof;
	if (rewrite) {
		wof.open(path, std::ios::out | std::ofstream::trunc);
	}
	else {
		wof.open(path, std::ios::out | std::ios::app);
	}

	if (wof.is_open()) {
		wstring cont;
		for (int i = 0; i < content.length(); ++i)
			cont += wchar_t(content[i]);

		const wchar_t* res = cont.c_str();

		wof << res << endl;
		wof.flush();
		wof.close();
	}
	else {
		std::cerr << "Failed to open file : " << SYSERROR() << std::endl;
	}
}
