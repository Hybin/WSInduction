//
// Created by Hybin on 2019/5/10.
//

#ifndef WSINDUCTION_EXTRACTOR_H
#define WSINDUCTION_EXTRACTOR_H

#include <utility>
#include <boost/filesystem.hpp>
#include "tinyxml/tinyxml2.h"
#include "config.h"
#include "utils.h"

class Extractor {
public:
    // 构造函数
    Extractor();

    // 加载测试目标词
    std::vector<std::pair<std::string, std::string>> LoadTargets();

    // 加载训练数据
    std::vector<std::vector<std::vector<std::string>>> LoadXml(const std::string &file);

    // 分批读取训练数据
    static std::vector<std::vector<std::vector<std::string>>> ReadXml(tinyxml2::XMLElement * root);
private:
    tinyxml2::XMLDocument document;
    Config config;
};


#endif //WSINDUCTION_EXTRACTOR_H
