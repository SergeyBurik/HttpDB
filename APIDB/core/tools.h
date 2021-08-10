#pragma once
#include <iostream>;
#include <nlohmann/json.hpp>;
using namespace std;
using json = nlohmann::json;

bool file_exists(const string& name);
string read_file(string path);
string get_uuid();
json get_json(string path);
void write_to_file(string path, string content, bool rewrite = false);
int get_collection_property(json db, string collection, string property);
