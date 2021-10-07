#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;
  
//function for printing the elements in a list
void showlist(list <int> g)
{
	list <int> :: iterator it;
	for(it = g.begin(); it != g.end(); ++it)
		cout << '\t' << *it;
	cout << '\n';
}
void showvec(vector <int> g)
{
	vector <int> :: iterator it;
	for(it = g.begin(); it != g.end(); ++it)
		cout << '\t' << *it;
	cout << '\n';
}
void rotateLeft(list<int> &aList, int offset){
	list<int>::iterator middle = aList.begin();
	for(int i = 0; i < offset; i++){
		middle = next(middle);
	}
	rotate(aList.begin(), middle, aList.end());
}

int main()
{

	std::string firstlevel ("com");
	std::string secondlevel ("cplusplus");
	std::string scheme ("http://");
	std::string hostname;
	std::string url;

	hostname = "www." + secondlevel + '.' + firstlevel;
	url = scheme + hostname;

	std::cout << url << '\n';
  
	int size = 6, ii = 3;
	// Load schedule table
	ifstream file;
	std::string filename = "scheduleTable/kin" + to_string(ii);
	cout << filename << endl;
	file.open (filename);
	string line;
	vector <int>*scheduleTable = new vector<int> [size];
	for (int i = 0; i < size; i++){
		getline(file, line);
		istringstream iss(line);
		int node;
		while (iss >> node) { scheduleTable[i].push_back(node); }
		showvec(scheduleTable[i]);
	}
  
	return 0;
  
}