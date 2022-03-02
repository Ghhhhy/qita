#include<iostream>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
using namespace std;

string key[6]={"main","int","if","else","while","for"}; //关键字 
int keyNum[6]={1,2,3,4,5,6}; //关键字的种别码

string symbol[21]={"+","-","*","(",")","{","}",",",";",":",">","<","!","/","=","|","&","!=",">=","<=","=="};//运算符和界符 
int symbolNum[21]={7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27};//运算符和界符的种别码 

string letter[1000];//存放文件取出的字符 
string NMU[1000];//将字符转换为单词
string biaoji[1];//存符号 
FILE *fp1,*fp2;
int length; //保存程序中字符的数目 
int num;
int call=0;
int e=0;
int row=1;
int isSymbol(string s){ //判断运算符和界符 
	int i;
	for(i=0;i<21;i++){
		if(s==symbol[i])
			 return symbolNum[i]; 
	}
	return 0;
} 
 
//判断是否为数字 
bool isNum(string s){
	if(s>="0" && s<="9")
		return true;
	return false;
}
 
//判断是否为字母 
bool isLetter(string s){
	if(s>="a" && s<="z"||s>="A" && s<="Z"||s=="_")
		return true;
	return false;
}
 
//判断是否为关键字,是返回种别码 
int isKeyWord(string s){
	int i;
	for(i=0;i<6;i++){
		if(s==key[i])
			return keyNum[i];
	}
	return 0;
}
 
//返回单个字符的类型 
int typeword(string str){
	if(str>="a" && str<="z"||str>="A" && str<="Z"||str=="_") // 字母 
		return 1;
	else if(str>="0" && str<="9") //数字 
  		return 2;
	else if(str==">"||str=="="||str=="<"||str=="!"||str==","||str==";"||str=="("||str=="|"||str=="&"
	   ||str==")"||str=="{"||str=="}"||str=="+"||str=="-"||str=="*"||str=="/"||str==":") //判断运算符和界符 
  		return 3;
  	else if(str[0]=='\n')
  		return 5;
	else
		return 4;  
}
 
string identifier(string s,int n){
 	int j=n+1;
 	int flag=1;
	while(flag){
  		if(isNum(letter[j])||isLetter(letter[j])){
   			s=(s+letter[j]).c_str();
  	 		if(isKeyWord(s)){
    			j++;
    			num=j;
    			return s;
   			}
   			j++;
  		}
  		else{
   			flag=0;
  		}
	} 
	num=j;
 	return s;
}
 
string symbolStr(string s,int n){
 	int j=n+1;
 	string str=letter[j];
 	if(letter[j]=="/"){
	    j++;
 		if(letter[j]=="*"){
 			while(letter[j]!="/"){
				j++;
			}
			j++;
			call=1;	
		}
		else
			j--;	
 	}
	else if(letter[j]=="|"){
 		if(letter[j]!=letter[j+1]){
			j++;
 			call=2;
		}	
 		else
			j++; 
 	}	
 	else if(str==">"||str=="="||str=="<"||str=="!") {
	  	s=(s+letter[j]).c_str();//c_str()返回的是一个临时指针
		j++;
 	}	
 	num=j;
 	return s;
}
 
string Number(string s,int n){
 	int j=n+1;
 	int flag=1;
 	int a;
 	while(flag){
 		a=1;
  		if(isNum(letter[j])){
   			s=(s+letter[j]).c_str();
   			j++;
  		}
  		else if(isLetter(letter[j])){
  			while(isLetter(letter[j])){
  				s=(s+letter[j]).c_str();
  				j++;  		  	
			}
			a=0;     
   			if(isNum(letter[j])) 
				flag=1;
			else
				flag=0;
  		}
  		else
  			flag=0;
 	}
 	if(a==0){
	 	e++;
	 	s=" ";
		cout<<"第"<<row<<"排的ID不是标准的ID"<<endl;
 	}
		
	num=j;
 	return s;
}
string Illegal(string s,int n){
	int j=n+1;
 	string str=letter[j];	
 	num=j;
 	return s;
 	
}
void print(string s,int n){
 	char c[20];
	strcpy(c,s.c_str());
 	if(n==0)
		fprintf(fp2,"ID\t\t%s\t%d\n",c,row);
	if(n>0&&n<7)
		fprintf(fp2,"%s\t\t%s\t%d\n",c,c,row);
	if(n>6&&n<21)
		fprintf(fp2,"singleword\t%s\t%d\n",c,row);
	if(n==21||n>23&&n<28)
		fprintf(fp2,"doubleword\t%s\t%d\n",c,row);
	if(n==22||n==23)
		fprintf(fp2,"doubleword\t%s%s\t%d\n",c,c,row);
	if(n==28)
		fprintf(fp2,"NUM\t\t%s\t%d\n",c,row);
}

int TESTscan(){
	char w;
	int i,j;	 
 	fp1=fopen("open.txt","r");
 	fp2=fopen("close.txt","w");
 	length=0;
	w=getc(fp1);
	while(w!=EOF){ 
		if(w!=' '){
			letter[length]=w;
	   		length++;	  
	  	} 
	  	w=getc(fp1);  //去掉程序中的空格
	}
 	int k;
 	for(num=0;num<length;){
	  	string str1,str;
	  	str=letter[num];
	  	k=typeword(str);
	  	switch(k){
		   	case 1:
		    {
			    str1=identifier(str,num);
			    if(isKeyWord(str1))
			    	print(str1,isKeyWord(str1));
		    	else
		    		print(str1,0);
		    break;
		    }
		 	case 2:
		    {
				str1=Number(str,num);
			    if(str1==" ")
			    	break;
			    else{
					print(str1,28);
				    break;	
			    }
		    }
		 	case 3:
		    {
			    str1=symbolStr(str,num);
			   	if(call==2)
			    	e++;
			    call=0;
			     print(str1,isSymbol(str1));
			   /* if(call==1)
			    	cout<<"注释"<<endl;*/
			    if(call==2){
					e++;
					//fprintf(fp2,"非法字符1");
					cout<<"第"<<row<<"排有非法字符1"<<endl; 
				}
			    	
			    call=0; 
			    break; 
		    }
		    case 5:
			{
				str1=Illegal(str,num);
				row++;
				//fprintf(fp2,"row\t\t%d\n",row);
				break;
			}
		    case 4:
			{
		    	str1=Illegal(str,num);
		    	e++;
		    	//fprintf(fp2,"非法字符2");
		    	//cout<<"第"<<row<<"排有非法字符2"<<endl; 
				break;
			}	
		
	  	} 
	} 
	fprintf(fp2,"#\t\t#\t#\n");
	fclose(fp1);//关闭文件  
	fclose(fp2);
	return e;
}

/*
TESTscan();将文件letter[]中每个字符进行提取，找出关键字，输出种别码
int isSymbol（）判断运算符和界符，并返回种别码
bool isNum（） 判断是否为数字
bool isLetter（）判断是否为字母
int isKeyWord（）判断是否为关键字,是返回种别码
int typeword（）返回单个字符的类型
string identifier（）标识符的连接
string symbolStr（）符号和界符的连接
string Number（）数字的连接
void print（）输出
*/
