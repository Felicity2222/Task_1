#include <iostream>
#include <vector>
#include <utility>
#include <fstream>

using namespace std;


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
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));// рюкзак
    
    for (int i = 1; i < N + 1; i++){
        for (int j = 0; j < W + 1; j++){
            if (v[i-1].second > j){
                dp[i][j] = dp[i-1][j];
            }
            else {
                dp[i][j] = max(dp[i-1][j], dp[i-1][j - v[i-1].second] + v[i-1].first);
            }       
    
        }
    }
    cout << "Maximum value " << dp[N][W] << endl;
    int w = W;
    vector<int> taken;
    for (int i = N; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            taken.push_back(i-1); 
            w -= v[i-1].second;
        }
    }
    
    cout << "Taken items "<<endl;
    for (int idx : taken) {
    cout<< " " << v[idx].first << " " << v[idx].second << endl;
}
    cout << endl;
    
    return 0;
}