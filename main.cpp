//  Umut Cem Avin
//  150140018
//  Compile command: g++ 150140018.cpp -o project -std=c++11
//  Running command: ./project inputfile_name outputfile_name(optional)


#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <limits.h> 


using namespace std;

class League {
  public:
    int size;
    vector<int> points;
    vector< vector<int> > matches;
    vector< vector<int> > residualGraph;
    bool bfs(vector< vector<int> >& graph, int s, int t, vector<int>& path) const;
    int fordFulkerson(int s, int t, int size);
};

/*
Standard BFS function to check if there is a path between source and sink.
Graph is searched by placing visited flags and path is saved inside of the path vector.
*/
bool League::bfs(vector< vector<int> >& graph, int s, int t, vector<int>& path) const{
    //Create a visited array full of 0 boolean values.
    bool visited[this->size];
    for(int i=0; i<this->size; i++){
        visited[i]=false;
    }
    queue<int> bfsQueue;
    bfsQueue.push(s);
    visited[s] = true; //Visited flag for source
    
    while (!bfsQueue.empty() && !visited[t]) {
        int u = bfsQueue.front();
        bfsQueue.pop();
        
        for (int i = 0; i < this->size && !visited[t]; i++) {
            if (!visited[i] && graph[u][i]>0) {
                bfsQueue.push(i);
                visited[i] = true;
                path[i] = u;
            }
        }
    }
    return visited[t];
}

/*
Return maximum flow from source to sink if there is a path between them
*/
int League::fordFulkerson(int s, int t, int size){ 
	int u, v; 
	this->residualGraph.resize(size);
    for(int i=0; i<size; i++){
        this->residualGraph[i].resize(size);

    }
    //Create residualGraph from the main graph
	for (u = 0; u < size; u++){
		for (v = 0; v < size; v++){
			this->residualGraph[u][v] = this->matches[u][v] ; 
        }
    }
    vector<int> path;
    path.resize(size);
	int maxFlow = 0;  

    //Check repeatedly if there is a path between source to sink and update the edges
	while (bfs(this->residualGraph, s, t, path)) 
	{ 

		int pathFlow = INT_MAX; 
		for (v=t; v!=s; v=path[v]) 
		{ 
			u = path[v]; 
            if(pathFlow > this->residualGraph[u][v])
			    pathFlow = this->residualGraph[u][v]; 
		} 

		//Change residual graph according to pathFlow 
		for (v=t; v != s; v=path[v]) 
		{ 
			u = path[v]; 
			this->residualGraph[u][v] -= pathFlow; 
			this->residualGraph[v][u] += pathFlow; 
		} 

		maxFlow += pathFlow; 
	} 

	return maxFlow; 
} 


int main(int argc, const char * argv[]){
    if(argc<2){
        cout << "Missing arguements!" << endl;
        return 0;
    }
    string output;
    if(argc==3){
        output = argv[2];
    }
    if(argc==2){
        output = "output.txt";
    }
    string fileName = argv[1];


    ifstream inputFile(fileName, ifstream::in); //Open input text file
    if (!inputFile.is_open()) {
        cout << endl << "File cannot be loaded!" << endl;
        return 0;
    }

    League league;
    string line;
    int N=0;

    //Read size
    getline(inputFile, line);
    N = stoi(line);
    league.size = N;

    //Read current points
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

    //Read fixture
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

    inputFile.close(); //Close file
    
    
    ofstream outputFile(output, ofstream::out);


    //Check if there is a possibility to win the league for teams
    int cnt=0;
    int maxFlow = league.fordFulkerson(0, N-1, N);
    for(int i=0; i<N; i++){
        cnt = 0;
        for(int j=0; j<N; j++){
            if((league.points[i] + maxFlow)>league.points[j])
                cnt++;
        }
        if(cnt==N)
            outputFile << "1 ";
        else
            outputFile << "0 ";
    }
    outputFile.close();
    return 0;
}
