//…Ó∂» 
#include<stdio.h>
int n,m,p,q,min=9999;
int dx[4]={1,0,0,-1};
int dy[4]={0,1,-1,0};
int map[100][100];
void dfs(int X,int Y)
{
    int x,y;    
	map[0][0]=3;
    for(int i=0;i<4;i++)
    {
       	x=X+dx[i];
       	y=Y+dy[i];
      	if(x<5&&y<5&&x>=0&&y>=0&&map[x][y]==0)
      	{
      		map[x][y]=3;
          	if (4==x&&4==y)
		 	{
				for (int i=0;i<5;++i)
 					for (int j=0;j<5;++j)
 						if(map[i][j]==3)
 							printf("(%d, %d)\n",i,j);
				map[x][y]=0;
				printf("\n");
		 	}
			else
		 		dfs(x,y);
        }
    }
   	map[X][Y]=0;
}
int main()
{
    for(int i=0;i<5;i++)
        for(int j=0;j<5;j++)
            scanf("%d",&map[i][j]);
    printf("(0, 0)\n");	       
    dfs(0,0);
} 
