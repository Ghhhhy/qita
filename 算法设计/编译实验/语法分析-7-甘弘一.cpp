#include<stdio.h>
#include<ctype.h>
#include<conio.h>
#include"词法分析-7-甘弘一.H"
 
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

int TESTparse(){
	int es=0;
	if((fp=fopen("close.txt","r"))==NULL)
		es=10;
	if(es==0)
		es=program();

	fclose(fp);
	return es; 
} 
int program(){
	int es=0;
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
	statement_list();
	if(fscanf(fp,"%s %s %s\n",&token,&token1,&token2)!=EOF)
		printf("singleword } %s\n",token2);
	else	printf("第%s行缺少}\n",token2);
	//if(strcmp(token1,"#"))
	//if(strcmp(token1,"}"))
	
	
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
	statement();
	if(strcmp(token1,"else")==0){
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		statement();
	}
}
int while_stat(){
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
	statement();
}
int for_stat(){
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token1,"("))
		printf("第%s行缺少（\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	expression();
	if(strcmp(token1,";"))
		printf("第%s行缺少;\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	}
	expression();
	if(strcmp(token1,";"))
		printf("第%s行缺少;\n",token2);
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
	statement();
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
	printf("%s %s %s\n",token,token1,token2);
	fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
	if(strcmp(token,"ID"))
		printf("第%s行缺少标识符\n",token2);
	else{
		printf("%s %s %s\n",token,token1,token2);
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
			printf("%s %s %s\n",token,token1,&token2);			
			printf("%s %s %s\n",token3,token4,&token5);
			fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
			bool_expr();
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
	    printf("%s %s %s\n",token,token1,token2);	
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
		additive_expr();	   	
	}
} 
int additive_expr(){
	term();
	while(strcmp(token1,"+")==0||strcmp(token1,"-")==0){
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);	
		term();
	}
}
int term(){
	factor();
	while(strcmp(token1,"*")==0||strcmp(token1,"/")==0){
		printf("%s %s %s\n",token,token1,token2);
		fscanf(fp,"%s %s %s\n",&token,&token1,&token2);
		term();
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
		if(strcmp(token,"ID")==0||strcmp(token,"NUM")==0){
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
