#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <set>
#include <cmath>

using namespace std;

void forward_selection(vector<vector<float>>&, int, int);
void backward_elimination(vector<vector<float>>&, int, int);
float LOO_cross_validation(vector<vector<float>>&, int, int, set<int>);
pair<set<int>, float> find_max_accuracy(vector<pair<set<int>, float>>);

int main () {
    string testFile = "small_test88.txt";
    int choice;
    ifstream inFS;

    cout << "Welcome to Alice Thai's Feature Selection Algorithm." << endl;
    cout << "Type the name of the file to test: ";
    // cin >> testFile;
    cout << endl;

    inFS.open(testFile);
    if (!inFS.is_open()) {
        cout << "Could not open " << testFile << endl;
        return 1;
    }

    istringstream inSS;
    string line, col;
    int numRows = 0;
    int numCols = 0;

    float dataNum;
    vector<vector<float>> data;
    vector<float> temp;
    while (getline(inFS, line)) {
        temp.clear();
        inSS.clear();
        inSS.str(line);
        while (inSS >> col) {
            dataNum = stof(col);
            temp.push_back(dataNum);
        }
        data.push_back(temp);     
    }

    numRows = data.size();
    numCols = data.at(0).size();
    cout << "This dataset has " << numCols - 1 << " features (not including the class attribute), with " << numRows << " instances." << endl;
    
    set<int> test;
    test.insert(1);
    test.insert(2);
    // test.insert(3);
    // test.insert(4);
    test.insert(5);
    // test.insert(6);
    // forward_selection(data, numCols - 1, numRows);
    // backward_elimination(data, numCols - 1, numRows);
    LOO_cross_validation(data, numCols, numRows, test);

    // cout << "Type the number of the algorithm you want to run:" << endl;
    // cout << "\t1) Forward Selection" << endl;
    // cout << "\t2) Backward Elimintation" << endl;
    // cin >> choice;

    // if (choice == 1) {
    //     forward_selection();
    // }
    // else if (choice == 2) {
    //     backward_elimination();
    // }
    // else {
    //     cout << "Invalid Choice." << endl;
    // }

    inFS.close();

    return 0;
}

void forward_selection(vector<vector<float>>& data, int numFeatures, int numRows) {
    set<int> added_feature;
    pair<set<int>, float> feature_set;
    vector<pair<set<int>, float>> possible_sets;

    set<int> best_features = {};
    vector<pair<set<int>, float>> best_sets;

    for (int i = 1; i <= numFeatures; ++i) {
        best_features.insert(i);
    }
    feature_set = make_pair(best_features, LOO_cross_validation(data, numFeatures, numRows, best_features));
    possible_sets.push_back(feature_set);

    pair<set<int>, float> best_feature_to_add = find_max_accuracy(possible_sets);
    cout << "Feature set {";
    set<int>::iterator itr;
    for (itr = best_feature_to_add.first.begin(); itr != best_feature_to_add.first.end(); itr++) {
        if (itr != best_feature_to_add.first.begin()) {
            cout << ",";
        }
        cout << *itr;
    }
    cout << "} was best, accuracy is " << best_feature_to_add.second << "%" << endl;
    possible_sets.clear();
    best_features.clear();
    // best_sets.push_back(feature_set);

    for (int i = 1; i <= numFeatures; ++i) {
        cout << "On level " << i << " of the search tree" << endl;
        for (int j = 1; j <= numFeatures; ++j) {
            added_feature.clear();
            added_feature = best_features;
            if (!added_feature.count(j)) {
                cout << "--Consider adding feature " << j << endl;
                added_feature.insert(j);
                feature_set = make_pair(added_feature, LOO_cross_validation(data, numFeatures, numRows, added_feature));
                possible_sets.push_back(feature_set);
            }
        }
    
        for (int i = 0; i < possible_sets.size(); ++i) {
            set<int> it = possible_sets.at(i).first;
            for (auto& str: it) {
                cout << str << " ";
            }
            cout << possible_sets.at(i).second << endl;
        }

        best_feature_to_add = find_max_accuracy(possible_sets);
        cout << "Feature set {";
        for (itr = best_feature_to_add.first.begin(); itr != best_feature_to_add.first.end(); itr++) {
            if (itr != best_feature_to_add.first.begin()) {
                cout << ",";
            }
            cout << *itr;
        }
        cout << "} was best, accuracy is " << best_feature_to_add.second << "%" << endl;

        best_sets.push_back(best_feature_to_add);
        best_features = best_feature_to_add.first;
        possible_sets.clear();
    }

    cout << endl << "best features in sequence" << endl;
    for (int i = 0; i < best_sets.size(); ++i) {
        for (itr = best_sets.at(i).first.begin(); itr != best_sets.at(i).first.end(); itr++) {
            if (itr != best_sets.at(i).first.begin()) {
                cout << ",";
            }
            cout << *itr;
        }
        cout << " " << best_sets.at(i).second << endl;
    }
}

