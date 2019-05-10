//
// Created by Hybin on 2019/5/10.
//

#ifndef WSINDUCTION_CONFIG_H
#define WSINDUCTION_CONFIG_H

#define CONFIG "../config/config.json"

#include <fstream>
#include <string>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

class Config {
public:
    // 构造函数
    Config();

    std::string get(const std::string &key);

private:
    std::string path = CONFIG;
    rapidjson::Document document;
};


#endif //WSINDUCTION_CONFIG_H
