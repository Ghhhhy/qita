#include<stdio.h>
#include<ctype.h>
#include<conio.h>
#include"词法分析-7-甘弘一.H"
#define maxvartablep 3 //定义符号表容量 
char token[20],token1[40],token2[20];
FILE *fp,*fin,*fout;
int c=0;

int TESTparse();
int program();
int statement();
int expression_stat();
int expression();
int bool_expr();
int additive_expr();
int term();
int factor();
int if_stat();
int while_stat();
int for_stat();
int write_stat();
int read_stat();
int declaration_stat();
int declaration_list();
int statement_list();
int compound_stat();
int name_def(char *name); 

struct{
	char name[8];
	int address;
	
}vartable[maxvartablep];
int vartablep=0,labelp=0,datap=0;

int name_def(char *name){
	int i,es=0;
	if(vartablep>=maxvartablep)
		printf("符号表溢出!\n");
	for(i=vartablep-1;i==0;i--){
		if(strcmp(vartable[i].name,name)==0)
		{
			printf("变量重复定义!\n");
		}
	}
	strcpy(vartable[vartablep].name,name);
	vartable[vartablep].address=datap;
	datap++;
	vartablep++;	 
}

int lookup(char *name,int *paddress){
	int i,es=0;
	for(i==0;i<vartablep;i++){
		if(strcmp(vartable[i].name,name)==0)
		{
			*paddress=vartable[i].address;
			break;
		}
	} 
	if(i==vartablep-1)
		printf("变量未声明!\n"); 
}

