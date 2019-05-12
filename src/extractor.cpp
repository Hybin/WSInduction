//
// Created by Hybin on 2019/5/10.
//

#include "include/extractor.h"


using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace tinyxml2;


Extractor::Extractor() = default;

void Extractor::initialize()
{
    this->keywords = this->LoadTargets();

    for (auto &keyword : this->keywords) {
        this->collect_status[keyword.first] = 0;
    }
}

Stichworte Extractor::LoadTargets()
{
    Stichworte targets;

    // Get the path of test data
    path path(this->config.get("target_data"));

    // Get the name of files of the test data
    for (auto item = directory_iterator(path); item != directory_iterator(); item++)
    {
        if (!is_directory(item->path())) {
            vector<string> items;
            string file =  item->path().filename().string();

            items = utils::split(file, ".");   // Store the word and its pos
            targets.emplace_back(items[0], items[1]);
        } else {
            continue;
        }
    }

    return targets;
}

Satze Extractor::ReadXml(XMLElement *root)
{
    Satze sentences;

    for (XMLElement * e = root->FirstChildElement("s"); e != nullptr; e = e->NextSiblingElement("s")) {
        vector<string> words;               // Each "word  pos  lemma" in <s></s>
        satz sentence;    // Sentence in <s></s>

        const char* text = e->GetText();

        words = utils::split(text, "\n");   // Get the entry <word, pos, lemma>

        for (auto &word : words) {
            vector<string> temp, lexicon;
            lexicon = utils::split(word, "\t");  // For pos transformation

            // Transform POS into the standard type
            if (!lexicon.empty()) {
                lexicon[1] = utils::transform(lexicon[1]);
            }

            sentence.emplace_back(lexicon);
        }

        sentences.emplace_back(sentence);
    }

    return sentences;
}

map<string, Satze> Extractor::LoadXml(const string &file)
{
    Satze sentences;

    // Build the path
    path directory(this->config.get("source_data")), xml(file);
    path path = directory / xml;

    // Load the .xml file
    this->document.LoadFile(path.c_str());

    XMLElement * root = this->document.FirstChildElement("text");

    // TODO: Gather the training data
    sentences = this->ReadXml(root);

    map<string, Satze> dataset;

    while (root != nullptr) {
        // Check if the dataset is ready
        if (utils::check(this->collect_status)) {
            break;
        }

        search(sentences, dataset);

        // Update the root node
        root = root->NextSiblingElement("text");
    }

    return dataset;
}

void Extractor::search(const Satze &sentences, map<string, Satze> dataset)
{
    for (auto &keyword : this->keywords) {
        for (auto &sentence : sentences) {
            Satz data = utils::match(keyword, sentence);

            if (!data.empty()) {
                if (dataset.find(keyword.first) == dataset.end()) {
                    dataset[keyword.first] = {};
                } else {
                    dataset[keyword.first].emplace_back(data);
                }
            }
        }
    }
}

void Extractor::store(map<string, Satze> dataset)
{
    std::ofstream out(this->config.get("train_data"), ios_base::out|ios_base::ate);


}

void Extractor::build()
{
    vector<string> files;
    // Get the path of the training data
    path path(this->config.get("source_data"));

    // Get the name of training data
    for (auto item = directory_iterator(path); item != directory_iterator(); item++)
    {
        if (!is_directory(item->path())) {
            string file =  item->path().filename().string();

            files.emplace_back(file);
        } else {
            continue;
        }
    }

    map<string, Satze> dataset;

    bool complete = utils::check(this->collect_status);   // If the dataset has been ready, to be true.

    for (auto &file : files) {
        if (complete) {
            break;
        } else {
            dataset = this->LoadXml(file);

            for (auto &item : dataset) {
                this->collect_status[item.first] += item.second.size();
            }

            // Update the complete status
            complete = utils::check(this->collect_status);

            // TODO:Store the data
        }
    }
}