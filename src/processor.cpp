//
// Created by Hybin on 2019-05-16.
//

#include "include/processor.h"

using namespace tinyxml2;
using namespace boost::filesystem;
using namespace std;
using namespace fmt;

Processor::Processor() = default;

void Processor::load(const string &keyword)
{
    // Initialize
    this->vocabulary = {};

    path directory(this->config.get("vocabulary")), file(keyword);
    path path = directory / file;

    std::ifstream in(path.c_str(), ios_base::in);

    if (!in) {
        this->vocabulary = {};
    } else {
        string word;

        while (getline(in, word)) {
            if (!word.empty()) {
                this->vocabulary.push_back(word);
            }
        }
    }

    in.close();
}

void Processor::extract(const string &file) {
    // Get the keyword inside the file name eg. add.xml
    string keyword = utils::split(file, ".")[0];

    // Get the path of train data
    path directory(this->config.get("train_data")), xml(file), cont(keyword + ".txt");
    path input_dir(this->config.get("input_data")), path = directory / xml, outer = input_dir / cont;

    // Load the .xml file
    this->document.LoadFile(path.c_str());

    XMLElement *root = this->document.RootElement();

    vector<vector<string>> contexts;

    for (XMLElement *instance = root->FirstChildElement("instance");
         instance != nullptr; instance = instance->NextSiblingElement("instance")) {
        vector<string> words;
        vector<vector<string>> sentence;

        const char *text = instance->GetText();

        words = utils::split(text, "\n");

        int index = -1;

        for (auto &word : words) {
            vector<string> lexicon = utils::split(word, "\t");

            if (!lexicon.empty() && lexicon.size() == 3) {
                if (lexicon[2] == keyword) {
                    index = distance(words.begin(), find(words.begin(), words.end(), word));
                }
                sentence.emplace_back(lexicon);
            }
        }

        vector<int> range = utils::window(index, sentence);

        Satz temp;

        for (int i = range[0]; i < range[1]; ++i) {
            temp.push_back(sentence[i]);
        }

        vector<string> context;

        context.reserve(temp.size());
        for (auto &s : temp) {
            if (!s.empty()) {
                context.emplace_back(s[0]);
            }
        }

        contexts.emplace_back(context);
    }

    // Write the contexts into a file
    std::ofstream out(outer.c_str(), ios_base::out);

    for (auto &context : contexts) {
        for (auto &word : context) {
            out << word << " ";
        }

        out << endl;
    }

    out.close();

    cout << "Target word: " << keyword << " has processed!" << endl;
}

void Processor::build(const string &type)
{
    // Get the path of train data
    path directory(this->config.get(this->config.get(type)));

    vector<string> files;

    // Iterate the directory
    for (auto item = directory_iterator(directory); item != directory_iterator(); item++)
    {
        if (!is_directory(item->path())) {
            string file =  item->path().filename().string();
            files.push_back(file);
        } else {
            continue;
        }
    }

    // Build the input data
    for (auto &file : files) {
        if (type == "extract") {
            this->extract(file);
        } else if (type == "read") {
            this->read(file);
        } else if (type == "convert") {
            this->convert(file);
        }

    }

    cout << "complete!" << endl;
}

void Processor::read(const string &file) {
    vector<string> pieces = utils::split(file, ".");
    string keyword = pieces[0];

    this->load(keyword);

    path directory(this->config.get("output_data")), output(file);
    path path = directory / output;

    std::ifstream in(path.c_str(), ios_base::in);

    string line;
    vector<string> lines;

    while (getline(in, line)) {
        if (line.empty()) {
            continue;
        }

        vector<string> words = utils::split(line, "\t");

        string sign = words[0];
        transform(sign.begin(), sign.end(), sign.begin(), ::tolower);

        if (sign == keyword || find(this->vocabulary.begin(), this->vocabulary.end(), sign) != this->vocabulary.end()) {
            lines.push_back(line);
        } else {
            continue;
        }
    }

    if (pieces.size() > 3) {
        keyword = pieces[0] + "." + pieces[1];
    }

    string substitute_data = this->config.get("sub_data") + "/" + keyword + ".sub";

    std::ofstream out(substitute_data, ios_base::out);

    for (auto &l : lines) {
        if (!l.empty()) {
            out << l << endl;
        }
    }

    out.close();

    cout << "Target word: " << keyword << " has processed!" << endl;
}

