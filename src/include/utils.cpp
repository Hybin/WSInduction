//
// Created by Hybin on 2019-05-12.
//

#include "utils.h"

using namespace std;

string utils::CheckPos(const string &pos)
{
    string type = "others";

    map<string, vector<string>> pos_tags = {
            {"pos_tag_n", {"NN", "NNS", "NP",  "NPS"}},
            {"pos_tag_v", {"VB", "VBD", "VBG", "VBN", "VBP", "VBZ", "VH", "VHD", "VHG", "VHN", "VHP", "VHZ", "VV", "VVD", "VVG", "VVN", "VVP", "VBZ"}},
            {"pos_tag_j", {"JJ", "JJR", "JJS"}}
    };

    vector<string> category = {"pos_tag_n", "pos_tag_v", "pos_tag_j"};

    for (auto &element : category) {
        vector<string> tags = pos_tags[element];
        if (find(tags.begin(), tags.end(), pos) != tags.end()) {
            type = element;
            break;
        }
    }

    return type;
}

string utils::transform(const string &pos)
{
    string type = utils::CheckPos(pos);

    map<string, string> rule = {
            {"pos_tag_n", "n"},
            {"pos_tag_v", "v"},
            {"pos_tag_j", "j"}
    };

    if (rule.find(type) != rule.end()) {
        return rule[type];
    } else {
        return pos;
    }
}

vector<string> utils::split(const string &str, const string &delimiter)
{
    vector<string> result;
    string::size_type start = 0, end = str.find(delimiter);

    while (end != string::npos) {
        result.emplace_back(str.substr(start, end - start));

        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }

    if (start != str.length()) {
        result.emplace_back(str.substr(start));
    }

    return result;
}