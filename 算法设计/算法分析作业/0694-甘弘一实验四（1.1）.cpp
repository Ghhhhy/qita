//¹ã¶È 
#include<stdio.h>
int pre[100],a[100],b[100];
int dx[4]={1,0,-1,0};
int dy[4]={0,1,0,-1};
int mark[5][5];
int map[5][5];
void find(int d)
{   
	if(pre[d]!=0)
	{
		find(pre[d]);
		printf("(%d, %d)\n",a[d],b[d]);
	}
}
void bfs()
{    
	int x,y,head=0,tail=1;
	a[1]=0;    
	b[1]=0;    
	mark[0][0]=1;    
	pre[1]=0;       
	while(head!=tail)	
	{		
		head++;		
		for(int i=0;i<4;i++)		
		{			
			x=a[head]+dx[i];			
			y=b[head]+dy[i]; 			
			if(x<5&&y<5&&x>=0&&y>=0&&!mark[x][y]&&map[x][y]==0)			
			{				
				tail++;				
				a[tail]=x;                
				b[tail]=y;				
				pre[tail]=head;								
				mark[x][y]=1; 				
				if(a[tail]==4&&b[tail]==4)				
				{					
					printf("(0, 0)\n");					
					find(tail);									
				}			
			}					
		}	
	}
}
int main()
{	
	for(int i=0;i<5;i++)		
		for(int j=0;j<5;j++)			
			scanf("%d",&map[i][j]);	
	bfs();	
}

