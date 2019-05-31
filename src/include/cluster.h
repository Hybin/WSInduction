//
// Created by Hybin on 2019-05-19.
//

#ifndef WSINDUCTION_CLUSTER_H
#define WSINDUCTION_CLUSTER_H

#include <fstream>
#include <cmath>
#include <utility>
#include <algorithm>
#include <iterator>
#include <stack>
#include <regex>
#include "fmt/format.h"
#include "config.h"
#include "utils.h"

using dict = std::map<std::string, std::map<std::string, std::vector<std::string>>>;

/**
 * Hierarchical Clustering
 */

/*
class node {
public:
    std::vector<std::string> x;
    std::vector<std::string> y;

    node(const std::vector<std::string> &a, const std::vector<std::string> &b);
};

class point {
public:
    std::vector<std::string> x;
    std::vector<std::string> y;
    std::vector<node> nodes;

    point(const std::vector<std::string> &a, const std::vector<std::string> &b);
};
*/

/**
 * DBSCAN Clustering
 */

class point {
public:
    std::vector<std::string> x;
    std::vector<std::string> y;

    int cluster = 0;

    int point_type = 1;  // 1: noise; 2: border; 3: core;

    int points = 0;

    std::vector<int> core_points;

    int visited = 0;

    point(std::vector<std::string> &a, std::vector<std::string> &b, int &c);
};

class Cluster {
public:
    Cluster();

    // Store the data into dictionary
    dict extract(const std::string &keyword);

    // Mapping the vector into text;
    void mapping(const std::string &keyword);

    // Transform the text into vectors;
    std::vector<point> fit(const std::string &file);

    // Transform the vector into the text
    std::string transform(const std::vector<std::string> &vec);

    // Count the distance
    static float distance(const point &a, const point &b);

    // Construct the clusters
    void hierarchical(std::vector<point> &dataset, const int &clusters);

    void dbscan(std::vector<point> &dataset, const float &Eps, const int &MinPts);

private:
    Config config;

    std::string lexime;

    std::map<std::vector<std::string>, std::string> collection;
};




#endif //WSINDUCTION_CLUSTER_H
