#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
using namespace std;

int main() {
	unordered_set<int> st;
	st.insert(1);
	st.insert(3);
	st.insert(5);
	st.insert(1);
	st.insert(2);
	for (int x: st) cout<<x<<" "; cout<<endl;
	return 0;
}

