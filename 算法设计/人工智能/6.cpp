#include <iostream>
#include <iomanip>
#include <string>
using namespace std;
int RULE=30;//动物识别系统的规则库有条规则
int CAUSE=5;//各条规则中的前提条件最多有个
int count=0;//记录所选择的动物特征的个数
int conditionlist[24];//所选择的动物特征
/*事实*/
string fact[] ={ "输入0时结束","有毛发", "有奶", "有羽毛", "会飞", "会下蛋","吃肉", "有犬齿",//1..7
                 "有爪","眼盯前方", "有蹄", "嚼反刍","黄褐色", "身上有暗斑点", "身上有黑色条纹",//8..14  
				 "有长脖子","有长腿", "不会飞", "会游泳", "黑白二色", "善飞", "哺乳动物", "鸟", "食肉动物","蹄类动物",//15..24  
				 "金钱豹", "虎", "长颈鹿", "斑马", "鸵鸟", "企鹅", "信天翁" };//25..31
/*规则*/
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


 /*知识匹配*/
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
            for (j=0;j<count;j++)                     //所选择的动物特征的条数
            {
         	if (rule[a+num][i]==conditionlist[j])//匹配上一个条件flag计数加一
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
/*正向推理*/
int inference()
{
    for (int i=1; i<=RULE;i++)
    {
        if (match(i)==true)
        {
        	cout<<"运用了规则"<<i<<":";
            cout<<fact[rule[i][0]]<<endl;
         	   return 0;
        }
    }
    cout<<"未知动物"<<endl;
    return 0;
}
int main()
{   
	int a=1;
    int k=0;
    cout<<"以下是一些动物的特征:"<<endl;
    for (int i=0;i<=24;i++)
    {
        cout<<i<<".";
        cout<<setiosflags(ios::left)<<"\t"<<setw(14)<<fact[i]<<"  ";//setiosflags(ios::left)左对齐 
        if (i%4==0)
            cout<<endl;
    }
	cout<<"请选择动物的特征:";
	while(a)                      //a为整形的，按任意字母可结束循环
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


