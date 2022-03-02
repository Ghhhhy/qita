#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct HNode
	{
		char roomN[20];	//�ͷ�����
		float Price;	//��׼�۸�
		float PriceL;	//��ס�۸�(Ĭ��ֵ=��׼�۸�*80%)
		int Beds;		//��λ��Beds
		char State[5];	//��ס״̬(ֵ��"����"��"��ס"��"Ԥ��"��Ĭ��ֵΪ"����")
		struct HNode*next;	//ָ����
	}Hotel, *HLink;

HLink p,H,rear,L;
int beds,k;
char *state;
float MaxPriceL;

void Build(HLink &H){
    FILE *fp;   //�ļ�ָ��
    fp=fopen("Hotel.txt","r");    //���ı��ļ�
    if(!fp){
        printf("���������ļ�û�ҵ�!\n");
        exit(1);
    }
    H=(HLink)malloc(sizeof(Hotel));
    rear=H;
    //���δ��ı��ļ��ж�ȡ������ݣ�ѧ�š������ͳɼ�)
    while(!feof(fp)){   //�ж��Ƿ��ȡ���ļ���β
        p=(HLink)malloc(sizeof(Hotel));
        //���ļ������ж�ȡ�ͷ�����p->roomN����׼�۸�p->price����ס�۸�p->PriceL����λ��p->Beds����ס״̬p->State
        fscanf(fp,"%s%f%d",&p->roomN,&p->Price,&p->Beds);
        p->PriceL=p->Price*0.8f; 
        strcpy(p->State,"����");
        rear->next=p;
        rear=p;
    }
    rear->next=NULL;
    fclose(fp);
}
void Exp(HLink &H){
    printf("\n�ͷ���Ϣ������£�\n");
    printf("----------------------------------------------------------------\n");
    printf("�ͷ�����\t��׼�۸�\t��ס�۸�\t��λ��\t��ס״̬\n");
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
        if(strcmp(p->State,"����")==0)    
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
    printf("\n���������е����ڼ�������Ƶ���һ�����λ��: ");
    scanf("%d",&k);
    HLink p1,p2;
    for(p=H;k>0;k--)
        p=p->next;
    if(k>0||!p){
        printf("����k�����ڵ���\n");
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
    printf("\n������������м�λ�ý��֮���ȫ����㵹��: \n");
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
	printf("����ס�۸�����");				
	for(p1 = H->next;p1->next != NULL; p1 = p1->next){
		for(p2 = p1->next; p2 != NULL; p2 = p2->next){
			if(p1->PriceL >p2->PriceL)
			Swap(p1,p2);
			}		
	}
	Exp(H);
	printf("����ס�ͷ�����");
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
    printf("����ͷ�����,��׼�۸�,��ס�۸�,��λ��,��ס״̬"); 
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
    Build(H);//��������
	Exp(H);
	while(1)
	{
		printf("��������Ҫ�ҵ���������:");
		scanf("%s",roomN);
		num=Find(H,roomN);
		if(num!=0){
			printf("����������ĵ�%d��λ��\n",num);
			break;
		}	
		else 
			printf("�ÿͷ�������\n");	
	} 
    printf("������Ҫ�޸ĵĴ�λ��:");
    scanf("%d", &beds);
    char Sstate[5];
    printf("�������µ���ס״̬: ");
    scanf("%s", Sstate);
    state = Sstate;
    updateH(H, beds, state);    //����ס״̬��Ϊstate
    Add(H);     //δ��ס�Ŀͷ���ס�۸���Ӽ�20%
    Exp(H);
	printf("\n");
	printf("��ߵķ��۷�����");
    FirstH(H);
    Exp(H);
    MoveK1(H, k);   //���������е�����k������Ƶ���һ�����λ��
    Exp(H);
    ReverseN2(H);   //������������м�λ�ý��֮���ȫ����㵹��
    Exp(H);
    SortPriceL(H);
    //Exp(H);
    upBed(H, beds);     //��λ��������beds�Ľ�㶼���ڴ�λ������beds�Ľ�����
    printf("������ס״̬����ס�۸��Լ���Ϣ�����\n");//����ʱ�������ֵ����Ϣ��ɾ��
    Exp(H);



    return 0;
}
