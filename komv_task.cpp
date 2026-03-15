#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <numeric>
#include <random>
#include <chrono>

using namespace std;

/*double found_min_path(int n, const vector<vector<double>>& dist, vector<int>& path, vector<bool>& visited,
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
*/
double totalDistance(const vector<int>& route, const vector<vector<double>>& dist) {
    double total = 0;
    int n = route.size();
    for (int i = 0; i < n - 1; ++i) {
        total += dist[route[i]][route[i+1]];
    }
    total += dist[route[n-1]][route[0]];
    return total;
}

vector<int> localSearchTSP(const vector<vector<double>>& dist, int maxIterations = 1000) {
    int n = dist.size();
    vector<int> route(n);
    iota(route.begin(), route.end(), 0);
    
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    for (int i = 0; i < n; ++i) {
    int j = rand() % n;
    swap(route[i], route[j]);
}
    
    double bestDist = totalDistance(route, dist);
    
    bool improved;
    int iteration = 0;
    
    do {
        improved = false;
        for (int i = 0; i < n - 2; ++i) {
            for (int j = i + 2; j < n; ++j) {
                int a = route[i];
                int b = route[i+1];
                int c = route[j];
                int d = route[(j+1) % n];
                
                double oldDist = dist[a][b] + dist[c][d];
                double newDist = dist[a][c] + dist[b][d];
                
                if (newDist < oldDist - 1e-9) {
                   int left = i + 1;
                   int right = j;
                    while (left < right) {
                        swap(route[left], route[right]);
                        left++;
                        right--;
                    }
                    
                    bestDist = bestDist - oldDist + newDist;
                    improved = true;
                    break; 
                }
            }
            if (improved) break;
        }
        
        iteration++;
    } while (improved && iteration < maxIterations);
    
    return route;
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
    vector<vector<double>> dist(N, vector<double>(N));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            dist[i][j] = sqrt((x[i] - x[j])*(x[i] - x[j]) + (y[i] - y[j])*(y[i] - y[j]));
        }
    }
    vector<int> bestRoute = localSearchTSP(dist);
    double bestDist = totalDistance(bestRoute, dist);
    cout << "Лучший найденный маршрут: ";
    for (int city : bestRoute) {
        cout << city << " ";
    }
    cout << bestRoute[0] << endl; // замыкаем цикл
    
    cout << "Длина маршрута: " << bestDist << endl;
   /* vector<vector<double>> dist(N, vector<double>(N));//расстояние между городами
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
    cout<<bestPath[0]<<" ";*/
    return 0;
      
}