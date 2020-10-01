#include<stdio.h>
#include<iostream>
using namespace std;
ShellSort(int A[],int n)
{
for(int gap=n/2;gap>=1;gap=gap/2)
{
	for(int j=gap;j<n;j++)
	{
		for(int i=j-gap;i>=0;i=i-gap)
		{
			if(A[i+gap]>A[i])
			break;
			else
			{
				int temp=A[i+gap];
				A[i+gap]=A[i];
				A[i]=temp;
			}
		}
	}
}
}
int main()
{
	int n,data,num;
	cout<<"enter no of elements"<<endl;
	cin>>n;
	int A[n];
	for(int i=0;i<n;i++)
	{cout<<"enter the data"<<endl;
	cin>>data;
		A[i]=data;
		}
		ShellSort(A,n);
		cout<<"new array is ";
		for(int i=0;i<n;i++)
		cout<<" "<<A[i];
		return 0;
}
