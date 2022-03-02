#include <stdio.h> 
#include<math.h>
int A[101][101]={0};
static int t=0; 
void chess(int a,int b,int h,int s,int length)
{    
	if(length==1)       
		return;    
   	t++;    
	int tem =t;    
	int L=length/2;     
	if(h<a+L&&s<b+L)        
		chess(a,b,h,s,L);        
	else
	{        
		A[a+L-1][b+L-1]=tem;        
		chess(a,b,a+L-1,b+L-1,L);    
	}    
	if(h<a+L&&s>=b+L)        
		chess(a,b+L,h,s,L);   
	else
	{        
		A[a+L-1][b+L]=tem;        
		chess(a,b+L,a+L-1,b+L,L);    
	}     
	if(h>=a+L&&s<b+L)       
		chess(a+L,b,h,s,L);       
	else
	{        
		A[a+L][b+L-1]=tem;        
		chess(a+L,b,a+L,b+L-1,L);    
	}      
	if(h>=a+L&&s>=b+L)        
		chess(a+L,b+L,h,s,L);        
	else
	{        
		A[a+L][b+L]=tem;        
		chess(a+L,b+L,a+L,b+L,L);   
	}
} 
int main()
{    
	int n,a=1,b=1,h,s,length,m;  
	printf("输入n得棋盘的格数为2^n*2^n,行数h和列数s\n");
	scanf("%d",&n);  
	scanf("%d",&h);
	scanf("%d",&s); 
	length=pow(2,n);   
	m=length;     
	chess(a,b,h,s,length);     
	for(int i=1;i<=m;i++)
	{        
		for(int j=1;j<=m;j++)
		{                     
			printf("%2d ",A[i][j]);           
			if(j==m)
		    	printf("\n");
		}    
	}
}


