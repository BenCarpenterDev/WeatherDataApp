#include <iostream>
#include <vector>

using namespace std;

int lab()
{
    vector<string> items = {"eggs", "milk", "sugar", "chocolate", "flour"};

    for (vector<string>::const_iterator i = items.begin(); i != items.end(); ++i) {
        // process i
        cout << *i << " "; // this will print all the contents of *features*
    }

    return 0;
}