int TESTparse(){
	int es=0;
	if((fp=fopen("close.txt","r"))==NULL)
		es=10;
	if((fout=fopen("result.txt","w"))==NULL)
		es=10;	
	if(es==0)
		es=program();

	fclose(fp);
	fclose(fout);
	return es; 
} 
int program(){
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token1,"main"))
		printf("第%s行缺少main\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}	
	if(strcmp(token1,"("))
		printf("第%s行缺少(\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	if(strcmp(token1,")"))
		printf("第%s行缺少)\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	if(strcmp(token1,"{"))
		printf("第%s行缺少{\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	declaration_list();
	
	printf("	符号表\n");
	printf("	名字	地址\n");
	for(int i=0;i<vartablep;i++)
		printf("	%s	%d\n",vartable[i].name,vartable[i].address); 
	
	statement_list();
	if(fscanf(fp,"%s %s %s\n",&token,&token1,&token2)!=EOF)
		printf("singleword } %s\n",token2);
	else	printf("第%s行缺少}\n",token2);
	//if(strcmp(token1,"#"))
	//if(strcmp(token1,"}"))
	fprintf(fout,"	STOP\n");
	
		//printf("singleword }\n");
}
int declaration_list(){
	while(strcmp(token1,"int")==0){
		declaration_stat();
	}
}
int declaration_stat(){
	printf("%s %s %s\n",token,token1,token2);	
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token,"ID"))
		printf("第%s行缺少标识符\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		name_def(token1);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	if(strcmp(token1,";"))
		printf("第%s行缺少;\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	
}
int statement_list(){
	while(strcmp(token1,"}")&&!feof(fp)){	
		statement();
	}	
} 
int statement(){
	if(strcmp(token1,"if")==0) if_stat();
	if(strcmp(token1,"while")==0) while_stat();
	if(strcmp(token1,"for")==0) for_stat();
	if(strcmp(token1,"read")==0) read_stat();
	if(strcmp(token1,"write")==0) write_stat();
	if(strcmp(token1,"{")==0) compound_stat();
	if((strcmp(token,"ID")==0||strcmp(token,"NUM")==0||strcmp(token1,"(")==0)) 
		expression_stat();		
}
int if_stat(){
	int label1,label2; 
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token1,"("))
		printf("第%s行缺少(\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	expression();
	if(strcmp(token1,")"))
		printf("第%s行缺少)\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	label1=labelp++;
	fprintf(fout,"	BRF LABEL%d\n",label1);
	
	statement();
	label2=labelp++;
	fprintf(fout,"	BR LABEL%d\n",label2);
	fprintf(fout,"LABEL%d:\n",label1);	
	
	if(strcmp(token1,"else")==0){
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		statement();
	}
	fprintf(fout,"LABEL%d:\n",label2);	
}
int while_stat(){
	int label1,label2;
	label1=labelp++;
	fprintf(fout,"LABEL%d:\n",label1);
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token1,"("))
		printf("第%s行缺少(\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	expression();
	if(strcmp(token1,")"))
		printf("第%s行缺少)\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	label2=labelp++;
	fprintf(fout,"	BRF LABEL%d\n",label2);
	statement();	
	fprintf(fout,"	BR LABEL%d\n",label1);
	fprintf(fout,"LABEL%d:\n",label2);	
}
int for_stat(){
	int label1,label2,label3,label4;
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token1,"("))
		printf("第%s行缺少（\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	expression();
	fprintf(fout,"	POP\n");
	if(strcmp(token1,";"))
		printf("第%s行缺少;\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	label1=labelp++;
	fprintf(fout,"LABEL%d:\n",label1);
	expression();
	label2=labelp++;
	fprintf(fout,"	BRF LABEL%d\n",label2);
	label3=labelp++;
	fprintf(fout,"	BR LABEL%d\n",label3);
	if(strcmp(token1,";"))
		printf("第%s行缺少;\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	label4=labelp++;
	fprintf(fout,"LABEL%d:\n",label4);
	expression();
	fprintf(fout,"	POP\n");
	fprintf(fout,"	BR LABEL%d\n",label1);
	if(strcmp(token1,")"))
		printf("第%s行缺少)\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	fprintf(fout,"LABEL%d:\n",label3);
	statement();
	fprintf(fout,"	BR LABEL%d\n",label4);
	fprintf(fout,"LABEL%d:\n",label2);
}
int write_stat(){
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	expression();
	if(strcmp(token1,";"))
		printf("第%s行缺少;\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	statement();
}
int read_stat(){
	int address;
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token,"ID"))
		printf("第%s行缺少标识符\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		lookup(token,&address);
		fprintf(fout,"	IN	\n");
		fprintf(fout,"	STO	%d\n",address);
		fprintf(fout,"	POP\n");
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}

	if(strcmp(token1,";"))
		printf("第%s行缺少;\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
}
int compound_stat(){
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	statement_list();  
	if(strcmp(token1,"}")) 
		printf("第%s行缺少}\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);	
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
	}
}
int expression_stat(){
	if(strcmp(token1,";")==0){
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
	}	
	expression();
	fprintf(fout,"	POP\n");
	if(strcmp(token1,";")==0){ 
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);		
	} 
	else
		printf("第%s行缺少;\n",token2);
}
int expression(){
	int fileadd;
	char token3[20],token4[40],token5[20];
	if(strcmp(token,"ID")==0){
		fileadd=ftell(fp);
		fscanf(fp,"%s %s %s\n",&token3,&token4,&token5);
		if(strcmp(token4,"=")==0){	
			int address;
			lookup(token1,&address); 
			printf("%s %s %s\n",token,token1,&token2);			
			printf("%s %s %s\n",token3,token4,&token5);
			fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
			bool_expr();
			fprintf(fout,"	STO %d\n",address);
		}else{
			fseek(fp,fileadd,0);	
			bool_expr();
		} 
	}
	else bool_expr();
}
int bool_expr(){
	additive_expr();
	if(strcmp(token1,">")==0||strcmp(token1,">=")==0||strcmp(token1,"<")==0||
	    strcmp(token1,"<=")==0||strcmp(token1,"==")==0||strcmp(token1,"!=")==0){
	    char token6[20];
	    strcpy(token6,token1);
		printf("%s %s %s\n",token,token1,token2);	
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
		additive_expr();
		if(strcmp(token6,">")==0) fprintf(fout,"	GT\n");	   
		if(strcmp(token6,">=")==0) fprintf(fout,"	GE\n");	   
		if(strcmp(token6,"<")==0) fprintf(fout,"	LES\n");	   
		if(strcmp(token6,"<=")==0) fprintf(fout,"	LE\n");	   
		if(strcmp(token6,"==")==0) fprintf(fout,"	EQ\n");	   
		if(strcmp(token6,"!=")==0) fprintf(fout,"	NOTEQ\n");	     	
	}
} 
int additive_expr(){
	term();
	while(strcmp(token1,"+")==0||strcmp(token1,"-")==0){
		char token6[20];
	    strcpy(token6,token1);
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
		term();
		if(strcmp(token6,"+")==0) fprintf(fout,"	ADD\n");	   
		if(strcmp(token6,"-")==0) fprintf(fout,"	SUB\n");	   
	}
}
int term(){
	factor();
	while(strcmp(token1,"*")==0||strcmp(token1,"/")==0){
		char token6[20];
	    strcpy(token6,token1);
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		term();
		if(strcmp(token6,"*")==0) fprintf(fout,"	MULT\n");	   
		if(strcmp(token6,"/")==0) fprintf(fout,"	DIV\n");
	}
}
int factor(){
	if(strcmp(token1,"(")==0){
		printf("%s %s %s\n",token,token1,token2);	
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		expression();
		if(strcmp(token1,")")) 
			printf("第%s行缺少)\n",token2);
		else{
		printf("%s %s %s\n",token,token1,token2);	
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		}
	}else{
		if(strcmp(token,"ID")==0){
			int address;
			lookup(token1,&address);
			fprintf(fout,"	LOAD %d\n",address);
			printf("%s %s %s\n",token,token1,token2);	 
			fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		}
		else if(strcmp(token,"NUM")==0){
			
			fprintf(fout,"	LOADI %s\n",token1);
			printf("%s %s %s\n",token,token1,token2);	 
			fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		}
		else
			printf("第%s行缺少操作数\n",token2);	
	}
}
int main(){
	int es=0;
	es=TESTscan();
	//printf("%d",es);
	if(es>0)
		printf("词法分析错误，编译停止\n");
	else printf("词法分析成功\n");
	TESTparse();	
	return 0;
} 
