#include <bits/stdc++.h>
#define IOS ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0)
#define int long long
using namespace std;
bool next_combination (vector<int> & a, int n) {
	int k = (int)a.size();
	for (int i=k-1; i>=0; --i)
		if (a[i] < n-k+i+1) {
			++a[i];
			for (int j=i+1; j<k; ++j)
				a[j] = a[j-1]+1;
			return true;
		}
	return false;
}
main(){        	
}