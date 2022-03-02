#include<stdio.h>
int a[100];
int Max(int a,int b)
{ 
	return ((a>=b)?a:b);
}
int Min(int a,int b)
{
    return ((a<=b)?a:b);
}
void MaxMin(int i,int j,int &max,int &min)
{
    int mid;
    if (i==j)
       min=max=a[i];
    else if(i==j-1)
    {
        if (a[i]<a[j])
        {
            min=a[i];
            max=a[j];
        }
        else
        {
            min=a[j];
            max=a[i];
        }
    }
    else
    {
        int max1,min1,max2,min2;
        mid=((i+j)/2);
        MaxMin(i,mid,max1,min1);
        MaxMin(mid+1,j,max2,min2);
        max=Max(max1,max2);
        min=Min(min1,min2);
    }
}
int main()
{
	int n,max,min;
	printf("输入要输入的数字个数:"); 
	scanf("%d",&n); 
	printf("分别输入数值\n"); 
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);  
    MaxMin(1,n,max,min);
    printf("\n\n最大值为:%d\n最小值为:%d\n\n",max,min);
    return 0;
}

