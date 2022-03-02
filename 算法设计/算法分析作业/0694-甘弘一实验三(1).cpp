#include<stdio.h>
#include<string.h>
int c[50][50]; 
int b[50][50];   
char f[50];
void print(int i,int j,int s,char x[],char y[])
{	
	if(b[i][j]==0)	
	{		
		f[s-1]=x[i-1];				
		print(i-1,j-1,s-1,x,y);	
	}	
	else if(b[i][j]==-1)			
		print(i-1,j,s,x,y);	
	else if(b[i][j]==1)		
		print(i,j-1,s,x,y);	
}
int LCS(char x[],char y[])
{	
	int i,j;	
	int longx,longy;	
	longx=strlen(x);	
	longy=strlen(y);	
	printf("   ");	
	for(i=0;i<longy;i++)			
		printf("%c  ",y[i]);		
	printf("\n");	
	for(i=1;i<=longx;i++)	
	{		
		printf("%c  ",x[i-1]);		
		for(j=1;j<=longy;j++)		
		{			
			if(x[i-1]==y[j-1])			
			{				
				c[i][j]=c[i-1][j-1]+1;				
				b[i][j]=0;				
				printf("%d  ",c[i][j]);			
			}			
			else			
			{	
				if(c[i-1][j]>c[i][j-1])	
				{
			    	c[i][j] = c[i-1][j];
					b[i][j]=-1;	
				}				
				else	
				{		
					c[i][j] = c[i][j-1];
					b[i][j] = 1;      
				}
				printf("%d  ",c[i][j]);			
			}		
		}		
		printf("\n");	
	}
	printf("X和Y的LCS是:");	
	print(longx,longy,c[longx][longy],x,y);	
	printf("%s",f);	
	printf("\n");	
	return c[longx][longy];
}
int main()
{	
	char X[50],Y[50];		
	printf("请输入字符串X:");	
	scanf("%s",X);	
	printf("请输入字符串Y:");	
	scanf("%s",Y);	
	printf("X和Y的LCS:%d \n",LCS(X,Y));
}





