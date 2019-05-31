//
// Created by Hybin on 2019-05-12.
//

#ifndef WSINDUCTION_UTILS_H
#define WSINDUCTION_UTILS_H

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include "tinyxml/tinyxml2.h"

using Lexikon = std::pair<std::string, std::string>;
using Satz = std::vector<std::vector<std::string>>;

namespace utils {
    // Check the type of POS
    std::string CheckPos(const std::string &pos);

    // Transform the POS into the standard type
    std::string transform(const std::string &pos);
    
    // Split the string
    std::vector<std::string> split(const std::string &str, const std::string &delimiter);

    // Match the sentence which contains the keyword
    Satz match(const Lexikon &keyword, const Satz &sentence);

    // Check if the dataset is ready
    bool check(const std::map<std::string, int> &status);

    // Merge the vector<string> into string
    std::string join(const std::vector<std::string> &entry);

    // Set the window of context
    std::vector<int> window(const int &index, const Satz &sentence);

    // Set the window of context
    std::vector<int> screen(const int &index, std::vector<std::string> &sentence);

    // Convert the string into float
    float StringToFloat(const std::string &word);
}

#endif //WSINDUCTION_UTILS_H
