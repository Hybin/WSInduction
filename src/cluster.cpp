#include <utility>

//
// Created by Hybin on 2019-05-19.
//

#include "include/cluster.h"

using namespace fmt;
using namespace std;

/**
 * Hierarchical Clustering
 */
/*
node::node(const vector<string> &a, const vector<string> &b)
{
    this->x = a;
    this->y = b;
}

point::point(const vector<string> &a, const vector<string> &b)
{
    this->x = a;
    this->y = b;

    node node(a, b);

    this->nodes.push_back(node);
}
*/

Cluster::Cluster() = default;

dict Cluster::extract(const string &keyword)
{
    dict dictionary;

    string line;

    // Read the file
    ifstream in(format(this->config.get("vectors"), keyword), ios_base::in);

    while (getline(in, line)) {
        if (!line.empty()) {
            vector<string> record = utils::split(line, "\t"), vec(record.begin() + 2, record.end());

            string index = record[0].substr(0, 1), word = record[0].substr(2);

            dictionary[index][word] = vec;
        }
    }

    in.close();

    ifstream test(format(this->config.get("test_vectors"), keyword), ios_base::in);

    while (getline(test, line)) {
        if (!line.empty()) {
            vector<string> record = utils::split(line, "\t"), vec(record.begin() + 2, record.end());

            string index = record[0].substr(0, 1), word = record[0].substr(2);

            dictionary[index][word] = vec;
        }
    }

    test.close();

    return dictionary;
}

void Cluster::mapping(const string &keyword)
{
    map<vector<string>, string> dictionary;
    
    string line;
    
    // Read the file
    ifstream in(format(this->config.get("vectors"), keyword), ios_base::in);

    while (getline(in, line)) {
        if (!line.empty()) {
            vector<string> record = utils::split(line, "\t"), vec(record.begin() + 2, record.end());

            string word = record[0].substr(2);

            dictionary[vec] = word;
        }
    }

    in.close();

    ifstream test(format(this->config.get("test_vectors"), keyword), ios_base::in);

    while (getline(test, line)) {
        if (!line.empty()) {
            vector<string> record = utils::split(line, "\t"), vec(record.begin() + 2, record.end());

            string word = record[0].substr(2);

            dictionary[vec] = word;
        }
    }

    test.close();
    
    this->collection = dictionary;
    
}

vector<point> Cluster::fit(const string &file)
{
    vector<point> dataset;
    // Load the target word and its substitutes
    vector<string> filename = utils::split(file, ".");

    string keyword, line;

    if (filename.size() > 2) {
        keyword = filename[0] + "." + filename[1];
    } else {
        keyword = filename[0];
    }

    this->lexime = keyword;

    // Load the dictionary
    dict dictionary = this->extract(keyword);

    ifstream in(format(this->config.get("pair_data"), keyword), ios_base::in);

    map<string, int> count, diversity;
    int n = 0;

    while (getline(in, line)) {
        if (!line.empty()) {
            vector<string> record = utils::split(line, "\t");

            regex pattern(R"([1-9\(\)\[\]\"\'\.,!\?]*[<\/s>]*)");

            diversity[record[1]]++;

            if (record[1].empty() || regex_match(record[1], pattern) ||
                record[1].size() == 1 || diversity[record[1]] > 10) {
                continue;
            }

            count[record[0]]++;

            if (count[record[0]] <= 5) {
                point point(dictionary["0"][record[0]], dictionary["1"][record[1]], n);

                cout << record[0] << "- point " << n << "; cluster " << n << endl;

                dataset.emplace_back(point);

                ++n;
            }

            if (n > 1000) {
                break;
            }

        }
    }

    in.close();

    ifstream test(format(this->config.get("test_pair_data"), keyword), ios_base::in);

    map<string, int> t_count, t_diversity;

    while (getline(test, line)) {
        if (!line.empty()) {
            vector<string> record = utils::split(line, "\t");

            regex pattern(R"([1-9\(\)\[\]\"\'\.,!\?]*[<\/s>]*)");

            t_diversity[record[1]]++;

            if (record[1].empty() || regex_match(record[1], pattern) ||
                record[1].size() == 1 || t_diversity[record[1]] > 10) {
                continue;
            }

            t_count[record[0]]++;

            if (t_count[record[0]] <= 10) {
                point point(dictionary["0"][record[0]], dictionary["1"][record[1]], n);

                cout << record[0] << "- point " << n << "; cluster " << n << endl;

                dataset.emplace_back(point);
                
                ++n;
            }

        }
    }

    test.close();

    return dataset;
}

string Cluster::transform(const vector<string> &vec)
{
    if (this->collection.find(vec) != this->collection.end()) {
        return this->collection.find(vec)->second;
    }

    return "";
}

float Cluster::distance(const point &a, const point &b)
{
    float sum = 0.0;

    vector<float> left, right;

    for (int i = 0; i < a.x.size(); ++i) {
        float left_scalar, right_scalar;
        left_scalar = utils::StringToFloat(a.x[i]) - utils::StringToFloat(b.x[i]);
        left.push_back(left_scalar);

        right_scalar = utils::StringToFloat(a.y[i]) - utils::StringToFloat(b.y[i]);
        right.push_back(right_scalar);
    }

    for (int j = 0; j < left.size(); ++j) {
        float diff = (left[j] - right[j]) * (left[j] - right[j]);
        sum += diff;
    }

    return sqrt(sum);
}

