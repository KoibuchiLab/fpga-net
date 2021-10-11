/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 20:25:10
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-10 20:54:02
 * @ Description:
 */

// BFS algorithm in C++

#include "schedule.hpp"



// Create a Kautz with given vertices,
// and maintain an adjacency list
Kautz::Kautz(int degree, string inout) {
	d = degree;
	this->inout = inout;
	numVertices = d*(d+1);
	bfs = new vector<int>[numVertices];
	scheduleTable = new vector<Int3>[numVertices];
	adjLists = new list <int> [numVertices]; // Dont need to delete. Why??
    adjListParent = new list <int> [numVertices];
	if(inout == "out" || inout == "cmb"){ //Outgoing tree
		for (int i = 0; i <= d; i ++){
			for (int j = 0; j <= d; j ++){
				if(i != j){ // host ij
					int source = hidx2r(i, j, d);
					for (int k = 0; k <= d; k++){
						if(k != j){ //host jk
							// add host jk to adj list
							int destination = hidx2r(j, k, d);
							addEdge(source, destination);
                            addParent(destination, source);
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
						if(k != j){ // Host kj
							int src = hidx2r(k, j, d);
							addEdge(dest, src);
                            addParent(src, dest);
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

void Kautz::addParent(const int src, const int dst){
	adjListParent[src].push_back(dst);
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
		//cout << currVertex << "\t";
		bfs[startVertex].push_back(currVertex);
		queue.pop_front();

		for (i = adjLists[currVertex].begin(); i != adjLists[currVertex].end(); ++i) {
			int adjVertex = * i;
			if (!visited[adjVertex]) {
				visited[adjVertex] = true;
				queue.push_back(adjVertex);
			}
		}
	}
	//cout << endl;
}

void Kautz::printAdjAndParentList(){
    for (int i = 0; i < numVertices; i++){
        for (list<int>::iterator it = adjLists[i].begin(); it != adjLists[i].end(); it++){
			cout << *it << " ";
		}
        for (list<int>::iterator it = adjListParent[i].begin(); it != adjListParent[i].end(); it++){
            cout << *it << " ";
        }
        for (list<int>::iterator it = adjLists[i].begin(); it != adjLists[i].end(); it++){
            for (list<int>::iterator jt = adjListParent[i].begin(); jt != adjListParent[i].end(); jt++){
                if (*it == *jt){
                    cout << *it;
                    break;
                }
            }
        }
		cout << endl;
    }
}
void Kautz::printAdjList(){
	for (int i = 0; i < numVertices; i++){
		cout << i << ":\t";
		for (list<int>::iterator it = adjLists[i].begin(); it != adjLists[i].end(); it++){
			cout << *it << " ";
		}
		cout << endl;
	}
}
void Kautz::printSchedTable(){
	//cout << "Schedule table: \n";
	for (int i = 0; i < numVertices; i++){
		for(auto j: scheduleTable[i]){
			cout << j.dst << "," << j.src << "," << j.sendidx << "," << j.recvidx << "\t";
		}
		cout << endl;
	}
}

void Kautz::schedule(){
	if(inout == "in"){
		/*for (int i = 0; i < numVertices; i++){ // schedule for step 1
			for(int j = 0; j < d - 1; j++){
				//cout << bfs[i][1] << endl;
				scheduleTable[bfs[i][1]].push_back(bfs[i][j + d + 1]);
			}
		}

		for (int step = 2; step <= d; step++){ // schedule for step 2 to d
			for (int i = 0; i < numVertices; i++){
				for (int j = 0; j < d; j ++){ 
					int bfsidx = d*step + j ;
					scheduleTable[bfs[i][step]].push_back(bfs[i][bfsidx]);
				}
			}
		}
		
		for (int i = 0; i < numVertices; i++){ // Schedule for last step. Step d + 1
			for (int j = 1; j <= d; j++){
				scheduleTable[i].push_back(bfs[i][j]);
			}
		}*/
	} else { // schedule for outgoing tree
        for (int i = 0; i < numVertices; i++){
            for (int j = 0; j < numVertices - 1; j++){
                Int3 aSchedule;
                scheduleTable[i].push_back(aSchedule);
            }
        }
		for (int i = 0; i < numVertices; i++){ //Schedule for step 0
			for (int j = 1; j <= d; j ++){ 
				Int3 aSchedule;
                int saveSrc[numVertices];
                /*aSchedule.dst = bfs[i][j];
                aSchedule.dtaIndex=  i;
                aSchedule.src = -1;*/
				scheduleTable[i][j - 1].dst = bfs[i][j];
                scheduleTable[i][j - 1].sendidx =  i;
                scheduleTable[bfs[i][j]][j - 1].recvidx = i;
                scheduleTable[bfs[i][j]][j - 1].src = i;
			}
		}
		for (int i = 0; i < numVertices; i++){ // Schedule for step 1
			for (int j = 0; j < d - 1; j++){
                Int3 aSchedule;
                /*aSchedule.dst = bfs[i][j + d + 1];
                aSchedule.dtaIndex = i;
				aSchedule.src = -1;
				scheduleTable[bfs[i][1]].push_back(aSchedule);*/

                scheduleTable[bfs[i][1]][d + j].dst = bfs[i][j + d + 1];
                scheduleTable[bfs[i][1]][d + j].sendidx =  i;
                scheduleTable[bfs[i][j + d + 1]][d + j].recvidx = i;
                scheduleTable[bfs[i][j + d + 1]][d + j].src = bfs[i][1];
			}
		}
		for (int step = 2; step <= d; step++){ // Schedule for step 2 to d
			for (int i = 0; i < numVertices; i++){
				for (int j = 0; j < d; j++){
                    Int3 aSchedule;
                    /*aSchedule.dst = bfs[i][d*step + j];
                    aSchedule.dtaIndex = i;
					aSchedule.src = -1;
					scheduleTable[bfs[i][step]].push_back(aSchedule);*/

                    scheduleTable[bfs[i][step]][d*step + j - 1].dst = bfs[i][d*step + j];
                    scheduleTable[bfs[i][step]][d*step + j - 1].sendidx =  i;
                    scheduleTable[bfs[i][d*step + j]][d*step + j - 1].recvidx = i;
                    scheduleTable[bfs[i][d*step + j]][d*step + j - 1].src = bfs[i][step];
				}
			}
		}
	}
}


int main(int argc, char *argv[]) {
	//fprintf(stderr, "./%s 3 in or ./%s 3 out\n", argv[0], argv[0]);
	Kautz g(atoi(argv[1]), argv[2]);
	//g.printAdjList();

	for (int i = 0; i < g.getNumVertices(); i ++){
		g.BFS(i);
	}
	g.schedule();
    if (string(argv[2]) == "cmb"){
        g.printAdjAndParentList();
    } else {
	    g.printSchedTable();
    }
	return 0;
}