void Processor::convert(const string &file)
{
    vector<string> pieces = utils::split(file, ".");
    string keyword = pieces[0];

    map<string, vector<string>> substitutes;

    path directory(this->config.get("sub_data")), sub(file);
    path path = directory / sub;

    std::ifstream in(path.c_str(), ios_base::in);

    string line;

    int count = 0;

    while (getline(in, line)) {
        vector<string> temp = utils::split(line, "\t"), items(temp.begin() + 1, temp.end());

        count++;

        string key = keyword + to_string(count);

        for (auto &item : items) {
            vector<string> pair = utils::split(item, " ");
            if (pair[0].length() <= 21) {
                substitutes[key].push_back(pair[0]);
            }
        }
    }

    in.close();


    // Store the data
    if (pieces.size() > 2) {
        keyword = pieces[0] + "." + pieces[1];
    }
    
    std::ofstream out(fmt::format(this->config.get("pair_data"), keyword), ios_base::out);

    for (auto &substitute : substitutes) {
        for (auto &entry : substitute.second) {
            out << substitute.first << "\t" << entry << endl;
        }
    }

    out.close();

    cout << "Target word: " << keyword << " has processed!" << endl;
}

void Processor::test()
{
    path directory(this->config.get("target_data"));

    vector<string> files;

    // Iterate the directory
    for (auto item = directory_iterator(directory); item != directory_iterator(); item++)
    {
        if (!is_directory(item->path())) {
            string file =  item->path().filename().string();
            files.push_back(file);
        } else {
            continue;
        }
    }

    map<string, vector<string>> instances_ids;

    for (auto &file : files) {
        vector<string > filename = utils::split(file, ".");

        string keyword = filename[0] + "." + filename[1];

        path xml(file), path = directory / xml;

        this->document.LoadFile(path.c_str());

        XMLElement * root = this->document.RootElement();

        // vector<vector<string>> contexts;

        vector<string> instances;

        for (XMLElement * instance = root->FirstChildElement("instance");
             instance != nullptr; instance = instance->NextSiblingElement("instance")) {
            /*
            const char *text = instance->GetText();

            string sentence;
            regex pattern(R"([,\."'\(\)!-\?\[\]]*)");

            for (int i = 0; i < strlen(text); ++i) {
                string character;
                character.push_back(text[i]);
                if (regex_match(character, pattern)) {
                    sentence.push_back(' ');
                    sentence.push_back(text[i]);
                    sentence.push_back(' ');
                } else {
                    sentence.push_back(text[i]);
                }
            }*/

            string id = instance->Attribute("id"), token = instance->Attribute("token");

            instances.push_back(id);

            /* vector<string> temp = utils::split(sentence, " "), words;

            for (auto &item : temp) {
                if (!item.empty()) {
                    continue;
                } else {
                    words.push_back(item);
                }
            }

            int index = -1;

            for (auto &word : words) {
                regex rule(token + R"([,\."'\(\)]*)");

                if (regex_match(word, rule)) {
                    index = distance(words.begin(), find(words.begin(), words.end(), word));
                }
            }

            vector<int> range = utils::screen(index, words);

            vector<string> context;

            for (int i = range[0]; i < range[1]; ++i) {
                context.push_back(words[i]);
            }

            contexts.push_back(context);*/
        }

        instances_ids[keyword] = instances;
        /*

        std::ofstream out(format(this->config.get("test_input"), keyword), ios_base::out);

        for (auto &context : contexts) {
            for (auto &word : context) {
                out << word << " ";
            }

            out << endl;
        }

        out.close();
         */
    }

    this->instance_ids = instances_ids;
}