#include "include/config.h"
#include "include/extractor.h"
#include "include/processor.h"

using namespace std;

int main() {

    // Build the Training data
    // Extractor Extractor;

    // Initialize
    // Extractor.initialize();

    // Extractor.build();

    // Build the input data
    Processor Processor;

    /*
     * Mode:
     *  - extract: extract the context of target word from .xml file
     *  - read: get the target word and its substitutes from output file
     *  - convert: build the instance - substitute pair
     */
    Processor.build("convert");

    return 0;
}