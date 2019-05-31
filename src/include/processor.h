//
// Created by Hybin on 2019-05-16.
//

#ifndef WSINDUCTION_PROCESSOR_H
#define WSINDUCTION_PROCESSOR_H

#include <boost/filesystem.hpp>
#include <fstream>
#include <algorithm>
#include <regex>
#include "fmt/format.h"
#include "utils.h"
#include "config.h"


class Processor {
public:
    Processor();

    // 读取词表
    void load(const std::string &keyword);

    // 提取目标词语境
    void extract(const std::string &file);

    // 批量处理文件
    void build(const std::string &type);

    // 提取输出替换词
    void read(const std::string &file);

    // 转换为低维向量
    void convert(const std::string &file);

    // 提取测试集语境
    void test();
private:
    Config config;

    tinyxml2::XMLDocument document;

    std::vector<std::string> vocabulary;

    std::map<std::string, std::vector<std::string>> instance_ids;
};


#endif //WSINDUCTION_PROCESSOR_H
