#include "include/config.h"
#include "include/extractor.h"

using namespace std;

int main() {
    Extractor Extractor;
    vector<vector<vector<string>>> sentences = Extractor.LoadXml("UKWAC-1.xml");

    int count = 0;
    for (auto &sentence : sentences) {
        count++;
        cout << count << " ";
        for (auto &word : sentence) {
            if (!word.empty()) {
                cout << word[0] << "/" << word[1] << " ";
            }
        }
        cout << endl;
    }

    return 0;
}