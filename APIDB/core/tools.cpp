#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#ifdef _WIN32
#include <windows.h>
#define SYSERROR()  GetLastError()
#else
#include <errno.h>
#define SYSERROR()  errno
#endif
#include <random>

using json = nlohmann::json;
using namespace std;

bool file_exists(const string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

string read_file(string path) {
	string res;
	ifstream file(path);

	while (getline(file, res)) {
		cout << res << endl;
	}
	file.close();

	return res;
}
json get_json(string path) {
	ifstream ifs(path);
	return json::parse(ifs);
}

template<typename Base, typename T>
inline bool instanceof(const T*) {
	return is_base_of<Base, T>::value;
}


int get_collection_property(json db, string collection, string property) {
	return db[collection][property];
}


string get_uuid() {
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
		//std::cout << "Saved to: " + path << std::endl;
	}
	else {
		std::cerr << "Failed to open file : " << SYSERROR() << std::endl;
	}
}
