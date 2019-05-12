//
// Created by Hybin on 2019/5/10.
//

#include "include/extractor.h"


using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace tinyxml2;


Extractor::Extractor() = default;

vector<pair<string, string>> Extractor::LoadTargets()
{
    vector<pair<string, string>> targets;

    path path(this->config.get("target_data"));

    for (auto item = directory_iterator(path); item != directory_iterator(); item++)
    {
        if (!is_directory(item->path())) {
            vector<string> items;
            string file =  item->path().filename().string();

            items = utils::split(file, ".");
            targets.emplace_back(items[0], items[1]);
        } else {
            continue;
        }
    }

    return targets;
}

vector<vector<vector<string>>> Extractor::ReadXml(XMLElement *root)
{
    vector<vector<vector<string>>> sentences;

    for (XMLElement * e = root->FirstChildElement("s"); e != nullptr; e = e->NextSiblingElement("s")) {
        vector<string> words;               // Each "word  pos  lemma" in <s></s>
        vector<vector<string>> sentence;    // Sentence in <s></s>

        const char* text = e->GetText();

        words = utils::split(text, "\n");

        for (auto &word : words) {
            vector<string> temp, lexicon;
            lexicon = utils::split(word, "\t");

            // Map POS into the standard type
            if (!lexicon.empty()) {
                lexicon[1] = utils::transform(lexicon[1]);
            }

            sentence.emplace_back(lexicon);
        }

        sentences.emplace_back(sentence);
    }

    return sentences;
}

vector<vector<vector<string>>> Extractor::LoadXml(const string &file)
{
    vector<vector<vector<string>>> sentences;

    // Build the path
    path directory(this->config.get("source_data")), xml(file);
    path path = directory / xml;

    // Load the .xml file
    this->document.LoadFile(path.c_str());

    XMLElement * root = this->document.FirstChildElement("text");

    // TODO: Gather the training data
    sentences = this->ReadXml(root);

    return sentences;
}