//
// Created by Hybin on 2019-05-12.
//

#include "include/utils.h"

using namespace std;
using namespace tinyxml2;

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

Satz utils::match(const Lexikon &keyword, const Satz &sentence)
{
    Satz data;
    for (auto &word : sentence) {
        if (!word.empty()) {
            if (word[1] == keyword.second && word[2] == keyword.first) {
                data = sentence;
                break;
            }
        }
    }

    return data;
}

bool utils::check(const map<string, int> &status)
{
    for (auto &s : status) {
        if (s.second < 20000) {
            return false;
        }
    }

    return true;
}

string utils::join(const vector<string> &entry)
{
    string result;

    for (auto &e : entry) {
        result += e + '\t';
    }

    // Trim the string
    result.erase(result.find_last_not_of('\t') + 1);
    return result;
}

void utils::clear()
{
    system("cls");
}