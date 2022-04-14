/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 20:25:23
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-10 11:27:22
 * @ Description:
 */

#ifndef IPDPS2021_SCHEDULE_HPP_
#define IPDPS2021_SCHEDULE_HPP_

#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include "helper.hpp"

using namespace std;

class Kautz {
	int d;
    string inout;
	int numVertices;
	list<int>* adjLists;
    list<int>* adjListParent;
    vector<int>* bfs;
    vector<Int3>* scheduleTable;
	bool* visited;
	void rotateLeft(list<int> &aList, int offset){
		list<int>::iterator middle = aList.begin();
		for(int i = 0; i < offset; i++){
			middle = next(middle);
		}
		rotate(aList.begin(), middle, aList.end());
	}

	public:
		Kautz(int vertices, string aInout);
		~Kautz();
		void addEdge(const int src, const int dst);
        void addParent(const int src, const int dst);
		void addEdgeName(const char *src,const char * dest);
		void BFS(int startVertex);
		void printAdjList();
        void printAdjAndParentList();
        void printSchedTable();
        void schedule();
		int getNumVertices(){
			return numVertices;
		}
};

#endif /*IPDPS2021_SCHEDULE_HPP_*/