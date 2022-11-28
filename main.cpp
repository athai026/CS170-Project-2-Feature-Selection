#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <set>

using namespace std;

void forward_selection(vector<vector<float>>, int);
void backward_elimination();
float LOO_cross_validation();
pair<set<int>, float> find_max_accuracy(vector<pair<set<int>, float>>);

int main () {
    string testFile = "small_test.txt";
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
    
    forward_selection(data, numCols - 1);
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

void forward_selection(vector<vector<float>> data, int numFeatures) {
    set<int> added_feature;
    pair<set<int>, float> feature_set;
    vector<pair<set<int>, float>> possible_sets;

    set<int> best_features = {};
    vector<pair<set<int>, float>> best_sets;
    for (int i = 1; i <= numFeatures; ++i) {
        cout << "On level " << i << " of the search tree" << endl;
        for (int j = 1; j <= numFeatures; ++j) {
            added_feature.clear();
            added_feature = best_features;
            if (!added_feature.count(j)) {
                cout << "--Consider adding feature " << j << endl;
                added_feature.insert(j);
                feature_set = make_pair(added_feature, LOO_cross_validation());
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

        best_sets.push_back(best_feature_to_add);
        best_features = best_feature_to_add.first;
        possible_sets.clear();
    }

    cout << endl << "best features in sequence" << endl;
    set<int>::iterator itr;
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

void backward_elimination() {

}

float LOO_cross_validation() {
    return rand() % 100;
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