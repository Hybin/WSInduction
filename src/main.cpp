#include "include/config.h"
#include "include/extractor.h"
#include "include/processor.h"
#include "include/cluster.h"

using namespace std;

int main() {

    // Build the Training data
    // Extractor Extractor;

    // Initialize
    // Extractor.initialize();

    // Extractor.build();

    // Build the input data
    // Processor Processor;

    // Processor.test();

    /*
     * Mode:
     *  - extract: extract the context of target word from .xml file
     *  - read: get the target word and its substitutes from output file
     *  - convert: build the instance - substitute pair
     */
    // Processor.build("convert");

    Cluster Cluster;


    vector<string> pairs = {
            "add.pair", "dark.pair", "life.pair", "power.pair", "suggest.pair",
            "appear.pair", "date.pair", "live.pair", "read.pair", "trace.n.pair",
            "ask.pair", "dismiss.pair", "lose.pair", "serious.pair", "trace.v.pair",
            "become.pair", "familiar.pair", "meet.pair", "serve.pair", "transfer.pair",
            "board.pair", "family.pair", "new.pair", "severe.pair", "wait.pair",
            "book.n.pair", "find.pair", "number.pair", "sight.pair", "warm.pair",
            "book.v.pair", "force.pair", "paper.pair", "sound.pair", "way.pair",
            "color.pair", "help.pair", "part.pair", "state.pair", "win.pair",
            "common.pair", "image.pair", "people.pair", "strike.pair", "window.pair",
            "control.pair", "late.pair", "poor.pair", "strong.pair", "write.pair"
    };


    for (auto &pair : pairs) {
        cout << "Load the dictionary and dataset..." << endl;

        vector<point> dataset = Cluster.fit(pair);

        cout << "ok" <<endl;

        cout << "Begin to cluster...";

        Cluster.dbscan(dataset, 1.5, 100);

        cout << "done" <<endl;
    }

    return 0;
}