#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define MAX_NODESIZE 30000
#define N 3	//4



//----------------------------------������ṹ��----------------------------------//
typedef struct node
{
	//�ڵ�״̬
	int a[N][N];
	//�ո���±�
	int i_0, j_0;
	//������Ϣ
	int d, w, f;	//������ȣ������Ӳ�����ȷλ�õ���Ŀ���ܴ���
	//ָ�򸸽ڵ�ָ��
	struct node *father;
}node, *p_node;



//----------------------------------˳���ṹ��----------------------------------//
typedef struct list
{
	p_node a[MAX_NODESIZE];
	long length;
}list, *p_list;



//-------------------------------��ʼ�ڵ�&Ŀ��ڵ�--------------------------------//
static int s0[N][N] = { 2,8,3,1,0,4,7,6,5 };	//��ʼ���
//static int s0[N][N] = { 1,2,3,4,6,7,8,0,5,10,11,12,9,13,14,15 };
/*
2	8	3
1	0	4
7	6	5
*/

static int sg[N][N] = { 1,2,3,8,0,4,7,6,5 };	//Ŀ����
//static int sg[N][N] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,0 };
/*
1	2	3
8	0	4
7	6	5
*/


//---------------------------------�ṹ�忪�ٿռ�---------------------------------//
p_node s_0 = (p_node)malloc(sizeof(node));		//��ʼ�ڵ�
p_node s_g = (p_node)malloc(sizeof(node));		//Ŀ��ڵ�
p_list OPEN = (p_list)malloc(sizeof(list));		//OPEN��
p_list CLOSE = (p_list)malloc(sizeof(list));	//CLOSE��



//------------------------------------��������------------------------------------//
int w(p_node s);						//�����������ٴ�����Ϊ�����پ���
int f(p_node s);						//���ۺ���
void init_node();						//��ʼ��
void out_node(p_node s);				//���������
void list_reverse(p_node &p);			//����������
void out_list(p_list &l);				//���OPEN��
bool search_list(p_list &l, p_node s);//�Ա���в��ң��ɹ�����true
void sort_list(p_list &l);				//��OPEN��������򣨰�f��С����
void add_list(p_list &l, p_node s);	//�����㵽OPEN���л�CLOSE����
void copy_node(p_node s1, p_node &s2);//�����µĽ�㣨��s2��ֵ��s1��
void delete_list(p_list &l);			//��OPEN���CLOSE��ɾ�����
bool is_equal(p_node s1, p_node s2);	//�ж����ڵ��Ƿ����
bool up_mov(p_node &s);				//�ո�����
bool down_mov(p_node &s);				//�ո�����
bool left_mov(p_node &s);				//�ո�����
bool right_mov(p_node &s);				//�ո�����
void move(p_node s);					//�ƶ����ڵ㲢����δ̽�����У���չ��㣩
int find_i(int a);
int find_j(int a);



//-------------------------------------������-------------------------------------//
int main()
{
	init_node();

	printf("\n�������������£�\n");
	printf("=================================================================\n\n");

	while (OPEN->length != 0 && CLOSE->length <= 1000)	//���ѭ������1000��
	{
		p_node n = OPEN->a[0];	//��Open��ĵ�һ���ڵ�ȡ������Close�����Ǹýڵ�Ϊn
		delete_list(OPEN);

		if (is_equal(n, s_g))	//����ڵ�n�Ƿ�ΪĿ��ڵ㡣���ǣ����ҵ�������Ľ⣬�ɹ��˳�����ɻ��� if(w(n)==0){...}
		{

			//out_node(n);
			list_reverse(n);
			list_reverse(n);
			list_reverse(n);
			while (n)
			{
				printf("�� %d ����\n", n->d + 1);
				out_node(n);
				n = n->father;
			}

			break;
		}

		add_list(CLOSE, n);
		move(n);	//��չ�ڵ�

		sort_list(OPEN);
		out_list(OPEN);
	}

	if (OPEN->length == 0 || CLOSE->length > 10000)
	{
		printf("\n�ӳ�ʼ����޷�����Ŀ���㣡\n\n");
	}

	return 0;


}



