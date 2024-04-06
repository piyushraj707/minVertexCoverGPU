#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;

vector<int> ans; //To store the final answer

void edgeToCSR(int E, int V, vector<vector<int>>& edges, vector<int>& destinations, vector<int>& rowPointers, vector<int>& degree) {
	destinations.resize(E);
	rowPointers.resize(V+1, E);
	degree.resize(V);
	sort(edges.begin(), edges.end());
	int cnt = 0;
	int lastNode = -1;
	for (int i=0; i<E; i++) {
		degree[edges[i][0]]++;
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

void print2dArray(vector<vector<int>>& mat, string name = "") {
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

void removeVertex(int node, vector<int>& rowP, vector<int>& degree, vector<int>& dest, vector<vector<int>>& edgesRemoved, int& currE) {
	//This function removes the node and the all the edges connected with the node
	for (int neighbour = rowP[node]; neighbour < rowP[node+1]; neighbour++) {
		if (degree[dest[neighbour]] == 0) continue; //If the neighbour node has been deleted before, then there is no edge between the vertex and the neighbour so skip this neighbour.
		edgesRemoved.push_back({node, dest[neighbour]});
		edgesRemoved.push_back({dest[neighbour], node});
		degree[node]--;
		degree[dest[neighbour]]--;
		currE -= 2;
	}
}

void restoreEdges(vector<int>& degree, vector<vector<int>>& edgesRemoved, int& currE) {
	//This functions re-populates the graph with the edges stored in edgesRemoved
	while (edgesRemoved.size()) {
		int a = edgesRemoved.back()[0];
		int b = edgesRemoved.back()[1];
		degree[a]++;
		edgesRemoved.pop_back();
		currE++;
	}
}

void EMVC(vector<int>& dest/*inations*/, vector<int>& rowP/*ointers*/, vector<int>& degree, unordered_set<int>& /*Vertex*/ C/*over*/, int& currE) {
	//Write the base cases
	if (currE == 0) {
		if (C.size() && (ans.size() == 0 || ans.size() > C.size())) {
			ans.resize(0);
			for (int x: C) ans.push_back(x);
		}
		return;
	}

	int MDN = max_element(degree.begin(), degree.end()) - degree.begin();//MDN: Max Degree Node or node with the maximum number of edges
	if (degree[MDN] == 0) return;

	//In the first/left branch of the search tree, we remove all the edges connected with MDN.
	vector<vector<int>> edgesRemoved; //to contain the neighbour vertices which has an edge with MDN and hence will be removed. (Note that not all neighbour necessarily have an edge with MNN, some edges might have been deleted previously)

	//remove all edges between MDN and its neighbours
	removeVertex(MDN, rowP, degree, dest, edgesRemoved, currE);

	bool MDNAlreadyExistsInC = C.count(MDN); //Does MDN already exist in Vertex Cover C?
	C.insert(MDN);
	EMVC(dest, rowP, degree, C, currE);

	//Now backtrack from the left branch of the search tree
	if (!MDNAlreadyExistsInC) C.erase(MDN); //If MDN didn't already exist in the Vertex Cover, delete it.
	restoreEdges(degree, edgesRemoved, currE);
	
	//All the changes have been reverted back. Now we can go to the right branch of the search tree.
	//In the next part, we remove the neighbours of the MDN and all the related edges.
	for (int neighbour = rowP[MDN]; neighbour < rowP[MDN+1]; neighbour++) {
		if (degree[dest[neighbour]] == 0) continue;
		removeVertex(dest[neighbour], rowP, degree, dest, edgesRemoved, currE);
		C.insert(dest[neighbour]);
	}

	//Now go the right branch of the search tree:
	EMVC(dest, rowP, degree, C, currE);

	//Now revert back the changes again
	for (vector<int>& e: edgesRemoved) C.erase(e[0]);
	restoreEdges(degree, edgesRemoved, currE);
}

int main() {
	//Taking inputs and preprocessing them__________________________________________
	int E, V; // E: the number of (undirected) edges, V: the number of vertices
	cin>>E>>V;
	vector<vector<int>> edges; //edges[i] = {a, b} shows that there is a directed edge from a to b
	for (int i=0; i<E; i++) {
		int a, b;
		cin>>a>>b;
		edges.push_back({a, b});
		edges.push_back({b, a});
	}
	E *= 2; //The number of edges becomes twice as much, since each undirected edge is converted to two directed edge.
	vector<int> destinations, // destinations stores the end nodes of all edges in ascending order of starting node;
				rowPointers, // rowPointers[i] stores the index value of destinations where ith node has edge ends;
				degree; //stores degree of each node

	edgeToCSR(E, V, edges, destinations, rowPointers, degree); //calculate destinations, rowPointers and degree given edges of the graph
	//Input taking and preprocessing done___________________________________________
	
	//calculating Minimum Vertex Cover (MVC) using the EMVC algorithm
	unordered_set<int> C; //stores the current vertex cover as the algorithm proceeds and eventually ends up with the MVC
	int currE = edges.size(); //stores the current number of edges remaining in the graph; 0 represents we have found the vertex cover
	
	EMVC(destinations, rowPointers, degree, C, currE); //The ENVC algorithm takes as inputs the graph (in the CSR format) and the vertex cover
	print1dArray(ans, "Minimum Vertex Cover");
	return 0;
}
