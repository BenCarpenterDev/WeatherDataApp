#include <iostream>
#include <vector>

using namespace std;

void printVector(vector<string> x) {
    for (vector<string>::const_iterator i = x.begin(); i != x.end(); ++i) {
        // process i
        cout << *i << " "; // this will print all the contents of *features*
    };


int lab(){
    vector<string> items = {"eggs", "milk", "sugar", "chocolate", "flour"};

    printVector(items);

    items.pop_back();

    return 0;
}