//--------------------------���������������ٴ�Ϊ�����پ���---------------------------//
int w(p_node s)
{
	/*int r = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (s->a[i][j] != sg[i][j])
				r++;
		}
	}
	if (s->a[1][1] == sg[1][1])
		r++;
	return r-1;*/
	int r = 0;
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			r += abs(i - find_i(s->a[i][j])) + abs(j - find_j(s->a[i][j]));
		}
	}
	return r;
}

int f(p_node s)
{
	return (s->d + s->w);
}


//-------------------------------------��ʼ��-------------------------------------//
void init_node()
{
	//------------------------��ʼ����ʼ�ڵ�

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			s_0->a[i][j] = s0[i][j];

			if (s_0->a[i][j] == 0)
			{
				s_0->i_0 = i;
				s_0->j_0 = j;
			}
		}
	}

	s_0->d = 0;
	s_0->w = w(s_0);
	s_0->f = f(s_0);
	s_0->father = NULL;

	//------------------------��ʼ��Ŀ��ڵ�

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			s_g->a[i][j] = sg[i][j];
			if (s_g->a[i][j] == 0)
			{
				s_g->i_0 = i;
				s_g->j_0 = j;
			}
		}
	}

	s_g->d = 0;
	s_g->w = w(s_g);
	s_g->f = f(s_g);

	OPEN->length = 0;
	CLOSE->length = 0;

	add_list(OPEN, s_0);	//��ʼ�ڵ����OPEN����

	printf("��ʼ�ڵ�Ϊ��\n");	//��ӡ��ʼ�ڵ�
	out_node(s_0);

	printf("Ŀ��ڵ�Ϊ��\n");	//��ӡĿ��ڵ�
	out_node(s_g);
}



//---------------------------------���������-------------------------------------//
void out_node(p_node s)
{
	printf("-------------------\n");
	//printf(" x=%d,y=%d\n", s->i_0, s->j_0);
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			printf("%5d", s->a[i][j]);
		}

		printf("\n");
	}
	printf("-------------------");
	printf(" d=%d,w=%d;  f=%d\n\n\n", s->d, s->w, s->f);
}



//---------------------------------����������-------------------------------------//
void list_reverse(p_node & p)
{
	p_node p_pre, p_suc;

	p_pre = NULL;
	p_suc = p->father;

	while (p)
	{
		p->father = p_pre;
		p_pre = p;
		if (p_suc == NULL)
			break;
		p = p_suc;
		p_suc = p_suc->father;
	}
}



//---------------------------------���OPEN��-------------------------------------//
void out_list(p_list &l)
{
	printf("****************************************************************\n");
	for (int i = 0; i < l->length; i++)
	{
		out_node(l->a[i]);
	}
	printf("****************************************************************\n");
}


//-------------------------�Ա���в��ң��ɹ�����true-----------------------------//
bool search_list(p_list &l, p_node s)
{
	for (int i = 0; i < l->length; i++)
	{
		if (is_equal(l->a[i], s))
			return true;
	}

	return false;
}



//-------------------��OPEN��������򣨰�f��С����(���룩-----------------------//
void sort_list(p_list &l)
{
	p_node temp = (p_node)malloc(sizeof(node));

	for (int i = 1; i < l->length; i++)
	{
		int j = i - 1;
		copy_node(temp, l->a[i]);
		while (j >= 0 && (temp->f < l->a[j]->f))
		{
			copy_node(l->a[j + 1], l->a[j]);//l->a[j + 1] = l->a[j];
			j--;
		}
		copy_node(l->a[j + 1], temp);
	}
}


//------------------------�����㵽OPEN���л�CLOSE����---------------------------//
void add_list(p_list & l, p_node s)
{
	l->a[l->length++] = s;
}



//-------------------------�����µĽ�㣨��s2��ֵ��s1��---------------------------//
void copy_node(p_node s1, p_node &s2)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			s1->a[i][j] = s2->a[i][j];
		}
	}

	s1->i_0 = s2->i_0;
	s1->j_0 = s2->j_0;

	s1->d = s2->d;
	s1->w = s2->w;
	s1->f = s2->f;
	s1->father = s2->father;
}



