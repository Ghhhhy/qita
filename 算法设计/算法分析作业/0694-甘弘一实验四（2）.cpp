#include<stdio.h>
#include<math.h>
int a[20]={0};
int judge(int j)
{
    int i=1;
    while(i<j)   
    { 
        if(a[i]==a[j]||abs(a[i]-a[j])==abs(i-j))
            return 0;
        i++;
    }
    return 1;
}
void queen(int n)
{
    int j=1,count=0;
    a[1]=0;
    while(j>0)
    { 
    	a[j]=a[j]+1;
        while(a[j]<=n&&!judge(j)) 
        	a[j]++;
        if(a[j]<=n)  
        {
            if(j==n)
                count++;
            else
            {
                j++; 
				a[j]=0;
            }
	    }
        else 
			j--;//没找到，回溯
    }
	printf("%d ",count);  
}
int main()
{
    int n;
    printf("请输入你的n皇后值： ");  
    scanf("%d",&n);
    queen(n);
    return 0;
}
