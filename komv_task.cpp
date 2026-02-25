#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

double found_min_path(int n, const vector<vector<double>>& dist, vector<int>& path, vector<bool>& visited,
                      int current,
                      double currentDist,
                      double& bestDist,
                      vector<int>& bestPath) {
    

    if (path.size() == static_cast<size_t>(n)){
        currentDist += dist[current][path[0]]; 
        if (currentDist < bestDist){
            bestDist = currentDist;
            bestPath = path;
        }
        return bestDist;
    }
    
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            double lowerpath = currentDist + dist[current][i];
            
            if (lowerpath < bestDist) {
                visited[i] = true;
                path.push_back(i);
                
                found_min_path(n,dist, path, visited, i, 
                              currentDist + dist[current][i], 
                              bestDist, bestPath);
                
                path.pop_back();
                visited[i] = false;
            }
        }
    }
    
    return bestDist;
}

int main(){
    ifstream file("tsp_5_1.txt");
    
    if (!file.is_open()) {
        cerr << "File is't open!" << endl;
        return 1;
    }
    int N;//кол-во городов
    file >> N;
     vector<double> x(N), y(N);//координаты городов
    for (int i = 0; i < N; i++) {
        file >> x[i] >> y[i];
    }
    file.close();
    
    vector<vector<double>> dist(N, vector<double>(N));//расстояние между городами
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = sqrt( (x[i] - x[j])*(x[i] - x[j]) + (y[i] - y[j])*(y[i] - y[j]));
        }
    }
    vector<int> path;
    vector<bool> visited(N, false);
    double bestDist = 1e10;
    vector<int> bestPath;
    
    path.push_back(0);
    visited[0] = true;
    double result = found_min_path(N, dist, path, visited, 0, 0, bestDist, bestPath);
    cout<<"Min dist = "<<result<<endl;
    cout<<"Min path  "<<endl;
    for(int i=0; i<bestPath.size(); i++){
        cout<<bestPath[i]<<" -> ";
    }
    cout<<bestPath[0]<<" ";
    return 0;
      
}