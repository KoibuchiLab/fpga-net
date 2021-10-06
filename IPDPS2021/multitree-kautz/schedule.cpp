/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 20:25:10
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-06 15:40:43
 * @ Description:
 */

// BFS algorithm in C++

#include "schedule.hpp"
#include "helper.hpp"


// Create a Kautz with given vertices,
// and maintain an adjacency list
Kautz::Kautz(int degree, string inout) {
	d = degree;
	numVertices = d*(d+1);
	adjLists = new list <int> [numVertices]; // Dont need to delete. Why??
	if(inout == "out"){ //Outgoing tree
		for (int i = 0; i <= d; i ++){
			for (int j = 0; j <= d; j ++){
				if(i != j){ // host ij
					int source = hidx2r(i, j, d);
					for (int k = 0; k <= d; k++){
						if (k != j){ //host jk
							// add host jk to adj list
							int destination = hidx2r(j, k, d);
							addEdge(source, destination);
						}
					}
				}
			}
		}
		for (int b = 0; b <= d; b++){
			int curPosition = 0;
			for (int a = 0; a <= d; a++){
				if (a != b){ // find host that have the same b
					int index = hidx2r(a, b, d);
					rotateLeft(adjLists[index], curPosition);
					//cout << "Current Position: " << curPosition << " Index: " << index << endl ;
					curPosition++;
				}
			}
		}
	} else { // Incoming tree
		for (int i = 0; i <= d; i++){
			for (int j = 0; j <= d; j++){
				if(i != j){ // Host ji
					int dest = hidx2r(j, i, d);
					for (int k = 0; k <= d; k++){
						if(k != j){ // Host kjs
							int src = hidx2r(k, j, d);
							addEdge(dest, src);
						}
					}
				}
			}
		}
		for (int a = 0; a <= d; a++){
			int curPosition = 0;
			for (int b = 0; b <= d; b++){
				if (a != b){ // find host that have the same b
					int index = hidx2r(a, b, d);
					rotateLeft(adjLists[index], curPosition);
					//cout << "Current Position: " << curPosition << " Index: " << index << endl ;
					curPosition++;
				}
			}
		}
	}
}


Kautz::~Kautz(){
	delete visited;
}

// Add edges to the Kautz
void Kautz::addEdgeName(const char *src,const char * dest) {
	int source = h2r(src, d);
	int destination = h2r(dest, d);
	adjLists[source].push_back(destination);
	//adjLists[destination].push_back(source);
}

void Kautz::addEdge(const int src, const int dst){
	adjLists[src].push_back(dst);
}

// BFS algorithm
void Kautz::BFS(int startVertex) {
	visited = new bool[numVertices];
	for (int i = 0; i < numVertices; i++)
		visited[i] = false;

	list < int > queue;

	visited[startVertex] = true;
	queue.push_back(startVertex);

	list < int > ::iterator i;

	while (!queue.empty()) {
		int currVertex = queue.front();
		cout << currVertex << "\t";
		queue.pop_front();

		for (i = adjLists[currVertex].begin(); i != adjLists[currVertex].end(); ++i) {
			int adjVertex = * i;
			if (!visited[adjVertex]) {
				visited[adjVertex] = true;
				queue.push_back(adjVertex);
			}
		}
	}
	cout << endl;
}
void Kautz::printAdjList(){
	for (int i = 0; i < numVertices; i++){
		cout << i << ":\t";
		for(list<int>::iterator it = adjLists[i].begin(); it != adjLists[i].end(); it++){
			cout << *it << " ";
		}
		cout << endl;
	}
}


int main(int argc, char *argv[]) {
	fprintf(stderr, "./%s 3 in or ./%s 3 out\n", argv[0], argv[0]);
	Kautz g(atoi(argv[1]), argv[2]);
	g.printAdjList();

	for (int i = 0; i < g.getNumVertices(); i ++){
		g.BFS(i);
	}
	return 0;
}