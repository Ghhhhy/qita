#include<iostream> 
#include<cstdlib>
#define N 1000
using namespace std;
int sum=0;
int head=0,step=0;
int m=0,ta=0;
int zuixiao[N];
char cdir[4]={'u','r','d','l'};// �����Ӧ��ĸ 
char opcdir[4]={'d','l','u','r'};//�������Ӧ��ĸ 
const int aim_state[9]={1,2,3,8,0,4,7,6,5};//Ŀ��״̬
const int aim_loc[9]={4,0,1,2,5,8,7,6,3};//ÿ�����ֵ�Ŀ��λ��
struct bashuma
{
    int ccstate[9];
    int a;
    char fx;
    int flag;
    int pre;
    int cengshu;
};
inline int Abs(int a)
{
    return a>0?a:-a;//����ֵ���� 
}
inline int Manh(int loc1,int loc2) 
{
    return Abs(loc1/3-loc2/3)+Abs(loc1%3-loc2%3);//���������֮��Ĳ�ĺͼ�Ϊ�õ�������ǵ�Ŀ��λ�ú�Ϊ����ֵ
}
inline int h(int state[9])//��1��8�ĵ��λ�þ������ǵ�Ŀ��λ�ú�Ϊ����ֵ 
{
    int ans=0;
    for(int i=0;i<9;i++)
    if(state[i])
    ans+=Manh(i,aim_loc[state[i]]);
    return ans;
}
inline int check(int state[9])//�Ƚ��Ƿ�ɹ� 
{
    for(int i=0;i<9;i++)
    if(state[i]!=aim_state[i])
        return 0;
    return 1;
}
inline bool canMove(int loc,int dir,int&nxt)//�жϿո���locλ��ʱ�ܷ���dir�����ƶ���ͬʱ�����ƶ����λ��nxt 
{
    switch(dir)
    {
        case 0:
            nxt=loc-3;    //�� 
            return loc>2;
        case 1:
            nxt=loc+1;    //�� 
            return loc%3!=2;
        case 2:
            nxt=loc+3;
            return loc<6;//�� 
        case 3:
            nxt=loc-1;
            return loc%3;//�� 
    }
}
bashuma chushihua(bashuma bsm[])
{
    int k;
    //int bsmchushihua[9]={2,8,3,1,6,4,7,0,5};//�岽����
    int bsmchushihua[9]={3,1,8,7,6,4,0,2,5};//21��
    //int bsmchushihua[9]={1,2,3,8,6,4,7,0,5};//һ������
    //int bsmchushihua[9]={1,2,3,6,0,5,7,8,4};
    //int bsmchushihua[9]={0,2,3,8,1,4,7,6,4};
    //int bsmchushihua[9]={2,0,3,1,8,4,7,6,5};
    //int bsmchushihua[9]={1,3,4,7,8,0,6,5,2};
    //����ʹ������Ĳ�ͬ��ʼ״̬���в���
    for(k=0;k<9;k++)
    {
        bsm[0].ccstate[k]=bsmchushihua[k];
    }
    bsm[0].a=h(bsm[0].ccstate);
    bsm[0].fx=0;
    bsm[0].flag=1;
    bsm[0].pre=-1;
    bsm[0].cengshu=0;
    return bsm[0];

}
void rudui1(bashuma bsm[],bashuma bsm1[],char c)
{   int k;
    ta++;
    for(k=0;k<9;k++)
       {
        bsm1[ta].ccstate[k]=bsm[head].ccstate[k];
       }
    //bsm1[ta].a=h(bsm[head].ccstate)+head;
      bsm1[ta].fx=c;
      bsm1[ta].flag=0;
      if(m==1){zuixiao[m-1]==-1;bsm1[ta].pre=zuixiao[m-1];}
      else
      {
          bsm1[ta].pre=zuixiao[m-1];}
        bsm1[ta].a=h(bsm[head].ccstate)+bsm1[bsm1[ta].pre].cengshu;//bsm1[ta].a=h(bsm[head].ccstate);
    bsm1[ta].cengshu=bsm1[bsm1[ta].pre].cengshu+1;
      /*printf("\n%d",ta);
         for(k=0;k<9;k++)
         {

            if(k%3==0)printf("\n");
             printf("%d  ",bsm1[ta].ccstate[k]);

        }
        printf("\n",ta-1);*/

}
void rudui(bashuma bsm[],bashuma bsm1[],int i)
{   
    head++;
     int k;
    for(k=0;k<9;k++)
       {
        bsm[head].ccstate[k]=bsm1[i].ccstate[k];
       }
      bsm[head].a=bsm1[i].a;
      bsm[head].fx=bsm1[i].fx;
      bsm[head].pre=i;
       /* printf("\n%d",head);
         for(k=0;k<9;k++)
         {

            if(k%3==0)printf("\n");
             printf("%d  ",bsm1[head].ccstate[k]);

        }
        printf("\n",head);*/

}
int min(bashuma bsm[])//�ҵ���С�� 
{

    int smallest;
    int i;
   smallest = 100000;
    for( i =1; i <=ta; i++)
    {   if(bsm[i].flag==1)continue;
        if(smallest > bsm[i].a)
            smallest = bsm[i].a;
    }
    for ( i = 1; i <=ta; i++)
    {   if(bsm[i].flag==1)continue;
            if(smallest ==bsm[i].a)
            return i;
    }

}
int locate(bashuma bsm[])
{   int k;
    for(k=0;k<9;k++)
       {
        if(bsm[head].ccstate[k]==0)
        return k;
       }
}
int bianli1(bashuma bsm[],bashuma bsm1[])
{
    int k,j,l;
    for(k=0;k<=ta;k++)
    {
        l=0;
        for(j=0;j<9;j++)
        {
         if(bsm1[k].ccstate[j]==bsm[head].ccstate[j])
             l++;
        }
        if(l==9)
        return 0;
        else return 1;
    }
}
int pp(bashuma bsm[],bashuma bsm1[])
{   int c,k,j,flag,n,a;
    //���ռ�Ŀ��������ؽ���
    int nxt,now; 
    while(!check(bsm[head].ccstate))
    {
        now=locate(bsm);
        for(int dir=0;dir<4;dir++)//�ո�������ĸ������ƶ�
        {
            if(head==0)
                if(canMove(now,dir,nxt)) 
                {
                bsm[head].ccstate[now]=bsm[head].ccstate[nxt];
                bsm[head].ccstate[nxt]=0;
                rudui1(bsm,bsm1,cdir[dir]);
                //��ԭ���飬������һ������ı任 
                bsm[head].ccstate[nxt]=bsm[head].ccstate[now];
                bsm[head].ccstate[now]=0;

                }
            if((head!=0)&&(bsm[head].fx!=opcdir[dir])&&canMove(now,dir,nxt))
            {
                bsm[head].ccstate[now]=bsm[head].ccstate[nxt];
                bsm[head].ccstate[nxt]=0;
                if(bianli1(bsm,bsm1)){rudui1(bsm,bsm1,cdir[dir]);}
                bsm[head].ccstate[nxt]=bsm[head].ccstate[now];
                bsm[head].ccstate[now]=0;
             }

        }
        zuixiao[m]=min(bsm1);
        a=zuixiao[m];
        bsm1[a].flag=1;
        m++;
            //A*�㷨�ҵ�ֵ��С�Ĳ���  ���head++ ��Ϊ��չ�ڵ㣬�������
        rudui(bsm,bsm1,a);
    }
    if(check(bsm[head].ccstate))
        return 1;
    else return 0;

}
void shuchu(bashuma bsm[],int i)
{   int k;
    if(bsm[i].pre==-1)
        return;
    if(bsm[i].pre!=-1)
        shuchu(bsm,bsm[i].pre);
     printf("\n");
         for(k=0;k<9;k++)
         {

            if(k%3==0)printf("\n");
             printf("%d  ",bsm[i].ccstate[k]);  
        }
         step++;
        printf("\n");   
}

int main()
{
    int i,k,s;
    bashuma bsm[N],bsm1[N];
    bsm[0]=chushihua(bsm);
    for(k=0;k<9;k++)
         {

            if(k%3==0)printf("\n");
             printf("%d  ",bsm[0].ccstate[k]);

        }
    bsm1[0]=chushihua(bsm1);
    i=pp(bsm,bsm1);
    if(i)
        {
        s=bsm[head].pre;
        shuchu(bsm1,s);
        printf("һ����Ҫ��%d������Ŀ��״̬��\n",step+1);
        }
    else 
    printf("���������֮��û���ҵ��⣡\n");
    return 0;

}
