//
// Created by Hybin on 2019/5/10.
//

#include "include/extractor.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;


Extractor::Extractor() = default;

vector<pair<string, string>> Extractor::load()
{
    vector<pair<string, string>> targets;

    path path(this->config.get("target_data"));

    for (auto item = directory_iterator(path); item != directory_iterator(); item++)
    {
        if (!is_directory(item->path())) {
            vector<string> items;
            string file =  item->path().filename().string();

            split(items, file, is_any_of("."), token_compress_on);
            targets.emplace_back(items[0], items[1]);
        } else {
            continue;
        }
    }

    return targets;
}