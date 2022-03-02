#include <stdio.h>//设有资源a,分配给n个项目
int main()
{
	int n,m;
    double a[4][8]={{0,0,0,0,0,0,0,0},
                {0,0.11,0.13,0.15,0.21,0.24,0.30,0.35},
                {0,0.12,0.16,0.21,0.23,0.25,0.24,0.34},
                {0,0.08,0.12,0.20,0.24,0.26,0.30,0.35}};
	double b[4][8];
    printf("输入项目数和资源数量：\n");
    scanf("%d",&n); 
    scanf("%d",&m); 
    for(int i=1;i<=n;i++)
    {
        for(int j=1;j<=m;j++)
        {
            for(int k=0;k<=j;k++)
            	if(b[i-1][k]+a[i][j-k]>=b[i][j])
               		b[i][j]=b[i-1][k]+a[i][j-k];  	
        }
    }
    printf("最大利润为：%.2f\n",b[n][m]);
    return 0;
}
