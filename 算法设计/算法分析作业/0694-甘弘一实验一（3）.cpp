#include<stdio.h>
#define input 
int a[20]; 
void func(int n,int r)
{	
	int i,j;	
	for(i=n;i>=r;i--)	
	{		
		a[r]=i;		
		if(r==1)	
		{	
			for(j=a[0];j>0;j--)			
				printf("%d ",a[j]);
			printf("\n");
		}	
		else		
			func(i-1,r-1);	
	}
}  
int main()
{	
	int n,r;	
	printf("请输入n和r的值:");	
	scanf("%d%d",&n,&r);
	if(r>n)		
		printf("输入n和r的值错误!");	
	else	
	{		
		a[0]=r;		
		func(n,r);	
	}	
	return 0;
}

 
