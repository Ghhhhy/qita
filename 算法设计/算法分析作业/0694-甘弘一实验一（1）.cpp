#include <stdio.h>
#include <stdlib.h>
int Q(int n, int m)
{
    if (n<1||m<1)
        exit(0);
    else if(1==n||1==m) 
        return 1;
    else if(n<m) 
        return Q(n,n);
    else if(n==m) 
        return 1+Q(n,n-1);
    else //(n>m) 
        return Q(n,m-1)+Q(n-m,m);
}
int main()
{
    int n;
    printf("请输入一个整数：");
    scanf("%d",&n);
    if (n>0)
        printf("%d",Q(n,n));
    return 0;
}

