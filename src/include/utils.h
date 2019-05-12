//
// Created by Hybin on 2019-05-12.
//

#ifndef WSINDUCTION_UTILS_H
#define WSINDUCTION_UTILS_H

#include <string>
#include <algorithm>
#include <vector>
#include <map>

namespace utils {
    // Check the type of POS
    std::string CheckPos(const std::string &pos);

    // Transform the POS into the standard type
    std::string transform(const std::string &pos);
    
    // Split the string
    std::vector<std::string> split(const std::string &str, const std::string &delimiter);
}

#endif //WSINDUCTION_UTILS_H
