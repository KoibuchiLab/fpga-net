/**
 * @ Author: Kien Pham
 * @ Create Time: 2021-10-05 20:25:23
 * @ Modified by: Kien Pham
 * @ Modified time: 2021-10-06 15:19:07
 * @ Description:
 */

#ifndef IPDPS2021_SCHEDULE_HPP_
#define IPDPS2021_SCHEDULE_HPP_

#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

class Kautz {
	int d;
    string inout;
	int numVertices;
	list<int>* adjLists;
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
		void addEdgeName(const char *src,const char * dest);
		void BFS(int startVertex);
		void printAdjList();
		int getNumVertices(){
			return numVertices;
		}
};

#endif /*IPDPS2021_SCHEDULE_HPP_*/