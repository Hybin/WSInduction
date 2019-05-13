#include "include/config.h"
#include "include/extractor.h"

using namespace std;

int main() {
    Extractor Extractor;

    // Initialize
    Extractor.initialize();

    Extractor.build();

    return 0;
}