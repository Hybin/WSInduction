//
// Created by Hybin on 2019/5/10.
//

#ifndef WSINDUCTION_EXTRACTOR_H
#define WSINDUCTION_EXTRACTOR_H

#include <vector>
#include <utility>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <string>
#include "tinyxml/tinyxml2.h"
#include "config.h"

class Extractor {
public:
    // 构造函数
    Extractor();

    // 加载训练数据
    std::vector<std::pair<std::string, std::string>> load();

    //TODO: 构造训练数据
private:
    // tinyxml2::XMLDocument document;
    Config config;
};


#endif //WSINDUCTION_EXTRACTOR_H
