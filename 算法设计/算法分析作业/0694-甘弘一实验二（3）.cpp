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
	printf("����n����ֵ������������ĵ�k����"); 
	scanf("%d",&n);	
	scanf("%d",&k);	
	printf("������ֵ\n"); 
	for(int i=1;i<=n;i++)		
		scanf("%d",&a[i]);
	if(n<k)	
		printf("�������");
	else 
		printf("��%d�������%d",k,Find(a,1,n,k));	
	return 0;
}

