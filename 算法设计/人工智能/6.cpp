#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
int RULE=30;//����ʶ��ϵͳ�Ĺ������������
int CAUSE=5;//���������е�ǰ����������и�
int count=0;//��¼��ѡ��Ķ��������ĸ���
int conditionlist[24];//��ѡ��Ķ�������
/*��ʵ*/
string fact[] ={ "����0ʱ����","��ë��", "����", "����ë", "���", "���µ�","����", "��Ȯ��",//1..7
                 "��צ","�۶�ǰ��", "����", "����ۻ","�ƺ�ɫ", "�����а��ߵ�", "�����к�ɫ����",//8..14  
				 "�г�����","�г���", "�����", "����Ӿ", "�ڰ׶�ɫ", "�Ʒ�", "���鶯��", "��", "ʳ�⶯��","���ද��",//15..24  
				 "��Ǯ��", "��", "����¹", "����", "����", "���", "������" };//25..31
/*����*/
int rule[][6]={{ 0, 0, 0, 0, 0, 0 },
			   {21, 1, 0, 0, 0, 0 },//1
			   {21, 2, 0, 0, 0, 0 },//2
			   {22, 3, 0, 0, 0, 0 },//3 
			   {22, 4, 5, 0, 0, 0 },//4 
			   {23, 6, 0, 0, 0, 0 },//5 
			   {23, 7, 8, 9, 0, 0 },//6
			   {24, 21, 10, 0, 0, 0 },//7 
			   {24, 21, 11, 0, 0, 0 },//8 
			   {25, 21, 23, 12, 13, 0 },//9 
			   {26, 21, 23, 12, 14, 0 },//10 
			   {27, 24, 15, 16, 13, 0 },//11 
			   {28, 24, 14, 0, 0, 0 },//12 
			   {29, 22, 15, 16, 17, 19 },//13 
			   {30, 22, 18, 17, 19, 0 },//14 
			   {31, 22, 20, 0, 0, 0 }};//15


 /*֪ʶƥ��*/
bool match(int a)
{
    int i=1;
    int j=0;
    int flag=0;
    int flag1=0;
    int num=0;
    while ((flag==i-1)&&(flag1!=count))
    {   
        flag=0;
        for (i=1;i<=CAUSE;i++)
        {
            if (rule[a+num][i]==0)
                break;                              
            for (j=0;j<count;j++)                     //��ѡ��Ķ�������������
            {
         	if (rule[a+num][i]==conditionlist[j])//ƥ����һ������flag������һ
                {
                    flag++;
                    flag1++;        
                    break;
                }
            }
        }
        num++;
    }
    if ((flag!=i-1)||(count==0))
        return false;  
    else
    {   
        for (i=0;i<num;i++)
        {
            for (j=0;j<num;j++)
            {
                if ((i!=j)&&(rule[a+i][0]!=rule[a+j][0]))
                    return false;
            }
  		  }
        return true;
    }
}
/*��������*/
int inference()
{
    for (int i=1; i<=RULE;i++)
    {
        if (match(i)==true)
        {
        	cout<<"�����˹���"<<i<<":";
            cout<<fact[rule[i][0]]<<endl;
         	   return 0;
        }
    }
    cout<<"δ֪����"<<endl;
    return 0;
}
int main()
{   
	int a=1;
    int k=0;
    cout<<"������һЩ���������:"<<endl;
    for (int i=0;i<=24;i++)
    {
        cout<<i<<".";
        cout<<setiosflags(ios::left)<<"\t"<<setw(14)<<fact[i]<<"  ";//setiosflags(ios::left)����� 
        if (i%4==0)
            cout<<endl;
    }
	cout<<"��ѡ���������:";
	while(a)                      //aΪ���εģ���������ĸ�ɽ���ѭ��
    {
        cin>>a;
	    if(a==0)
	   		break;
        conditionlist[k]=a;                   
        k++;
        count++;
    }
    
    inference();
	return 0;   
}


