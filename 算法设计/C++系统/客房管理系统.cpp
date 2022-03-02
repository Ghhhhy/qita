#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct HNode
	{
		char roomN[20];	//客房名称
		float Price;	//标准价格
		float PriceL;	//入住价格(默认值=标准价格*80%)
		int Beds;		//床位数Beds
		char State[5];	//入住状态(值域："空闲"、"入住"、"预订"，默认值为"空闲")
		struct HNode*next;	//指针域
	}Hotel, *HLink;

HLink p,H,rear,L;
int beds,k;
char *state;
float MaxPriceL;

void Build(HLink &H){
    FILE *fp;   //文件指针
    fp=fopen("Hotel.txt","r");    //打开文本文件
    if(!fp){
        printf("数据输入文件没找到!\n");
        exit(1);
    }
    H=(HLink)malloc(sizeof(Hotel));
    rear=H;
    //依次从文本文件中读取结点数据（学号、姓名和成绩)
    while(!feof(fp)){   //判断是否读取到文件结尾
        p=(HLink)malloc(sizeof(Hotel));
        //从文件中逐行读取客房名称p->roomN、标准价格p->price、入住价格p->PriceL、床位数p->Beds和入住状态p->State
        fscanf(fp,"%s%f%d",&p->roomN,&p->Price,&p->Beds);
        p->PriceL=p->Price*0.8f; 
        strcpy(p->State,"空闲");
        rear->next=p;
        rear=p;
    }
    rear->next=NULL;
    fclose(fp);
}
void Exp(HLink &H){
    printf("\n客房信息输出如下：\n");
    printf("----------------------------------------------------------------\n");
    printf("客房名称\t标准价格\t入住价格\t床位数\t入住状态\n");
    printf("----------------------------------------------------------------\n");
    for(p=H->next;p!=NULL;p=p->next)
        printf("%s\t%8.1f\t%8.1f\t%6d%8s\n",p->roomN,p->Price,p->PriceL,p->Beds,p->State);
}
int Find(HLink &H,char *roomN){
	int i=1;
	p=H->next;
	while(p!=NULL&&strcmp(p->roomN,roomN)!=0){
		p=p->next;
		i++;
	}
	if(p!=NULL)
		return i;
	else
		return 0;
} 
void updateH(HLink &H,int beds,char *state){
    for(p=H->next;p!=NULL;p=p->next)
        if(p->Beds==beds)
            strcpy(p->State,state);
}
void Add(HLink &H){
    for(p=H->next;p!=NULL;p=p->next){
        if(strcmp(p->State,"空闲")==0)    
			p->PriceL=p->PriceL*1.2f;

	} 
}
HLink FirstH(HLink &H){
	
    HLink pmax,q1,q2;
    pmax=H->next;
    p=pmax->next;
    q1=H->next;
    while(p!=NULL){
        if(p->PriceL > pmax->PriceL){
            q2=q1;
			pmax=p;
			p=p->next;
			q1=q1->next;
        }
        else{
			p=p->next;
			q1=q1->next;	
		}
        	
    }
    printf("%s\t%8.1f\t%8.1f\t%6d%8s\n",pmax->roomN,pmax->Price,pmax->PriceL,pmax->Beds,pmax->State);
	q2->next=pmax->next;
	return 0;
}
void MoveK1(HLink &H, int k){
it:
    printf("\n将单链表中倒数第几个结点移到第一个结点位置: ");
    scanf("%d",&k);
    HLink p1,p2;
    for(p=H;k>0;k--)
        p=p->next;
    if(k>0||!p){
        printf("输入k超过节点数\n");
        goto it;
    }
    for(p2=H;p;p2=p2->next){
        p=p->next;
        p1=p2;
    }
    p1->next=p2->next;
    p2->next=H->next;
    H->next=p2;
}
void ReverseN2(HLink &H){
    printf("\n将单链表的正中间位置结点之后的全部结点倒置: \n");
    HLink q,p1,temp;
    q=p1=H;
    for(p=H;q&&q->next;p=p->next){
        q=q->next->next;
    }
    temp=p;
    for(q=H;q->next;q=q->next);
    for(;temp->next!=q;){
        p=temp;
        for(p1=p->next;q->next!=p1;p=p->next){
            p->next=p1->next;
            p1->next=p1->next->next;
            p->next->next=p1;
        }
    }
}
void Swap(struct HNode*a,struct  HNode*b)
{
    float t;
    t=a->Price;
    a->Price=b->Price;
    b->Price=t;
    char nn[20];
    strcpy(nn,a->roomN);
    strcpy(a->roomN,b->roomN);
    strcpy(b->roomN,nn);
    char hh[5];
    strcpy(hh,a->State);
    strcpy(a->State,b->State);
    strcpy(b->State,hh);
    float tt;
    tt=a->PriceL;
    a->PriceL=b->PriceL;
    b->PriceL=tt;
    int ttt;
    ttt=a->Beds;
    a->Beds=b->Beds;
    b->Beds=ttt;
}
void SortPriceL(HLink &H)
{
	HLink p1 = H->next;
	HLink p2 = NULL;
	if(p1 == NULL)
		return;
	int v = 0;
	printf("按入住价格排列");				
	for(p1 = H->next;p1->next != NULL; p1 = p1->next){
		for(p2 = p1->next; p2 != NULL; p2 = p2->next){
			if(p1->PriceL >p2->PriceL)
			Swap(p1,p2);
			}		
	}
	Exp(H);
	printf("按入住客房名称");
		for(p1 = H->next;p1->next != NULL; p1 = p1->next){
			for(p2 = p1->next; p2 != NULL; p2 = p2->next){
				if(strcmp(p1->roomN,p2->roomN)>0)
				Swap(p1,p2);
			}		
	}
	Exp(H);
}
void upBed(HLink &H,int beds){
  
    HLink p1,p2,p3,q;
    p=H->next;
	q=new HNode;
    H->next=q;
    q->next=NULL;
	p2=H;
	p3=q;
    printf("输入客房名称,标准价格,入住价格,床位数,入住状态"); 
    scanf("%s%f%f%d%s",&q->roomN,&q->Price,&q->PriceL,&q->Beds,&q->State);
    while(p!=NULL){	
		p1=p;
		if(p1->Beds>q->Beds){ 
			p2->next=p1;
			p=p->next;
			p1->next=q;
			p2=p1;
		}
		if(p1->Beds<=q->Beds){
			p3->next=p1;
			p=p->next;
			p1->next=NULL;
			p3=p1; 
			
		}
		
        
	}  
    
}
int main(){
	int num;
	char roomN[20];
	Hotel* H;
    Build(H);//创建链表
	Exp(H);
	while(1)
	{
		printf("输入你想要找到房间名称:");
		scanf("%s",roomN);
		num=Find(H,roomN);
		if(num!=0){
			printf("房间在链表的第%d个位置\n",num);
			break;
		}	
		else 
			printf("该客房不存在\n");	
	} 
    printf("请输入要修改的床位数:");
    scanf("%d", &beds);
    char Sstate[5];
    printf("请输入新的入住状态: ");
    scanf("%s", Sstate);
    state = Sstate;
    updateH(H, beds, state);    //将入住状态改为state
    Add(H);     //未入住的客房入住价格均加价20%
    Exp(H);
	printf("\n");
	printf("最高的房价房间是");
    FirstH(H);
    Exp(H);
    MoveK1(H, k);   //将单链表中倒数第k个结点移到第一个结点位置
    Exp(H);
    ReverseN2(H);   //将单链表的正中间位置结点之后的全部结点倒置
    Exp(H);
    SortPriceL(H);
    //Exp(H);
    upBed(H, beds);     //床位数不超过beds的结点都放在床位数超过beds的结点后面
    printf("更改入住状态、入住价格以及信息整理后：\n");//将此时所有最大值的信息都删除
    Exp(H);



    return 0;
}
