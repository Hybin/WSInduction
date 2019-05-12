//
// Created by Hybin on 2019-05-12.
//

#ifndef WSINDUCTION_UTILS_H
#define WSINDUCTION_UTILS_H

#include <string>
#include <algorithm>
#include <vector>
#include <map>

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
}

#endif //WSINDUCTION_UTILS_H