void backward_elimination(vector<vector<float>>& data, int numFeatures, int numRows) {
    set<int> added_feature;
    pair<set<int>, float> feature_set;
    vector<pair<set<int>, float>> possible_sets;

    set<int> best_features = {};
    vector<pair<set<int>, float>> best_sets;

    for (int i = 1; i <= numFeatures; ++i) {
        best_features.insert(i);
    }
    feature_set = make_pair(best_features, LOO_cross_validation(data, numFeatures, numRows, best_features));
    possible_sets.push_back(feature_set);

    pair<set<int>, float> best_feature_to_add = find_max_accuracy(possible_sets);
    cout << "Feature set {";
    set<int>::iterator itr;
    for (itr = best_feature_to_add.first.begin(); itr != best_feature_to_add.first.end(); itr++) {
        if (itr != best_feature_to_add.first.begin()) {
            cout << ",";
        }
        cout << *itr;
    }
    cout << "} was best, accuracy is " << best_feature_to_add.second << "%" << endl;
    possible_sets.clear();
    best_sets.push_back(feature_set);

    for (int i = 1; i < numFeatures; ++i) {
        cout << "On level " << i << " of the search tree" << endl;
        for (int j = 1; j <= numFeatures; ++j) {
            added_feature = best_features;
            if (added_feature.count(j)) {
                cout << "--Consider removing feature " << j << endl;
                added_feature.erase(j);
                feature_set = make_pair(added_feature, LOO_cross_validation(data, numFeatures, numRows, added_feature));
                possible_sets.push_back(feature_set);
            }
        }
    
        for (int i = 0; i < possible_sets.size(); ++i) {
            set<int> it = possible_sets.at(i).first;
            for (auto& str: it) {
                cout << str << " ";
            }
            cout << possible_sets.at(i).second << endl;
        }

        best_feature_to_add = find_max_accuracy(possible_sets);
        cout << "Feature set {";
        for (itr = best_feature_to_add.first.begin(); itr != best_feature_to_add.first.end(); itr++) {
            if (itr != best_feature_to_add.first.begin()) {
                cout << ",";
            }
            cout << *itr;
        }
        cout << "} was best, accuracy is " << best_feature_to_add.second << "%" << endl;

        best_sets.push_back(best_feature_to_add);
        best_features = best_feature_to_add.first;
        possible_sets.clear();
    }

    cout << endl << "best features in sequence" << endl;
    for (int i = 0; i < best_sets.size(); ++i) {
        for (itr = best_sets.at(i).first.begin(); itr != best_sets.at(i).first.end(); itr++) {
            if (itr != best_sets.at(i).first.begin()) {
                cout << ",";
            }
            cout << *itr;
        }
        cout << " " << best_sets.at(i).second << endl;
    }

}

float LOO_cross_validation(vector<vector<float>>& data, int numFeatures, int numRows, set<int> features) {
    set<int>::iterator itr;
    vector<int> featuresTest;
    for(itr = features.begin(); itr != features.end(); itr++) {
        featuresTest.push_back(*itr);
    }

    //euclidean distance
    vector<float> distance;
    float sum_distances = 0;
    float euclidean = 0;
    float minDist = INFINITY;
    int minIndex = -1;
    float numCorrect = 0;
    bool match = false;
    for (int i = 0; i < numRows; ++i) {
        sum_distances = 0;
        minDist = INFINITY;
        minIndex = -1;
        for (int j = 0; j < numRows; ++j) {
            if (i != j) {
                sum_distances = 0;
                for (int k = 0; k < featuresTest.size(); ++k) {
                    sum_distances += pow(data.at(i).at(featuresTest.at(k)) - data.at(j).at(featuresTest.at(k)), 2);
                }
                euclidean = sqrt(sum_distances);
                if (euclidean < minDist) {
                    minDist = euclidean;
                    minIndex = j;
                }
            }
        }
        if (data.at(i).at(0) == data.at(minIndex).at(0)) {
            ++numCorrect;
        }
    }
    float accuracy = numCorrect / numRows;
    cout << "accuracy: " << accuracy << endl;
    return accuracy;
}

pair<set<int>, float> find_max_accuracy(vector<pair<set<int>, float>> possible_sets) {
    int maxIndex = -1;
    int bestAccuracy = 0;

    for (int i = 0; i < possible_sets.size(); ++i) {
        if (bestAccuracy < possible_sets.at(i).second) {
            bestAccuracy = possible_sets.at(i).second;
            maxIndex = i;
        }
    }

    return possible_sets.at(maxIndex);
}