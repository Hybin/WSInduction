//
// Created by Hybin on 2019/5/10.
//

#include "include/extractor.h"
#include <stdlib.h>


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
        Satz sentence;    // Sentence in <s></s>

        const char* text = e->GetText();

        words = utils::split(text, "\n");   // Get the entry <word, pos, lemma>

        for (auto &word : words) {
            vector<string> lexicon;
            lexicon = utils::split(word, "\t");  // For pos transformation

            // Transform POS into the standard type
            if (!lexicon.empty() && lexicon.size() == 3) {
                lexicon[1] = utils::transform(lexicon[1]);
            }

            sentence.emplace_back(lexicon);
        }

        sentences.emplace_back(sentence);
    }

    return sentences;
}

bool Extractor::LoadXml(const string &file)
{
    Satze sentences;

    // Build the path
    path directory(this->config.get("source_data")), xml(file);
    path path = directory / xml;

    // Load the .xml file
    this->document.LoadFile(path.c_str());

    XMLElement * root = this->document.FirstChildElement("text");

    map<string, Satze> dataset;

    bool complete = utils::check(this->collect_status);

    while (root != nullptr) {
        // Check if the dataset is ready
        if (complete) {
            break;
        }

        // Gather the training data
        sentences = this->ReadXml(root);

        dataset = search(sentences);

        for (auto &item : dataset) {
            if (this->collect_status[item.first] < 20000) {
                this->collect_status[item.first] += item.second.size();
            }
        }

        // Store the data
        this->store(dataset);
        this->log();

        // Update the complete status
        complete = utils::check(this->collect_status);

        // Update the root node
        root = root->NextSiblingElement("text");
    }

    return complete;
}

map<string, Satze> Extractor::search(const Satze &sentences)
{
    map<string, Satze> dataset;
    for (auto &keyword : this->keywords) {
        if (this->collect_status[keyword.first] < 20000) {
            for (auto &sentence : sentences) {
                Satz data = utils::match(keyword, sentence);

                if (!data.empty()) {
                    if (dataset.find(keyword.first) == dataset.end()) {
                        dataset[keyword.first] = {};
                    }
                    dataset[keyword.first].emplace_back(data);
                }
            }
        }

    }
    return dataset;
}

void Extractor::store(map<string, Satze> &dataset)
{
    XMLDocument doc;
    path directory(this->config.get("train_data"));

    for (auto &item : dataset) {
        if (this->collect_status[item.first] >= 20000) {
            continue;
        }
        // Build the full path
        path xml(item.first + ".xml"), path = directory / xml;

        // Check if the .xml exists
        if (!exists(path)) {
            std::ofstream out(path.c_str());
            out.close();
        }

        // Load the .xml file
        XMLError success = doc.LoadFile(path.c_str());

        XMLElement* root;

        if (success != XML_SUCCESS) {
            const char* declaration = R"(<?xml version="1.0" encoding="UTF-8"?>)";

            doc.Parse(declaration);

            // Insert the root node
            root = doc.NewElement("instances");
            doc.InsertEndChild(root);
        } else {
            root = doc.RootElement();
        }

        for (auto &sentence : item.second) {
            XMLElement* instance = doc.NewElement("instance");
            instance->SetAttribute("lemma", item.first.c_str());

            // Insert the text of sentence
            string content;

            for (auto &word : sentence) {
                string piece = utils::join(word);
                content += piece + '\n';
            }

            instance->InsertEndChild(doc.NewText(content.c_str()));
            root->InsertEndChild(instance);
        }

        doc.SaveFile(path.c_str());
    }


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

    bool complete = utils::check(this->collect_status);   // If the dataset has been ready, to be true.

    for (auto &file : files) {
        if (complete) {
            break;
        } else {
            complete = this->LoadXml(file);
        }
    }
}

void Extractor::log()
{
    cout << "INFO: The status of training data collection." << endl;
    for (auto &status: this->collect_status) {
        cout << status.first << ": " << status.second << endl;
    }
}