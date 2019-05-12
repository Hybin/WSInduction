//
// Created by Hybin on 2019/5/10.
//

#ifndef WSINDUCTION_EXTRACTOR_H
#define WSINDUCTION_EXTRACTOR_H

#include <utility>
#include <fstream>
#include <boost/filesystem.hpp>
#include "tinyxml/tinyxml2.h"
#include "config.h"
#include "utils.h"

using Satze = std::vector<std::vector<std::vector<std::string>>>;
using Stichworte = std::vector<std::pair<std::string, std::string>>;

class Extractor {
public:
    // 构造函数
    Extractor();

    // 初始化
    void initialize();

    // 加载测试目标词
    Stichworte LoadTargets();

    // 加载训练数据
    std::map<std::string, Satze> LoadXml(const std::string &file);

    // 分批读取训练数据
    static Satze ReadXml(tinyxml2::XMLElement * root);

    // 搜集训练数据
    void search(const Satze &sentences, std::map<std::string, Satze> dataset);

    // 保存训练数据
    void store(std::map<std::string, Satze> dataset);

    // 构造训练数据
    void build();
private:
    // 读取配置文件
    Config config;

    // 保存.xml文档
    tinyxml2::XMLDocument document;

    // 保存目标测试单词
    Stichworte keywords;

    // 记录训练数据搜集状态
    std::map<std::string, int> collect_status;
};


#endif //WSINDUCTION_EXTRACTOR_H
