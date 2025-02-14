#include<iostream>
using namespace std;
int x,y;
 void rec (int a, int b)
{
	if (a%b==0)
	{
		cout<<b;
		return;
	}
	rec (b,a%b);
}
int main ()
{
	cin>>x;
	cin>>y;
	rec (max (x,y),min (x,y));
}
