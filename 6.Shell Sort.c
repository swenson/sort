#include<stdio.h>
#define M 50
void ShellSort (int a[],int n)
{
	int i,j,temp;	
	int h=1;
	while(h<n/3)
	
		h=(h*3)+1;
		
		while (h>=1)
		{
			for(i=h;i<n;i++)
			{
				for(j=i;j>=h;j-=h)
				{
					if(a[j]<a[j-h])
					{
						temp = a[j];
						a[j]=a[j-h];
						a[j-h]=temp;
				
					}
				}
			}
		h=h/3;
		}	
	
}
int main() 
{
	printf("\nPlease Enter the Nos of Elements");
	int N; 
	scanf("%d",&N);
	int A[M];
	printf("\nPlease Enter The Elements");
	int i;
	for(i=0;i<N;i++)
	{
		scanf("%d",&A[i]);
	}
	ShellSort(A,N);
	printf("\nThe Sorted array is: ");
	for(i=0;i<N;i++)
	{
		printf(" %d ",A[i]);
	}
	return 0;
}
