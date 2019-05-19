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

    // Processor.extract("trace.n.xml");

    // Processor.read("become.txt.out");
    Processor.convert("add.sub");

    return 0;
}