//--------------------------��OPEN���CLOSE��ɾ�����-----------------------------//
void delete_list(p_list & l)
{
	for (int i = 0; i < l->length; i++)
	{
		l->a[i] = l->a[i + 1];
	}
	l->length--;
}



//----------------------------�ж����ڵ��Ƿ����----------------------------------//
bool is_equal(p_node s1, p_node s2)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (s1->a[i][j] != s2->a[i][j])
			{
				return false;
			}
		}
	}

	return true;
}



//----------------------------------�ո�����--------------------------------------//
bool left_mov(p_node &s)
{
	if (s->j_0 == 0)
		return false;
	int temp;
	temp = s->a[s->i_0][s->j_0];
	s->a[s->i_0][s->j_0] = s->a[s->i_0][s->j_0 - 1];
	s->a[s->i_0][s->j_0 - 1] = temp;

	s->j_0--;
	return true;
}



//----------------------------------�ո�����--------------------------------------//
bool right_mov(p_node &s)
{
	if (s->j_0 == N - 1)
		return false;

	int temp;
	temp = s->a[s->i_0][s->j_0];
	s->a[s->i_0][s->j_0] = s->a[s->i_0][s->j_0 + 1];
	s->a[s->i_0][s->j_0 + 1] = temp;

	s->j_0++;
	return true;
}



//----------------------------------�ո�����--------------------------------------//
bool up_mov(p_node &s)
{
	if (s->i_0 == 0)
		return false;

	int temp;
	temp = s->a[s->i_0][s->j_0];
	s->a[s->i_0][s->j_0] = s->a[s->i_0 - 1][s->j_0];
	s->a[s->i_0 - 1][s->j_0] = temp;

	s->i_0--;
	return true;
}



//----------------------------------�ո�����--------------------------------------//
bool down_mov(p_node &s)
{
	if (s->i_0 == N - 1)
		return false;

	int temp;
	temp = s->a[s->i_0][s->j_0];
	s->a[s->i_0][s->j_0] = s->a[s->i_0 + 1][s->j_0];
	s->a[s->i_0 + 1][s->j_0] = temp;

	s->i_0++;
	return true;
}



//----------------------�ƶ����ڵ㲢����δ̽�����У���չ��㣩--------------------//
void move(p_node s)
{
	p_node p1 = (p_node)malloc(sizeof(node));
	p_node p2 = (p_node)malloc(sizeof(node));
	p_node p3 = (p_node)malloc(sizeof(node));
	p_node p4 = (p_node)malloc(sizeof(node));

	copy_node(p1, s);
	copy_node(p2, s);
	copy_node(p3, s);
	copy_node(p4, s);

	p1->father = s;
	p2->father = s;
	p3->father = s;
	p4->father = s;

	//����ܹ��ƶ�����CLOSE���в����ڣ������OPEN����
	if (left_mov(p1) && !is_equal(p1, p1->father) && !search_list(CLOSE, p1) && !search_list(OPEN, p1))
	{
		add_list(OPEN, p1);
		p1->d++;
		p1->w = w(p1);
		p1->f = f(p1);
	}
	else
		free(p1);

	if (right_mov(p2) && !is_equal(p2, p2->father) && !search_list(CLOSE, p2) && !search_list(OPEN, p2))
	{
		add_list(OPEN, p2);
		p2->d++;
		p2->w = w(p2);
		p2->f = f(p2);
	}
	else
		free(p2);

	if (up_mov(p3) && !is_equal(p3, p3->father) && !search_list(CLOSE, p3) && !search_list(OPEN, p3))
	{
		add_list(OPEN, p3);
		p3->d++;
		p3->w = w(p3);
		p3->f = f(p3);
	}
	else
		free(p3);

	if (down_mov(p4) && !is_equal(p4, p4->father) && !search_list(CLOSE, p4) && !search_list(OPEN, p4))
	{
		add_list(OPEN, p4);
		p4->d++;
		p4->w = w(p4);
		p4->f = f(p4);
	}
	else
		free(p4);
}

int find_i(int a)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (sg[i][j] == a)
				return i;
		}
	}
}

int find_j(int a)
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (sg[i][j] == a)
				return j;
		}
	}
}


