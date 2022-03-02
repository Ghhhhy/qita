#include <stdio.h>
int a[100];
int Find(int a[],int start,int end,int k)
{	
	int temp=a[start];	
	int i=start,j=end;	
	while(i!=j)
	{		
		while(i<j&&a[j]<=temp)			
			j--;		
		while(i<j&&a[i]>=temp)			
			i++;		
		if(i<j)
		{			
			int t=a[i];			
			a[i]=a[j];			
			a[j]=t;		
		}	
	}	
	a[start]=a[i];	
	a[i]=temp;	
	//for(int i=1;i<=end;i++)		
	//	printf("%d ",a[i]);
	printf("\n");	
	if(i-start+1==k)		
		return a[i];	
	else if(i-start+1>k)
		return Find(a,start,i,k);	
	else
		return Find(a,i+1,end,k-(i-start+1));	
}
int main()
{	
	int n,k;
	printf("输入n个数值，并且输入求的第k大数"); 
	scanf("%d",&n);	
	scanf("%d",&k);	
	printf("输入数值\n"); 
	for(int i=1;i<=n;i++)		
		scanf("%d",&a[i]);
	if(n<k)	
		printf("输入错误");
	else 
		printf("第%d大的数是%d",k,Find(a,1,n,k));	
	return 0;
}

