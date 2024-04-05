#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void edgeToCSR(int E, int V, vector<vector<int>>& edges, vector<int>& destinations, vector<int>& rowPointers, vector<int>& degree) {
	destinations.resize(E);
	rowPointers.resize(V+1, E);
	degree.resize(V);
	sort(edges.begin(), edges.end());
	int cnt = 0;
	int lastNode = -1;
	for (int i=0; i<E; i++) {
		degree[edges[i][0]]++;
		degree[edges[i][1]]++;
		destinations[i] = edges[i][1];
		if (lastNode != edges[i][0]) rowPointers[edges[i][0]] = cnt;
		lastNode = edges[i][0];
		cnt++;
	}
}

void print1dArray(vector<int>& arr, string name = "") {
	if (name.length()) cout<<name<<": \n";
	for (int& x: arr) cout<<x<<" "; cout<<endl;
}

void print2dArray(vector<vector<int>>& matrix, string name = "") {
	if (name.length()) cout<<name<<": \n";
	for (vector<int>& arr: mat) print1dArray(arr, "");
}

int findMDN(vector<int>& dest, vector<int>& rowP) {
	int currMax = -1, ans = -1;
	for (int i=0; i<rowP.size()-1; i++)
		if (currMax < rowP[i+1] - rowP[i]) {
			currMax = rowP[i+1] - rowP[i];
			ans = i;
		}
	return ans;
}

void EMVC(vector<int>& dest/*inations*/, vector<int>& rowP/*ointers*/, vector<int>& degree, unordered_set<int>& /*Vertex*/ C/*over*/) {
	int MDN = max_element(degree.begin(), degree.end()) - degree.begin();//MDN: Max Degree Node or node with the maximum number of edges
	
	
}

int main() {
	//Taking inputs and preprocessing them__________________________________________
	int E, V; // E: the number of edges, V: the number of vertices
	cin>>E>>V;
	vector<vector<int>> edges(E, vector<int>(2)); //edges[i] = {a, b} shows that there is a directed edge from a to b
	for (int i=0; i<E; i++) {
		cin>>edges[i][0];
		cin>>edges[i][1];
	}
	vector<int> destinations, // destinations stores the end nodes of all edges in ascending order of starting node;
				rowPointers, // rowPointers[i] stores the index value of destinations where ith node has edge ends;
				degree; //stores degree of each node

	edgeToCSR(E, V, edges, destinations, rowPointers, degree); //calculate destinations, rowPointers and degree given edges of the graph
	//Input taking and preprocessing done___________________________________________
	

	//calculating Minimum Vertex Cover (MVC) using the EMVC algorithm
	unordered_set<int> C; //stores the current vertex cover as the algorithm proceeds and eventually ends up with the MVC
	EMVC(destinations, rowPointers, degree, C); //The ENVC algorithm takes as inputs the graph (in the CSR format) and the vertex cover
	return 0;
}
