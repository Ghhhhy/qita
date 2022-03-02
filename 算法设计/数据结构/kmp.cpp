#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
#define SHUZHU 50
char T[20]={0};
char S[20]={0};
void bulid(){
	printf("输入主串");
	scanf("%s",T+1);
	printf("输入子串");
	scanf("%s",S+1);	
}
void BF(){ 
	int Tlen=strlen(T+1);
	int Slen=strlen(S+1);
	int i=1,j=1,a=1,k,l,m,h=0;
	printf("请输入从第几位开始匹配");
	scanf("%d",&i);
	printf("主串:       ");
	for(k=1;k<=Tlen;k++) 
		printf("%C ",T[k]);
	printf("\n"); 
	while(1){
	 	printf("第%d次匹配:",a);
		for(m=1;m<=i;m++)		 
			printf("  ");
		while(i<=Tlen&&j<=Slen){
			if(T[i]==S[j]){
				i++;
				j++;
			}
			else
				break;			
		} 
		for(l=1;l<=j;l++)
			printf("%C ",S[l]);
		printf("\n");
		if(j>Slen||h>=Tlen-Slen+1) {
			break;	
		}		
		if(T[i]!=S[j]){
			i=i-j+2;
			j=1;
			h=i;
			a++;		
		}
	} 
	printf("匹配了%d次",a); 
}
void KMP(){
	int next[100]={0};
	int Tlen=strlen(T+1);
	int Slen=strlen(S+1);
	int k=1;
	int q=0;
	next[1]=0;
	while(k<Slen){
		if(q==0||S[k]==S[q]){
			k++;
			q++;
			next[k]=q;
		}
		else
			q=next[q];
	}
	printf("next[j]:");
	for(int v=1;v<=Slen;v++)
		printf("%d ",next[v]);
	printf("\n");		
	int i,w,j=1,a=1,m,h=0;
	printf("请输入从第几位开始匹配");
	scanf("%d",&i);
	printf("主串:       ");
	for(k=1;k<=Tlen;k++) 
		printf("%C ",T[k]);
	printf("\n");
	while(1){	
		printf("第%d次匹配:",a);
		for(m=1;m<=i-j+1;m++)		 
			printf("  ");
		w=j;
		if(j==0)
			w=j+1;
		while(i<=Tlen&&j<=Slen)
		{
		
			if(j==0||T[i]==S[j])
			{
				i++;
				j++;
			}
			else
				break;	
		}
		for(int l=1;l<w;l++){
            if(l==1)
                printf("(");
            printf("%c ",S[l]);
            if(l==w-1)
                printf(")");        
            }
		for(int l=w;l<=j;l++)
			printf("%c ",S[l]);
		printf("\n");
		if(j>Slen||h>=Tlen-Slen+1) {
			break;	
		}
		if(T[i]!=S[j]){
			j=next[j];
			h=i;
			a++;	
		}
	}
	printf("匹配了%d次",a); 
}
void KMPval()
{	
	int nextval[100]={0};
	int Tlen=strlen(T+1);
	int Slen=strlen(S+1);
	int k=1;
	int q=0;
	nextval[1]=0;
	while(k<Slen){
		if(q==0||S[k]==S[q]){
			k++;
			q++;
			if(S[k]!=S[q])
				nextval[k]=q;
			else
			nextval[k]=nextval[q];
		}
		else
			q=nextval[q];
	}
	printf("nextval[j]:");
	for(int v=1;v<=Slen;v++)
		printf("%d ",nextval[v]);
	printf("\n");		
	int i,w,j=1,a=1,m,h=0;
	printf("请输入从第几位开始匹配");
	scanf("%d",&i);
	printf("主串:       ");
	for(k=1;k<=Tlen;k++) 
		printf("%C ",T[k]);
	printf("\n");
	while(1){	
		printf("第%d次匹配:",a);
		for(m=1;m<=i-j+1;m++)		 
			printf("  ");
		w=j;
		if(j==0)
			w=j+1;
		while(i<=Tlen&&j<=Slen){
			if(j==0||T[i]==S[j]){
				i++;
				j++;
			}
			else
				break;	
		}
		for(int l=1;l<w;l++){
            if(l==1)
                printf("(");
            printf("%c ",S[l]);
            if(l==w-1)
                printf(")");        
        }
		for(int l=w;l<=j;l++)
			printf("%c ",S[l]);
		printf("\n");
		if(j>Slen||h>=Tlen-Slen+1)
			break;	
		if(T[i]!=S[j]){
			j=nextval[j];
			h=i;
			a++;	
		}
	}
	printf("匹配了%d次",a); 
}
int main()
{
	while(1)
	{                                                          
		char id;
		printf("\n请你选择要执行的程序\n");
		printf("1．输入主串、子串和匹配起始位置\n");
		printf("2．朴素的模式匹配算法\n");
		printf("3．KMP改进算法(Next[ ])\n");
		printf("4．KMP改进算法(NextVal[ ])\n");
		printf("0．退出管理系统\n");
		scanf("%c",&id);
		switch(id)
		{
			case '1':
				bulid();
				break;
			case '2':
				BF();
				break;
			case '3':
				KMP();
				break;
			case '4':
				KMPval();
				break;						
			case '0':exit(0);
			default:
				printf("执行错误\n");
				break;
		}getchar();
	}
return 0;
}
