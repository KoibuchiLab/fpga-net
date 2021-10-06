#include <iostream>
#include <list>
#include <iterator>
#include <algorithm>
using namespace std;
  
//function for printing the elements in a list
void showlist(list <int> g)
{
    list <int> :: iterator it;
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
  
    list <int> gqlist1, gqlist2;
  
  
    for (int i = 0; i < 10; ++i)
    {
        gqlist1.push_back(i * 2);
        gqlist2.push_front(i * 3);
    }
    cout << "\nList 1 (gqlist1) is : ";
    showlist(gqlist1);
  
    cout << "\nList 2 (gqlist2) is : ";
    showlist(gqlist2);
  
    cout << "\ngqlist1.front() : " << gqlist1.front();
    cout << "\ngqlist1.back() : " << gqlist1.back();
  
    cout << "\ngqlist1.pop_front() : ";
    gqlist1.pop_front();
    showlist(gqlist1);
  
    cout << "\ngqlist2.pop_back() : ";
    gqlist2.pop_back();
    showlist(gqlist2);
  
    cout << "\ngqlist1.reverse() : ";
    gqlist1.reverse();
    showlist(gqlist1);
  
    cout << "\ngqlist2.sort(): ";
    gqlist2.sort();
    showlist(gqlist2);

    list<int>::iterator middle = gqlist1.begin();
    middle = next(middle);
    middle = next(middle);
    middle = next(middle);

    //rotate(gqlist1.begin(), middle, gqlist1.end());
    rotateLeft(gqlist1, 1);
    showlist(gqlist1);
  
    return 0;
  
}