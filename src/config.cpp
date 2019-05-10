//
// Created by Hybin on 2019/5/10.
//

#include "include/config.h"

using namespace rapidjson;
using namespace std;

Config::Config()
{
    ifstream config(this->path);

    if (!config) {
        cerr << "Error: Cannot load the config file correctly." << endl;
    } else {
        IStreamWrapper json(config);

        this->document.ParseStream(json);
    }

    config.close();
}

string Config::get(const std::string &key)
{
    string value;
    if (this->document.HasMember(key.c_str())) {
        value = this->document[key.c_str()].GetString();
    }

    return value;
}
