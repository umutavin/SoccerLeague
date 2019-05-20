//  Created by Umut Avin on 12.05.2019.
//  Copyright © 2019 avin. All rights reserved.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string.h>
#include <queue>

using namespace std;

class League {
  public:
    int size;
    vector<int> points;
    vector< vector<int> > matches;
    vector< vector<int> > residualGraph;
    bool bfs(vector< vector<int> >& graph, int s, int t, vector<int>& path);
    int fordFulkerson(int s, int t, int size);
};

bool League::bfs(vector< vector<int> >& graph, int s, int t, vector<int>& path){
    vector<bool> visited(this->size,false); // Initial value is false
    queue<int> bfsQueue;
    visited[s] = true; // Mark source as explored.
    bfsQueue.push(s);
    
    while (!bfsQueue.empty() && !visited[t]) {
        int u = bfsQueue.front();
        bfsQueue.pop();
        
        // Check all shops can be explored from the popped shop.
        for (int i = 0; i < this->size && !visited[t]; i++) {
            if (!visited[i] && graph[u][i]>0) {
                bfsQueue.push(i);
                visited[i] = true;
                path[i] = u;
            }
        }
    }
    
    // Return the sink flag. It will be true if sink has been explored,
    // therefore there is a path between source and sink.
    return visited[t];
}

int League::fordFulkerson(int s, int t, int size) 
{ 
	int u, v; 

	// Create a residual graph and fill the residual graph with 
	// given capacities in the original graph as residual capacities 
	// in residual graph 
	this->residualGraph.resize(size);
    for(int i=0; i<size; i++){
        this->residualGraph[i].resize(size);

    }
	for (u = 0; u < size; u++) 
		for (v = 0; v < size; v++) 
			this->residualGraph[u][v] = this->matches[u][v] ; 

    vector<int> path;
	int max_flow = 0; // There is no flow initially 

	// Augment the flow while tere is path from source to sink 
	while (bfs(this->residualGraph, s, t, path)) 
	{ 
		// Find minimum residual capacity of the edges along the 
		// path filled by BFS. Or we can say find the maximum flow 
		// through the path found. 
		int path_flow = INT_MAX; 
		for (v=t; v!=s; v=path[v]) 
		{ 
			u = path[v]; 
			path_flow = min(path_flow, this->residualGraph[u][v]); 
		} 

		// update residual capacities of the edges and reverse edges 
		// along the path 
		for (v=t; v != s; v=path[v]) 
		{ 
			u = path[v]; 
			this->residualGraph[u][v] -= path_flow; 
			this->residualGraph[v][u] += path_flow; 
		} 

		// Add path flow to overall flow 
		max_flow += path_flow; 
	} 

	// Return the overall flow 
	return max_flow; 
} 


int main(){
    ifstream inputFile("input1.txt", ifstream::in);
    
    if (!inputFile.is_open()) {
        cout << endl << "File cannot be loaded!" << endl;
        return 0;
    }

    League league;
    string line;
    int N=0;

    getline(inputFile, line);
    N = stoi(line);
    league.size = N;

    getline(inputFile, line);
    stringstream ss(line);
    string point;
    string match;
    int score;
    int intMatch;
    for(int i=0; i<N; i++){
        getline(ss, point, ' ');
        score = stoi(point);
        league.points.push_back(score);
    }
    int array[N][N];
    league.matches.resize(N);

    for(int i=0; i<N; i++){
        getline(inputFile, line);
        stringstream ss2(line);
        league.matches[i].resize(N);

        for(int j=0; j<N; j++){
            getline(ss2, match, ' ');
            intMatch = stoi(match);
            league.matches[i][j] = intMatch;

        }
    }

    inputFile.close();

    cout << "max flow is: " << league.fordFulkerson(0, N-1, N);



    // for(int i=0; i<N; i++){
    //     cout << league.points[i] << " ";
    // }
    // cout << endl;

    // for(int i=0; i<N; i++){
    //     for(int j=0; j<N; j++){
    //         cout << league.matches[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    

    return 0;
}
