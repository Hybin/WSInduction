#include "include/config.h"
#include "include/extractor.h"

using namespace std;

int main() {
    Extractor Extractor;
    vector<pair<string, string>> texts = Extractor.load();

    for (auto &text : texts) {
        cout << text.first << "\t" << text.second << endl;
    }

    return 0;
}