/*
void Cluster::hierarchical(vector<point> &dataset, const int &clusters)
{
    // Create a table for similarity based on distance
    vector<vector<float>> distance_table;

    cout << "Length of DataSet:" << dataset.size() << endl;

    for (int i = 0; i < dataset.size(); ++i) {
        vector<float> similarities;

        for (int j = 0; j < dataset.size(); ++j) {
            cout << this->lexime  << " - " << "Row: " << i << " Column: " << j << endl;
            if (j > i) {
                cout << this->distance(dataset[i], dataset[j]) << endl;
                similarities.push_back(this->distance(dataset[i], dataset[j]));
            } else if (j < i) {
                similarities.push_back(distance_table[j][i]);
            } else {
                similarities.push_back(0);
            }
        }
        distance_table.push_back(similarities);
    }

    cout << "Shape of the table: " << distance_table.size() << endl;

    cout << "Begin to clustering" << endl;

    while (dataset.size() > clusters) {
        // Merge two closest clusters

        float min_distance = INT_MAX;
        int mi, mj;

        for (int i = 0; i < distance_table.size(); ++i) {
            for (int j = i + 1; j < distance_table[i].size(); ++j) {
                if (distance_table[i][j] < min_distance) {
                    min_distance = distance_table[i][j];
                    mi = i;
                    mj = j;
                }
            }
            cout << "Row " << i << "Min Distance: " << min_distance << " : " << mi << " - " << mj << endl;
        }

        for (int n=0; n < dataset[mj].nodes.size(); n++) {
            dataset[mi].nodes.push_back(dataset[mj].nodes[n]);
        }

        // Remove the one which is merged
        dataset.erase(dataset.begin() + mj);

        // Update the distance_table
        for (int k = 0; k < distance_table.size(); ++k) {
            if (k == mi || k == mj) {
                continue;
            }

            if (distance_table[mi][k] > distance_table[mj][k]) {
                distance_table[mi][k] = distance_table[mj][k];
                distance_table[k][mi] = distance_table[mi][k];
            }
        }

        // Remove
        distance_table.erase(distance_table.begin() + mj);

        for (auto &l : distance_table) {
            l.erase(l.begin() + mj);
        }
    }

    cout << "Begin to write the result...";

    this->mapping(this->lexime);

    // Output the clusters data
    ofstream clustering(format(this->config.get("res_path"), this->lexime), ios_base::out);

    for (int m = 0; m < dataset.size(); ++m) {
        for (auto & node : dataset[m].nodes) {
            clustering << this->transform(node.x) << "\t" << this->transform(node.y) << "\t" << m + 1 << endl;
        }
    }

    clustering.close();
}
*/

/**
 * DBSCAN Clustering
 */

point::point(vector<string> &a, vector<string> &b, int &c)
{
    this->x = a;
    this->y = b;
    this->cluster = c;
}

void Cluster::dbscan(vector<point> &dataset, const float &Eps, const int &MinPts)
{
    int len = dataset.size();

    cout << "calculating points..." << endl;

    for (int i = 0; i < len; ++i) {
        for (int j = i + 1; j < len; ++j) {
            if (this->distance(dataset[i], dataset[j]) < Eps) {
                dataset[i].points++;
                dataset[j].points++;
            }
        }
    }

    // core point
    cout << "core points" << endl;

    vector<point> core_point;

    for (int l = 0; l < len; ++l) {
        if (dataset[l].points >= MinPts) {
            dataset[l].point_type = 3;
            core_point.push_back(dataset[l]);
        }
    }

    cout << "joint core points" << endl;

    for (int k = 0; k < core_point.size(); ++k) {
        for (int i = k + 1; i < core_point.size(); ++i) {
            if (this->distance(core_point[k], core_point[i]) < Eps) {
                core_point[k].core_points.push_back(i);
                core_point[i].core_points.push_back(k);
            }
        }
    }

    for (int m = 0; m < core_point.size(); ++m) {
        stack<point*> ps;

        if (core_point[m].visited == 1) {
            continue;
        }

        ps.push(&core_point[m]);

        point * v;

        while (!ps.empty()) {
            v = ps.top();
            v->visited = 1;
            ps.pop();

            for (int i = 0; i < v->core_points.size(); ++i) {
                if (core_point[v->core_points[i]].visited == 1) {
                    continue;
                }

                core_point[v->core_points[i]].cluster = core_point[i].cluster;

                core_point[v->core_points[i]].visited = 1;

                ps.push(&core_point[v->core_points[i]]);
            }
        }

    }

    cout << "border point,joint border point to core point" << endl;

    for (auto &point : dataset) {
        if (point.point_type == 3) {
            continue;
        }

        for (auto &core : core_point) {
            if (this->distance(point, core) < Eps) {
                point.point_type = 2;
                point.cluster = core.cluster;
                break;
            }
        }
    }

    cout << "Begin to write the result..." << endl;

    this->mapping(this->lexime);

    // Output the clusters data
    ofstream clustering(format(this->config.get("res_path"), this->lexime), ios_base::out);

    for (auto &point : dataset) {
        if (point.point_type == 2) {
            clustering << this->transform(point.x) << "\t" << this->transform(point.y) << "\t" << point.cluster << endl;
        }
    }

    for (auto &core : core_point) {
        clustering << this->transform(core.x) << "\t" << this->transform(core.y) << "\t" << core.cluster << endl;
    }

    clustering.close();
}
