#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<math.h>
struct stu
{
	char name[20];
	int score[20],number;
	double sum,aver;
	struct stu *next;	
};
int m;
struct stu *head,*o,*p;
int input()
{
	int a=1;

	head=(struct stu*)malloc (sizeof(struct stu));  
	memset(head,0,sizeof(head) );
  	if (head==NULL)   
 	{
    printf ("no enough memory!\n");
    return (NULL);
	}  
  	head->next = NULL; 
	o=head;
	printf("��������m����Ŀ");
	scanf("%d",&m);
	while(a)
	{
		p=(struct stu*)malloc (sizeof(struct stu));
		p->next=NULL;
		printf("������ ѧ��  ");
		scanf(" %d",&p->number);
		printf("������ ����  ");
		scanf(" %s",p->name);
		p->sum=0;
		for(int i=1;i<=m;i++)
		{
			printf("������ѧ���ɼ�%d ",i);
			scanf(" %d",&p->score[i]);	
			p->sum+=p->score[i];
		}
		p->aver=p->sum/m;
		while(o->next!=NULL)
		{
			o=o->next;
		}
		o->next=p;
		printf("����¼������1, �˳�����0");
		scanf("%d",&a);
		
	}
		printf("¼�����");
}
void list()
{
	for(o=head->next;o!=NULL;o=o->next)  //��pָ��Ҫ����ĵ�i���ڵ�
    {
		printf("ѧ��:%d ����:%s �ܳɼ�:%.2llf ƽ���ɼ�%.2llf  ",o->number,o->name,o->sum,o->aver);
		for(int i=1;i<=m;i++)
		{
			printf("�ɼ�%d %d",i,o->score[i]);
		}
		printf("\n");
	}  

}
void stu_delete()
{
	int a,b,c;
	int d=1;
	char name[20];
	while(d)
	{
		b=0;
		printf("���������ɾ������1  ��ѧ��ɾ������2");
		scanf("%d",&c);
		if(c==1)
		{
			printf("������������Ҫɾ����Ϣ��ѧ��������");
			scanf("%s",name);
			p=head;
			for(o=head->next;o!=NULL;o=o->next)
			{	
				if(strcmp(o->name,name)==0)
				{
					p->next=o->next;
					printf("����ɹ���\n");
					b=1;
					free(o);
				}
			p=p->next; 
			} 
		} 
		if(c==2)
		{
			printf("������������Ҫɾ����Ϣ��ѧ����ѧ��");
			scanf("%d",&a);
			p=head;
			for(o=head->next;o!=NULL;o=o->next)
			{	
				if(o->number==a)
				{
					p->next=o->next;
					printf("����ɹ���\n");
					b=1;
					free(o);
				}
			
			} 			
		}
		if(b==0)
			printf("�Բ��𣬲��޴���,����������\n");
		printf("\n");
		printf("������������1  �˳�����0");
		scanf("%d",&d);
	} 
}	 
void modify()
{
	int e,a,c,b;
	int d=1;
	char name[20];
	while(d)
	{
		e=0;
		printf("��������������޸�����1  ��ѧ�Ų����޸�����2");
		scanf("%d",&c);
		if(c==1)
		{ 
			printf("������������Ҫ�޸���Ϣ��ѧ��������");
			scanf("%s",name);
			p=head;	
			for(o=head->next;o!=NULL;o=o->next)
			{
				if(strcmp(o->name,name)==0)
				{ 	
					printf("����޸� ��������1���ɼ�����2,ѧ������3");
					scanf("%d",&b);
					if(b==1)
					{
						printf("����ѧ����������");
						scanf("%s",o->name);
						printf("�޸����");
					}
					if(b==2)
					{
						printf("����ѧ���µ�ÿ�Ƴɼ�");
						for(int j=1;j<=m;j++)
						{
							printf("������ѧ���ɼ�%d  ",j);
							scanf("%d",&o->score[j]);
							o->sum+=o->score[j];
						}
						o->aver=o->sum/m;
						printf("�޸����");
					}
					if(b==3)
					{
						printf("����ѧ������ѧ��");
						scanf("%d",&o->number);
						printf("�޸����");
					}			 
					e=1;
				} 
			} 
		}
		if(c==2) 
		{
			printf("��������Ҫ�޸���Ϣ��ѧ����ѧ��");
			scanf("%d",&a);
			p=head;	
			for(o=head->next;o!=NULL;o=o->next)
			{
				if(a==o->number)
				{
					printf("����޸� ��������1���ɼ�����2,ѧ������3");
					scanf("%d",&b);
					if(b==1)
					{
						printf("����ѧ����������");
						scanf("%s",o->name);
						printf("�޸����");
					}
					if(b==2)
					{
						printf("����ѧ���µ�ÿ�Ƴɼ�");
						for(int j=1;j<=m;j++)
						{
							printf("������ѧ���ɼ�%d  ",j);
							scanf("%d",&o->score[j]);
							o->sum+=o->score[j];
						}
						o->aver=o->sum/m;
						printf("�޸����");
					}
					if(b==3)
					{
						printf("����ѧ������ѧ��");
						scanf("%d",&o->number);
						printf("�޸����");
					}			 
					e=1;
				}
			}
		} 
	
		if(e==0)
			printf("�Բ��𣬲��޴���,����������\n");
		printf("\n");
		printf("������������1  �˳�����0");
		scanf("%d",&d);
	}
}
void search()
{
	int a,b,c;
	int d=1;
	char name[20];
	while(d)
	{
		printf("�����������������1  ��ѧ�Ų�������2");
		scanf("%d",&c);
		if(c==1)
		{
			b=0;
			printf("��������Ҫ���ҵ�ѧ��������");
			scanf("%s",name);
			p=head;	
			for(o=head->next;o!=NULL;o=o->next)
			{
				if(strcmp(o->name,name)==0)
				{
					printf("ѧ��:%d ����:%s �ܳɼ�:%.2llf ƽ���ɼ�%.2llf",o->number,o->name,o->sum,o->aver);
					for(int j=1;j<=m;j++)
					{
						printf("�ɼ�%d %d",j,o->score[j]);
						b=1;
					}
				} 
			}
		}
		if(c==2)
		{
			b=0;
			printf("��������Ҫ���ҵ�ѧ����ѧ��");
			scanf("%d",&a);
			p=head;	
			for(o=head->next;o!=NULL;o=o->next)
			{
				if(a==o->number)
				{
					printf("ѧ��:%d ����:%s �ܳɼ�:%.2llf ƽ���ɼ�%.2llf",o->number,o->name,o->sum,o->aver);
					for(int j=1;j<=m;j++)
					{
						printf("�ɼ�%d %d",j,o->score[j]);
						b=1;
					}		
				}
			}
		}
		if(b==0)
			printf("�Բ��𣬲��޴���,����������\n");
		printf("\n");
		printf("������������1  �˳�����0");
		scanf("%d",&d);		
	}
	
}
void Swap(struct stu *a,struct stu *b)
{
    int t;
    t=a->number;
    a->number=b->number;
    b->number=t;
    char nn[25];
    strcpy(nn,a->name);
    strcpy(a->name,b->name);
    strcpy(b->name,nn);
    for(int i=1;i<=m;i++)
    {
        t=a->score[i];
        a->score[i]=b->score[i];
       	b->score[i]=t;
    }
    t=o->sum;
    a->sum=b->sum;
    b->sum=t;
    double tt;
    tt=a->aver;
    a->aver=b->aver;
    b->aver=tt;
}
void sum_sort_des()
{
	for(o=head->next;o!=NULL;o=o->next)
	{
		for(p=o->next;p!=NULL;p=p->next)
		{
			if(o->sum<p->sum)
			Swap(o,p);

		}
	}
	list();
}
void sum_sort_sc()
{
		for(o=head->next;o!=NULL;o=o->next)
	{
		for(p=o->next;p!=NULL;p=p->next)
		{
			if(o->sum>p->sum)
			Swap(o,p);

		}
	}
	list();
}
void num_sort_sc()
{	for(o=head->next;o!=NULL;o=o->next)
	{
		for(p=o->next;p!=NULL;p=p->next)
		{
			if(o->number>p->number)
			Swap(o,p);

		}
	}
	list();
}
void num_sort_des()
{
		for(o=head->next;o!=NULL;o=o->next)
	{
		for(p=o->next;p!=NULL;p=p->next)
		{
			if(o->number<p->number)
			Swap(o,p);

		}
	}
	list();
}
void insert()
{
	int s,a=1; 
	struct stu *h;
	while(a)
	{
	
		h=(struct stu*)malloc (sizeof(struct stu));
		h->next=NULL;
		printf("������ ѧ��  ");
		scanf(" %d",&h->number);
		printf("������ ����  ");
		scanf(" %s",h->name);
		h->sum=0;
		for(int i=1;i<=m;i++)
		{
			printf("������ѧ���ɼ�%d ",i);
			scanf(" %d",&h->score[i]);	
			h->sum+=h->score[i];
		}
		h->aver=h->sum/m;
		printf("�����������ѧ��Ϊn��ͬѧ�ĺ���  ");
		scanf("%d",&s);
		o=head;
		while(o->next!=NULL&&o->number!=s)
		{
			o=o->next;
		}
		if(o->number==s)
		{
			h->next=o->next;
			o->next=h;
			printf("����ɹ�\n");
		}
		else
		{
			printf("�������,�����²���\n"); 
			free(h);
		}
		
		printf("����¼������1, �˳�����0");
		scanf("%d",&a);
	}	
} 
void write()
{
	FILE *fp;
    fp=fopen("computer.txt","w");
    if(!fp)
    {
    	printf("����"); 
		exit(0);
	}
	
	for(o=head->next;o!=NULL;o=o->next) 
	{
		fprintf(fp,"number: %d  ",o->number);
		fprintf(fp,"name: %s ",o->name);
		fprintf(fp,"sorce: ");
			for(int j=1;j<=m;j++)
			{
				fprintf(fp,"%d  %d",o->score[j]);
	
			}	
			fprintf(fp,"sum: %1f  ",o->sum);
			fprintf(fp,"aver: %1f \n",o->aver);
		printf("\n");
		
	}
	fclose(fp);	
    printf("�ļ�д��ɹ�");	
    
    
}
void read()
{
	int c,d=1;
    FILE *fp;
    fp=fopen("computer.txt","w");
    if(!fp)
    {
    	printf("����"); 
		exit(0);
	}
	for(o=head->next;o!=NULL;o=o->next) 
	{
		fscanf(fp,"number: %d  ",&o->number);
		fscanf(fp,"name: %s ",o->name);
		fscanf(fp,"sorce: ");
			for(int j=1;j<=m;j++)
			{
				fscanf(fp,"%d  %d",&o->score[j]);
	
			}	
			fscanf(fp,"sum: %1lf  ",&o->sum);
			fscanf(fp,"aver: %1lf \n",&o->aver);
		printf("\n");
		
	}
	fclose(fp);	
	while(d)
	{
		printf("�����Ҫ��ȡ���˵�����1    ��ȡ���е�����2");
		scanf("%d",&c);


		if(c==1)
		{
			d=1;
			search();
			printf("\n");
			printf("�������� ����1  �˳�����0");
			scanf("%d",&d);
		}
			
		if(c==2)
		{
			for(o=head->next;o!=NULL;o=o->next) 
			{ 
				printf("ѧ��:%d ����:%s �ܳɼ�:%.1f ƽ���ɼ�%.1f",o->number,o->name,o->sum,o->aver);
				for(int l=1;l<=m;l++)
				{ 
					printf("�ɼ�%d %d",l,o->score[l]);
				} 
				printf("\n");

			}
			printf("�������� ����1  �˳�����0");
			scanf("%d",&d);
		}	
	}
}
int main()
{
	while(1)
	{
		char id;
		printf("\nManagemen for Students'score\n");
		printf("1. Append record\n");
		printf("2. List record\n");
		printf("3. Delete record\n");
		printf("4. Modify record\n");
		printf("5. Search record\n");
		printf("6. Sort score in descending order by sum\n");
		printf("7. Sort score in scending order by sum\n");
		printf("8. Sort score in descending order by num\n");
		printf("9. Sort score in scending order by num\n");
		printf("S. Insert a new student and sort by sum\n");
		printf("W. Write to a File\n");
		printf("R. Read from a File\n");
		printf("0. Exit\n");
		scanf("%c",&id);
		switch (id)
		{
			case '1':
				input();
				break;
			case '2':
				list();
				break;
			case '3':
				stu_delete();
				break;
			case '4':
				modify();
				break;
			case '5':
				search();
				break;
			case '6':
				sum_sort_des();
				break;
			case '7':
				sum_sort_sc();
				break;
			case '8':
				num_sort_sc();
				break;
			case '9':	
				num_sort_des();
				break;
			case 'S':
				insert();
				break;
			case 'W':
				write();
				break;
			case 'R':
				read();	
				break;							
			case '0':exit(0);
			default:
				printf("Error input*************\n");
				break;
		}getchar();
	}
return 0;
}
