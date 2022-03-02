#include<stdio.h> 
#include<iostream>
#include<vector>
#include<time.h>
#include<algorithm>
using namespace std;

const int GRID = 3;			//Grid��ʾ��������(����)������3*3�ľŹ���
int rightPos[9] = { 4, 0, 1, 2, 5, 8, 7, 6, 3 };	
//Ŀ��״̬ʱ����p[i][j]=OMG,��ô3*i+j = rightPos[OMG]
struct state{
	int panel[GRID][GRID];
	int level;		//��¼���
	int h;
	state * parent;
	state(int level) :level(level){}
	bool operator == (state & q){	
//�ж�����״̬�Ƿ���ȫ��ȣ���Ӧλ��Ԫ����ȣ�����ȫ��ȷ���true,���򷵻�false
		for (int i = 0; i<GRID; i++){
			for (int j = 0; j<GRID; j++){
				if (panel[i][j] != q.panel[i][j])
					return false;
			}
		}
		return true;
	}
	state & operator = (state p){	//��״̬pΪ��ǰ״̬��ֵ����Ӧλ��Ԫ����ͬ
		for (int i = 0; i<GRID; i++){
			for (int j = 0; j<GRID; j++){
				panel[i][j] = p.panel[i][j];
			}
		}
		return *this;
	}
};
void dump_panel(state * p){			//�������밴3*3������ʽ���
	for (int i = 0; i<GRID; i++){
		for (int j = 0; j<GRID; j++)
			cout << p->panel[i][j] << " ";
		cout << endl;
	}
}
int countH(state st){				//����״̬st����������hֵ��
	int h = 0;
	for (int i = 0; i<GRID; i++){
		for (int j = 0; j<GRID; j++){
			if (st.panel[i][j] != 0)
				h += abs(rightPos[st.panel[i][j]] / GRID - i) +
abs(rightPos[st.panel[i][j]] % GRID - j);		
		}
	}
	return h;
}
int findZero(state & st){				//�ҵ���ֵԪ�أ��������ڱ��е�λ��
	for (int i = 0; i<GRID; i++){
		for (int j = 0; j<GRID; j++){
			if (st.panel[i][j] == 0)
				return i * 3 + j;
		}
	}
}
int f(state * p){				//���㲢����f()ֵ����hֵ+level
	return countH(*p) + p->level;
}
bool compare_state(state * p, state * q){		//�Ƚ�����״̬��fֵ
	return f(p)  >  f(q);
}
vector<state *> open_table;			//open��
vector<state *> close_table;		//close��
vector<state*>::iterator look_up_dup(vector<state*> & vec, state * p){
	vector<state*>::iterator it_r = vec.begin();
	for (; it_r<vec.end(); it_r++){
		if ((*(*it_r)) == *p){
			break;
		}
	}
	return it_r;
}
state * search(state & start){		//A*�㷨��������
	int level = 0;
	open_table.push_back(&start);
	int count = 0;
	while (!open_table.empty()){
		sort(open_table.begin(), open_table.end(), compare_state);
		//��open���е�Ԫ�ؽ�������
		state * p = open_table.back();
		open_table.pop_back();
		if (countH(*p) == 0)
			return p;			//���н��Ƶ���Ŀ��λ�ã��������̽���
		level = p->level + 1;
		int zeroPos = findZero(*p);
		int x = zeroPos / 3;		//�ո�����±�
		int y = zeroPos % 3;		//�ո�����±�
		for (int i = 0; i<4; i++){	//���������ĸ�����
			int x_offset = 0, y_offset = 0;
			switch (i){
			case 0:x_offset = 0, y_offset = 1; break;	//��
			case 1:x_offset = 0, y_offset = -1; break;//��
			case 2:x_offset = 1, y_offset = 0; break;//��
			case 3:x_offset = -1, y_offset = 0; break;//��
			};
			if (x + x_offset<0 || x + x_offset >= GRID || y + y_offset<0 || y + y_offset >= GRID){
				continue;		
//���ƶ�һ���󣬽�������/��/��/�ұ߽磬��������򲻿��ߣ�������һ������
			}
			state * q = new state(level);		//���������ߣ���չ��һ���ڵ�
			q->parent = p;
			*q = *p;
			q->panel[x][y] = q->panel[x + x_offset][y + y_offset];
			q->panel[x + x_offset][y + y_offset] = 0;//�ո������������һ��
			bool skip = false;
			vector<state *>::iterator dup = look_up_dup(open_table, q);
			//��q����open���У����open���е���Ϣ���и���
			if (dup != open_table.end()){
				if (f(q) < f(*dup)){
					(*dup)->level = q->level;
					(*dup)->parent = q->parent;
				}
				skip = true;
			}
			dup = look_up_dup(close_table, q);
			if (dup != close_table.end()){	//��q����close���У���fֵ��ԭֵС��
				if (f(q) < f(*dup)){			//��q��close�����������open��
					delete *dup;
					close_table.erase(dup);
					open_table.push_back(q);
					skip = true;
				}
			}
			if (!skip){
				open_table.push_back(q);
			}
		}
		close_table.push_back(p);
	}
}
void dump_solution(state *q)		//�����·��
{
	vector<state *> trace;
	while (q){
		trace.push_back(q);
		q = q->parent;
	}
	int count = 0;
	while (!trace.empty()){
		cout << "��" << count << "��\n";
		dump_panel(trace.back());
		cout << "h: " << countH(*trace.back()) <<"\tg:"<<count<< "\tf: " << f(trace.back()) << endl << endl;
		trace.pop_back();
		count++;
	}
}
int main()
{
	state p(0);
	state *q;
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			cout << "������" << i+1 << "�У�" << j+1 << "��ֵ:"; 
			cin >> p.panel[i][j];
		}
	}
	p.parent = NULL;
	q = search(p);
	dump_solution(q);
	system("pause");
}

