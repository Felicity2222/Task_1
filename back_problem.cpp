#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

/*void branchAndBound(int idx, int currentWeight, int currentValue,
                    const vector<int>& values,
                    const vector<int>& weights,
                    const vector<int>& order, 
                    int W, int N,
                    int& bestValue,
                    vector<bool>& bestTaken,
                    vector<bool>& currentTaken,
                    const vector<double>& ratio) {
    if (idx == N) {
        if (currentValue > bestValue) {
            bestValue = currentValue;
            bestTaken = currentTaken;
        }
        return;
    }

    int itemIdx = order[idx];
    int w = weights[itemIdx];
    int v = values[itemIdx];

    double bound = currentValue;
    int remainingWeight = W - currentWeight;
    int j = idx;
    while (j < N && weights[order[j]] <= remainingWeight){
        bound += values[order[j]];
        remainingWeight =remainingWeight - weights[order[j]];
        j++;
    }
    if (j < N){
        bound =bound + ratio[order[j]] * remainingWeight;
    }
    if (bound <= bestValue) return;
    if (currentWeight + w <= W) {
        currentTaken[itemIdx] = true;
        branchAndBound(idx + 1, currentWeight + w, currentValue + v,
                       values, weights, order, W, N,
                       bestValue, bestTaken, currentTaken, ratio);
        currentTaken[itemIdx] = false;
    }
}

vector<int> order(N);
    iota(order.begin(), order.end(), 0);
    vector<double> ratio(N);
    for (int i = 0; i < N; ++i) {
        ratio[i] = static_cast<double>(values[i]) / weights[i];
    }
    sort(order.begin(), order.end(), [&](int a, int b) {
        return ratio[a] > ratio[b];
    });

    int bestValueBB = 0;
    vector<bool> bestTakenBB(N, false);
    vector<bool> currentTakenBB(N, false);

    branchAndBound(0, 0, 0,
                   values, weights, order, W, N,
                   bestValueBB, bestTakenBB, currentTakenBB, ratio);

    cout << "\nBranch and bound result: " << bestValueBB << endl;
    cout << "BB items (index, value, weight):" << endl;
    for (int i = 0; i < N; ++i) {
        if (bestTakenBB[i]) {
            cout << "  " << i << ": " << values[i] << " " << weights[i] << endl;
        }
    }*/
int hammingNeighborhood(const vector<int>& values, const vector<int>& weights, int W, int maxAttempts = 10) {
    int N = values.size();
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine rng(seed);
    uniform_int_distribution<int> dist(0, 1);
    
    int bestGlobalValue = 0;
    vector<int> bestGlobalSolution(N, 0);
    
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        vector<int> solution(N, 0);
        int currentWeight = 0;
        int currentValue = 0;

        for (int i = 0; i < N; ++i) {
            if (dist(rng) && currentWeight + weights[i] <= W) {
                solution[i] = 1;
                currentWeight += weights[i];
                currentValue += values[i];
            }
        }
        
        bool improved;
        do {
            improved = false;
            int bestDelta = 0;
            int bestIdx = -1;
            for (int i = 0; i < N; ++i) {
                int newWeight = currentWeight;
                int newValue = currentValue;
                if (solution[i] == 0) {
                    newWeight += weights[i];
                    newValue += values[i];
                } else {
                    newWeight -= weights[i];
                    newValue -= values[i];
                }
                if (newWeight <= W && newValue > currentValue) {
                    if (newValue - currentValue > bestDelta) {
                        bestDelta = newValue - currentValue;
                        bestIdx = i;
                    }
                }
            }
            if (bestIdx != -1) {
                if (solution[bestIdx] == 0) {
                    solution[bestIdx] = 1;
                    currentWeight += weights[bestIdx];
                    currentValue += values[bestIdx];
                } else {
                    solution[bestIdx] = 0;
                    currentWeight -= weights[bestIdx];
                    currentValue -= values[bestIdx];
                }
                improved = true;
            }
        } while (improved);
        
        if (currentValue > bestGlobalValue) {
            bestGlobalValue = currentValue;
            bestGlobalSolution = solution;
        }
    }
    return bestGlobalValue;
}


int main(){
    ifstream file("ks_4_0.txt");
    
    if (!file.is_open()) {
        cerr << "File is't open!" << endl;
        return 1;
    }
    int N, W, pi, wi;//кол-во пар и вместимость рюкзака, стоимость и вес каждого предмета
    vector<pair<int,int>> v;//вещи
    file >> N >> W;
    for (int i =0; i< N; i++){
        file >>pi >>wi;
        v.push_back(make_pair(pi, wi));
    }
    file.close();
    vector<int> values(N), weights(N);
    for (int i = 0; i < N; ++i) {
        values[i] = v[i].first;
        weights[i] = v[i].second;
    }
    int hammingResult = hammingNeighborhood(values, weights, W);
    cout << "Hamming neighborhood result: " << hammingResult << endl;
    cout << endl;
    
    return 0;
}