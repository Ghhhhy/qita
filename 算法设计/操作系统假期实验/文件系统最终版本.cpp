#include <stdlib.h>
#include <stdio.h>  
#include <malloc.h>  
#include <string.h>  
#include <time.h>
#include <math.h>

#include <ctype.h>

#define BLOCKSIZE 512  // 磁盘块大小 
#define Nofile 10  /*u_ofile表可分配的个数*/ 
#define Nfile 10  /*file表可分配的个数*/ 
#define Nfcb 32  /*pipecb可分配的个数*/


struct 
{
   char filename[100];
   int  file_fd;   
}open_file[100]; 

/*u_file表*/ 
int u_ofile[Nofile]; 

/*file表*/ 
struct 
{ 
   int f_flag;/*可读可写标志，0可读，1可读可写*/ 
   int f_state;/*表示此表项的状态，=0表示此表项没被使用，可分配；=1表示此表项在被*/
   int f_fcb;/*..下打开文件则对应的fcb表下标，主目录或子目录下打开文件对应文件控制块数据在模拟磁盘文件.txt中的地址*/ 
   long f_woffset;/*写指针，已写个数*/
   long f_roffset;/*读指针，已读个数*/ 
}file[Nfile];

/*文件控制块*/ 
struct fcb
{
   char filename[9];
   char state;//可用标志,0表示可用，1表示不可用
   char type; //属性字段，0表示目录		文件，1表示数据文件
   int size;//文件长度
   int firstnode;//文件分配表的头结点
	}fcb[Nfcb]; 

//Fat文件分配表定义
struct FATTABLE
{
    char state;//可用标志,0表示可用，1表示不可用
	int nextbl;//0表示当前块为最后块，不为零指示下一块
}FATTABLE[1000];

//保存当前目录的fcb
struct Curfcb		
{
   char filename[9];//                                                                  前八位文件名字 
   char state;//可用标志,0表示可用，1表示不可用										      9
   char type; //属性字段，0表示目录文件，1表示数据文件                          		  10
   int size;//文件长度																	  八位 
   int firstnode;//文件头结点															  三位 
   int index;//指向根目录文件fcb信息表中某一项，为-1表示当前是在子目录下				  
   int address;//保存子目录文件fcb信息在磁盘中的位置，留以后修改size用
}curfcb;

FILE *fd;
int address;	//读写模拟磁盘即模拟磁盘文件.txt的地址
int systate=1;	//0当前可创建根目录和文件;1当前处在一个根目录下,可创建子目录和文件;系统刚启动为1 

//为/root根目录下
char curpath[30]="/root";	//保存当前目录路径
char fileinit[]="110       ";//数据文件fcb的初始化  1已经用， 1文件， 0文件大小 
char dirinit[]="100       ";//目录文件fcb的初始化   1已经用， 0目录， 0文件大小 


void initialize();
void startsystem();
int Fcreat(char *filename);
int addir(char *dirname);
int Ff_d(char *name,int flag);
int Frootf_d(char *name,int flag);
int Fopen(char *filename,int flag);
int Fread(int fp,char *abuf,int n);
int Fwrite(int fp,char *abuff,int n);
void Fclose(int fp);
int cd(char *dirname);
void ls();
void root();
int quit();
int check(char *name,int flag);
int getblock(int mode);
void Setcurfcb(char *abuf);
void print();
void print_u_ofile();
void print_FAT();
void print_fcb();
void print_open_file();
char *rtrim(char *str);

void welcome()//系统界面欢迎函数
{
	printf("*****************************************************************\n");
	printf("**\t\t <<欢迎来到文件系统!>>                         **\n");
	printf("**\t\t     甘弘一小组完成                            **\n");
	printf("*****************************************************************\n");
}
void showchoice()//选择提示函数
{
    printf("** 以下为操作提示信息:                                         **\n");
	printf("*****************************************************************\n");
	printf("**\tcreat_file  filename   :创建文件!                      **\n");
    printf("**\tcreat_dir  dirname     :创建目录!                      **\n");
	printf("**\tcd  dirname            :切换目录！                     **\n");
	printf("**\tquit  system           :退出系统！                     **\n");
    printf("**\tls  -a                 :列出目录/文件!                 **\n");
	printf("**\topen  filename         :打开文件！                     **\n");
    printf("**\tread  filename         :读取文件！                     **\n");
    printf("**\twrite  filename        :写文件！                       **\n");
    printf("**\tclose  filename        :关闭文件!                      **\n");
    printf("**\tprint  FILE            :打印FILE表！                   **\n");
    printf("**\tprint  FAT             :打印FAT表！                    **\n");
    printf("**\tprint  FCB             :打印FCB表！                    **\n");
    printf("**\tprint  OPEN_FIle       :打印当前已打开文件！           **\n");
	printf("*****************************************************************\n");
}
int main()
{
	char filename[9],dirname[9]; //中间变量  文件名/目录名 
	int flag,i,canshu,n;
	
	char abuf[100]="0";
	initialize();
	startsystem();
	
	int system=1; 
	welcome();
	showchoice(); 
	
	while(system){
		i = -1;
		printf("%s：",curpath);  //开机运行为root 
		char flag1[20];
		char flag2[20];	
		scanf("%s%s",flag1,flag2); 
		if(strcmp(flag1,"creat_file")==0)
		   i=0;
		else if(strcmp(flag1,"creat_dir")==0)
		   i=1;
		else if(strcmp(flag1,"cd")==0)
		{
			if(strcmp(flag2,"../")==0)
			    i =4;
			else 
			    i=2;
		}
		else if(strcmp(flag1,"quit")==0)
		   i=3;
		else if(strcmp(flag1,"ls")==0)
		   i=5; 	   
		else if(strcmp(flag1,"open")==0)
		   i=6;
		else if(strcmp(flag1,"read")==0)
		   i=7;
		else if(strcmp(flag1,"write")==0)
		   i=8; 
		else if(strcmp(flag1,"close")==0)
		   i=9; 
		else if(strcmp(flag1,"print")==0)
		{
			if(strcmp(flag2,"FILE")==0)
			    i=10;
			else if(strcmp(flag2,"FAT")==0)
			    i=11;
			else if(strcmp(flag2,"FCB")==0) 
			    i=12; 
			else
			    i = 13;
		} 	
//		getchar();
		switch(i){
		case 0:
			flag=Fcreat(flag2);
			if(flag){
				printf("创建文件成功\n");
				quit();
			}
			break;
		case 1:
			flag=addir(flag2);
			if(flag){
				printf("创建目录成功\n");
				quit();
			}
			break;
		case 2:
			flag=cd(flag2);
			if(flag){
				printf("进入成功\n");
				if(flag==1){
					strcpy(curpath,"/");
					strcat(curpath,flag2);
				}
				else{
					strcat(curpath,"/");  //flag=1  此时为root/子目录：的形式 
					strcat(curpath,flag2);
				}
			}
			else printf("不存在所输入目录名\n");
			break;
		case 3:
			system=0;
			printf("退出成功\n");
			break;
		case 4:
			root();
			break;
		case 5:
			ls();
			break;
		case 6:
			printf("输入文件打开方式(只读:R,可读可写:RW）:");  //打开的方式 
			char open_method[10];
			scanf("%s",open_method);
			if(strcmp(open_method, "R")==0)
			    canshu = 0;
			else
			    canshu = 1; 
			 
			if(canshu!=0&&canshu!=1)
				printf("打开方式错误，文件打开失败\n");
			else
				{
				char  filenametemp[20];
				strcpy(filenametemp,flag2);
				flag=Fopen(flag2,canshu);//返回fd 
				 for(int open=0;open<100;open++)
				 {
				 	if(strcmp(open_file[open].filename,"0")==0)
				 	{
				 	    char curtemp[20];
				 	    strcpy(curtemp,curpath);
				 		strcpy(open_file[open].filename,rtrim(curtemp));
				 		strcat(open_file[open].filename,"/");
				 		strcat(open_file[open].filename,filenametemp);
				 		open_file[open].file_fd  = flag ;
				 		break;
					 }
				 }
				 
		        }
			break;
		case 7:
	        for(int pp = 0;pp<100;pp++)
	        {
	        	if(strcmp(flag2,open_file[pp].filename)==0)
	        	{
	        		flag = open_file[pp].file_fd;
	        		break;
				}
				 
			}		
//		    flag= atoi(flag2);
			if(u_ofile[flag]==-1){//判断读取文件是否被被打开 
				printf("无此打开文件标识符\n");
				break;
			}
			printf("输入读取数据的字节数:");
			scanf("%d",&n);
			Fread(flag,abuf,n); //字节数，读取数据放在abuf中 
			printf("读的数据为：%s\n",abuf);
			break;
		case 8:
		   for(int pp = 0;pp<100;pp++)
	        {
	        	if(strcmp(flag2,open_file[pp].filename)==0)
	        	{
	        		flag = open_file[pp].file_fd;
	        		break;
				}	 
			}
//		    flag= atoi(flag2);
			if(u_ofile[flag]==-1){//判断写入文件是否被被打开 
				printf("无此打开文件标识符\n");
				break;
			}
			printf("输入要写的数据:");
			getchar();
			gets(abuf);//flag是打开文件描述符 
			if(Fwrite(flag,abuf,strlen(abuf))==-1)
				printf("对打开的文件没有写权限！\n");
			else {
				printf("写入成功\n");
				quit();
			}
			break;
		case 9:
			for(int pp = 0;pp<100;pp++)
	        {
	        	if(strcmp(flag2,open_file[pp].filename)==0)
	        	{
	        		flag = open_file[pp].file_fd;
	        		strcpy(open_file[pp].filename,"0"); 
	        		break;
				}	 
			}
//		    flag= atoi(flag2);
			Fclose(flag);
			break;		
		case 10:
		    print_u_ofile();
			break; 
		case 11:
			print_FAT();
			break;		
		case 12:
			print_fcb();
			break;
		case 13:
			print_open_file();
		default:
			break;
		}
	}
}
// 读取当前目录（root）的fcb等信息 
void startsystem()  
{
	int i,j;
	char abuf[32]=" ",abuf2[8]="0";
	fd = fopen("模拟磁盘文件.txt", "r");
	address=3*BLOCKSIZE;
	fseek(fd,address,SEEK_SET);
	
	
	
	
	//这一步应该是从内存中   读取fat表的信息 
	//这一步不是很懂        从fat表第四（3+1）行  开始读1000*（3+1）个字节给FATTABLE 
	for(i=0;i<1000;i++){
		fread(abuf,sizeof(char),1,fd);
		FATTABLE[i].state=abuf[0];
		fread(abuf,sizeof(char),3,fd);
		FATTABLE[i].nextbl= atoi(abuf);
	}
	
	
	//第二行 
	address=1*BLOCKSIZE;
	fseek(fd,address,SEEK_SET);
	
	
	//32字节  读取root 的fcb信息 
	fread(abuf,sizeof(char),32,fd);
	Setcurfcb(abuf); //当前目录为root目录 
	
	 
	 
	 
	 //根目录区为两个物理块 为512*2 = 32 * 32 
	
	//新增加32个fcb
	//fcb的信息都是再第二行  根目录中 
//	注意512/32 = 16 
	for(i=0;i<32;i++){
		for(j=0;j<8;j++)//首个abuf为root的fcb 
			fcb[i].filename[j]=abuf[j];
		fcb[i].state=abuf[8];  //1 表示被使用了 
		fcb[i].type=abuf[9];
		for(j=0;j<8;j++)
			abuf2[j]=abuf[10+j];
		fcb[i].size= atoi(abuf2);		
		for(j=0;j<3;j++)
			abuf2[j]=abuf[18+j];
		fcb[i].firstnode= atoi(abuf2);
		fread(abuf,sizeof(char),32,fd);  ///再读32个字节信息 
	}
	fclose(fd);
}


int getblock(int mode){//mode为0表示首次申请块，不为0表示上一个块
	int i;  //不为0 表示对物理块进行增加 	
	//989 个 数据区 
	for(i=0;i<1000;i++)  //第一次为1 
		if(FATTABLE[i].state=='0')	break; 
	//此时仍然有多余的数据区  即物理块	 
	if(i<1000){
		FATTABLE[i].state='1';
		if(mode)	FATTABLE[mode].nextbl=i;     
		return i+1; //返回得到的物理块号 
	}
	else 
		return -1;
}



void print(){
	int i;
	printf("state type filename size firstnode\n");
	for(i=0;i<Nfcb;i++)
       		printf("%c  %c  %s  %d  %d\n",fcb[i].state,fcb[i].type,fcb[i].filename,fcb[i].size,fcb[i].firstnode); 
    printf("FATTABLE state nextbl\n");
	for(i=0;i<15;i++) 
       		printf("%c\t%d\n",FATTABLE[i].state,FATTABLE[i].nextbl);
	printf("u_ofile\n");
	for(i=0;i<Nofile;i++) 
       		printf("%d\n",u_ofile[i]);
	printf("file\n");
    for(i=0;i<Nfile;i++) 
       		printf("%d\t%d\t%d\t%d\t%d\n",file[i].f_state,file[i].f_flag,file[i].f_fcb,file[i].f_woffset,file[i].f_roffset);
}



//设置当前的curfcb 
void Setcurfcb(char *abuf){
	int j;
	char abuf2[8]="0"; 
	for(j=0;j<8;j++)
		curfcb.filename[j]=abuf[j];
	curfcb.state=abuf[8];
	curfcb.type=abuf[9];
	for(j=0;j<6;j++)
		abuf2[j]=abuf[10+j];
	curfcb.size= atoi(abuf2);		
	for(j=0;j<3;j++)
		abuf2[j]=abuf[16+j];
	curfcb.firstnode= atoi(abuf2);
}






























//模拟磁盘1000*512bit,初始化全0
void initialize(){
	int i;
	char s[512];
    //u_ofile表初始化全部为-1，未使用 
	for(i=0;i<Nofile;i++) 
       		u_ofile[i] = -1; 
    //file表初始化全部为-1，未使用 
    for(i=0;i<Nfile;i++){
       		file[i].f_state= -1;
			file[i].f_flag=-1;
			file[i].f_fcb=-1;
			file[i].f_woffset=-1;
			file[i].f_roffset=-1;
	}	
    //文件不存在时，才会进入if语句，进行初始化磁盘文件 
	if((fd = fopen("模拟磁盘文件.txt", "r")) == NULL){
		printf("创建模拟磁盘成功\n");
	//将数组初始化一个物理块512字节为0	
	memset(s,'0',sizeof(char)*512);	
	//初始化磁盘文件，将1000个物理块初始化为0 
	fd=fopen("模拟磁盘文件.txt", "w");
	for(i=0;i<1000;i++)
		fwrite(s,sizeof(char),512,fd);	
	fclose(fd);	
	//模拟磁盘不存在时，根目录也要重新设计 
	fd = fopen("模拟磁盘文件.txt", "r+");
	address=1*BLOCKSIZE;
	fseek(fd,address,SEEK_SET); //重定义文件的开头 
	fwrite("root    \0",sizeof(char),8,fd);
	fwrite("1",sizeof(char),1,fd);
	fwrite("0",sizeof(char),1,fd);
	fwrite("0       ",sizeof(char),8,fd);
	fwrite("1",sizeof(char),3,fd);
	//第四行 fat表给个1 ,表示第一个物理块被使用，即根目录使用了 
	address=3*BLOCKSIZE;   
	fseek(fd,address,SEEK_SET);
	fwrite("1",sizeof(char),1,fd);
	}
	fclose(fd);

	for(i=0;i<100;i++)
	{
		strcpy(open_file[i].filename,"0");
	} 
}

void print_open_file()
{
	printf("------------------------------\n");
	printf("Filename          |文件描述符|\n");
	for(int i=0;i<100;i++)
	{
		if( strcmp(open_file[i].filename,"0")==0)
		    break;
		printf("------------------------------\n");
		
		printf("%-18s|%-10d|\n",open_file[i].filename,open_file[i].file_fd); 
	}
	printf("------------------------------\n");
}
void print_FAT()
{
	printf("------------------------\n");
	printf("块数  |State |NextBlock|\n");
	for(int i=0;i<10;i++)
	{
		if( FATTABLE[i].state=='0')
		    break;
		printf("------------------------\n");
		
		printf("第%d块 |%-6s|%-9s|\n",i+1,"使用","最后块"); 
	}
	printf("------------------------\n");
}
void print_u_ofile()

{
//	system("color 5");
	printf("----------------------------------------------\n"); 
    printf("U_OFILE |Flag |State |Fcb   |Woffset |Roffset| \n"); 
	for(int i=0; i<10; i++)
	{    
	    if(file[i].f_state==-1)
	        break;
	    char flag[4];
		if(file[i].f_flag==0)
		    strcpy(flag,"读");
		else
		    strcpy(flag,"读&写");
	    printf("----------------------------------------------\n"); 
	    printf("%-8d|%-5s|%-6s|%-6d|%-8d|%-7d|\n",u_ofile[i],flag,"使用",file[i].f_fcb,file[i].f_woffset,file[i].f_roffset);
	}
	printf("----------------------------------------------\n"); 
	return ;
	    
}

void print_fcb()

{
//	system("color 5");
	printf("----------------------------------------\n"); 
    printf("Filename |State |Type |Size |FirstBlock| \n"); 
	for(int i=0; i<10; i++)
	{    
	    if(fcb[i].state=='0')
	        break;
	    char  flag[5];
	    if(fcb[i].type=='0')
		    strcpy(flag,"目录");
		else
		    strcpy(flag,"文件");
	    printf("----------------------------------------\n"); 
	    printf("%-9s|%-6s|%-5s|%-5d|第%d块     |\n",fcb[i].filename,"使用",flag,fcb[i].size,fcb[i].firstnode);
	}
	printf("----------------------------------------\n"); 
	return ;
	    
}

int Ff_d(char *name,int flag){//创建目录或文件
	int i,j;
	if(strlen(name)<=8){
		for(i=strlen(name);i<8;i++)
			strcat(name," ");
	}
	for(i=0;i<Nfcb;i++){
		if(fcb[i].state=='0'){
			for(j=0;j<8;j++)
				fcb[i].filename[j]=*(name+j);                        //件  的个数？
			fcb[i].state='1';
			if(flag)
				fcb[i].type='1';//创建文件 
			else 
				fcb[i].type='0';//创建目录 
			fcb[i].size=0;
			fcb[i].firstnode=getblock(0);
			break;
		}
	}
	if(i==Nfcb){
		printf("fcb表已满，创建根目录或文件失败\n");
		return 0;}
	return 1;
}

//创建根目录下的文件或子目录
int Frootf_d(char *name,int flag){
	int i,size,offset,p,blocknum;
	char abuf[33]="0",a[3]="0",abuf2[9]="0";
	blocknum=getblock(0);	//先分配块,失败则函数返回-1
	if(blocknum<0){
		printf("\n分配块失败，创建失败\n");
		return 0;
	}
	if(strlen(name)<=8){	//将名字填充空格
		for(i=strlen(name);i<8;i++)
			strcat(name," ");
	}
	size=curfcb.size;			//修改当前fcb即curfcb的size信息，并拿出firstnode，找
	//到最后一个块号
	p=curfcb.firstnode;	
	while(FATTABLE[p].nextbl!=0)
		p=FATTABLE[p].nextbl;
	// 此时 p= 最后一个物理块
	//此时size为实际的大小  而不是 几个fcb的意思         n*512% 512 = 0	
	offset=size%BLOCKSIZE;  //判断size大小是否已经多出  一个物理块的大小
	 //offset为最后一个物理块的超出部分   
	if(size!=0&&offset==0)	//目录文件最后块已占满，申请新块
		p=getblock(p);//p更新了 
		
//    更新 fcb的size大小 
	curfcb.size+=32;
	fd=fopen("模拟磁盘文件.txt", "r+");
	if(curfcb.index>=0)	 //为0是根目录 否则为子目录 
		//修改当前fcb即curfcb的size信息，当前目录若为子目录
		fcb[curfcb.index].size=curfcb.size;//则要将修改的size信息写入模拟磁盘文件.txt文件
	else
	{
		address=curfcb.address;
		fseek(fd,address,SEEK_SET);
		fread(abuf,sizeof(char),32,fd);
		itoa(curfcb.size,abuf2,10);//转换成10进制 
		for(i=0;i<8;i++){
			if(i<strlen(abuf2))
				abuf[i+10]=abuf2[i];
			else
				abuf[i+10]=' ';
			}
		fseek(fd,address,SEEK_SET);
		fwrite(abuf,sizeof(char),32,fd);
	}
	
	
	address=(10+p)*BLOCKSIZE+offset;  //移动最后物理块的最后一个位置
	 
	fseek(fd,address,SEEK_SET);
	fwrite(name,sizeof(char),8,fd);//写入文件名
 	
	if(flag) 
		fwrite(fileinit,sizeof(char),10,fd);
	else 
		fwrite(dirinit,sizeof(char),10,fd);	//将文件名或目录名以及基本信息写入当前目录文件中
	
	
	itoa(blocknum,a,10);//将申请的块号也写入
	fwrite(a,sizeof(char),3,fd);
	fclose(fd);
	return 1;
}


int Fcreat(char *filename){
	int flag;
	flag = check(filename,1);//检查文件名 

	if(flag==0){ 
		printf("文件名重复\n");
		return 0;
	}
		 
	if(systate==0)  //子目录下 
		flag=Ff_d(filename,1);
		
	else if(systate==1)  //根目录下 
		flag=Frootf_d(filename,1);
	return flag;
	
}

int addir(char *dirname){
	int flag;
	flag=check(dirname,0); //检查目录名 
	
	
	if(flag==0) 
		{printf("目录名重复\n");
		 return 0;}
	if(systate==0)       //子目录下创建目录/文件 
		flag=Ff_d(dirname,0);
	else if(systate==1)  //根目录下创建文件/目录 
		flag=Frootf_d(dirname,0);
	return flag;
}


void root(){
	if(systate){
		systate=0;
		strcpy(curpath,"/..");
	}
}


int cd(char *dirname){
	char abuf[33]="0";
	int p,size,n,i,j,address,num=32;
	
	//文件/目录名固定8位 
	if(strlen(dirname)<=8){
		for(i=strlen(dirname);i<8;i++)
			strcat(dirname," ");
	}
	
	if(systate){//当前为根目录下
		p=curfcb.firstnode;
		size=curfcb.size;
		fd=fopen("模拟磁盘文件.txt", "r");
		address=(10+p)*BLOCKSIZE;//找根目录的数据区 
		fseek(fd,address,SEEK_SET);
		n=size/32;//得到根目录下的fcb信息     共n个fcb 
		 
		while(n>0||FATTABLE[p].nextbl){
			if(n<32) num=n;
			
			for(i=0;i<num;i++){
				fread(abuf,sizeof(char),32,fd);
				if(abuf[9]=='1') continue; //a[9]=0为目录，1为文件 
				for(j=0;j<8;j++)
					if(abuf[j]!=dirname[j])//判断cd的目录名是否相同 
						break;
				if (j==8){//目录名存在 
					Setcurfcb(abuf);   //设置新的当前curfcb 
					curfcb.index=-1;//设置为子目录 
					curfcb.address=(int)ftell(fd)-32; //保存当前子目录的fcb信息（根目录的数据区中） 
//					C 库函数 long int ftell(FILE *stream) 返回给定流 stream 的当前文件位置。
					n=0;
					break;
				}
			}
			//n-=32;
			p=FATTABLE[p].nextbl;//读取下一个物理块 
			address=(11+p)*BLOCKSIZE;
			fseek(fd,address,SEEK_SET);
		}
		if(n==0) return 2;  //目录文件名字存在 
		else return 0;
		fclose(fd);
	}
	
	else {	//当前目录为/..
		for(i=0;i<Nfcb;i++){
			if(fcb[i].type=='0'&&strcmp(fcb[i].filename,dirname)==0){
				strcpy(curfcb.filename,dirname);
				curfcb.firstnode=fcb[i].firstnode;
				curfcb.size=fcb[i].size;
				curfcb.index=i;
				break;
			}
		}
		if (i==Nfcb) return 0;
		else {
			systate=1;
			return 1;
		}
	}
}


int quit(){
	char abuf[33]="0",abuf2[10]="0";
	int i,j;
	fd=fopen("模拟磁盘文件.txt","r+");
	address=3*BLOCKSIZE;
	fseek(fd,address,SEEK_SET);
	for(i=0;i<10;i++){
		if(FATTABLE[i].state=='0')continue;
		abuf[0]=FATTABLE[i].state;
		itoa(FATTABLE[i].nextbl,abuf2,10);

		for(j=0;j<3;j++){
			if(j<strlen(abuf2))
				abuf[j+1]=abuf2[j];//读出 
			else
				abuf[j+1]=' ';
			}
		fwrite(abuf,sizeof(char),3,fd);
	}
	address=1*BLOCKSIZE;
	fseek(fd,address,SEEK_SET);
	for(j=0;j<32;j++){
		if(fcb[j].state=='0') continue;
		strcpy(abuf,fcb[j].filename);
		abuf[8]=fcb[j].state;
		abuf[9]=fcb[j].type;

		itoa(fcb[j].size,abuf2,10);
		for(i=0;i<8;i++){
			if(i<strlen(abuf2))
				abuf[i+10]=abuf2[i];
			else
				abuf[i+10]=' ';
			}
		itoa(fcb[j].firstnode,abuf2,10);
		for(i=0;i<3;i++){
			if(i<strlen(abuf2))
				abuf[i+18]=abuf2[i];
			else
				abuf[i+18]=' ';
			}
		for(i=strlen(abuf);i<32;i++)
			abuf[i]='0';
		fwrite(abuf,sizeof(char),32,fd);
	}
	fclose(fd);
	return 1;
}


int check(char *name,int flag){	//flag为0表示检查目录名，为1表示检查文件名
	char abuf[33]="0",f;
	int p,size,n,i,j,address,num=16;
	
    if(strlen(name)<=8){
	for(i=strlen(name);i<8;i++)
			strcat(name," "); //文件名固定为8位数 
    }
		
		
	if(systate){   //根目录 
		if(flag)f='0'; //查文件名 
		else f='1';//查目录名 
		
		p=curfcb.firstnode;//第一个物理块号   数据区的第一块物理块 
		
		
		size=curfcb.size;//文件大小   以字节为单位的 
		
		fd=fopen("模拟磁盘文件.txt", "r");
		address=(10+p)*BLOCKSIZE;    //数据区从12物理块开始 
		fseek(fd,address,SEEK_SET);  //读取根目录下的子目录及文件的fcb 
		
		
		n=size/32;  //n 表示n个fcb信息，因为一个fcb有32个字节
		// 注意  32/32 = 1，   此后有n个fcb  即n个文件或目录信息 
		
		//  32 * 16 = 512 
		//第一次创建wang的文件时候，n=0， Fta【1】.nextbl=0 
		while(n>0||FATTABLE[p].nextbl){ //或者数据区不止一个物理块 	
			if(n<16)  num=n;  //num初值为16 一个物理块共有16个fcb信息 
			for(i=0;i<num;i++){   // f = 0 是文件； f = 1是目录 
				fread(abuf,sizeof(char),32,fd);
				//abuf的0是目录， 1是文件 
				if(abuf[9]==f)continue;     //f0=是文件。1是目录 
				//文件对应文件/目录对应目录才进行检查名字冲突的问题 
				for(j=0;j<8;j++){    //是文件则检查名字冲突问题 
					if(abuf[j]!=name[j])
						break;
					}
				if (j==8){
					fclose(fd);
					return 0;//有重复名
					}
				}
			n-=16;  //找到下一个物理块 
			p=FATTABLE[p].nextbl;
			address=(11+p)*BLOCKSIZE;
			fseek(fd,address,SEEK_SET);
		}
	}
	
	
	else{   //子目录 
		for(i=0;i<Nfcb;i++){
			if(fcb[i].state=='1'&&strcmp(name,fcb[i].filename)==0)
				return 0;	
			}
		}
		
		 
		return 1;//无重复名
}


int Fopen(char *filename,int flag){//0表示可读，1表示可读可写
	char abuf[33]="0",abuf2[10]="0";
	int p,size,n,i,j,address,num=32;
	if(strlen(filename)<=8){
		for(i=strlen(filename);i<8;i++)
			strcat(filename," ");
	}
	
	n=check(filename,1);
	if(n){
		printf("文件名不存在，打开文件失败!\n");
		return -1;
	}
		
	//根目录或子目录下打开文件
	if(systate){
			p=curfcb.firstnode;
			size=curfcb.size;
			fd=fopen("模拟磁盘文件.txt", "r");
			address=(10+p)*BLOCKSIZE;
			fseek(fd,address,SEEK_SET);
			
			n=size/32;//n个fcb信息 
			while(n>0||FATTABLE[p].nextbl){
				if(n<32)num=n;
				for(i=0;i<num;i++){
					fread(abuf,sizeof(char),32,fd);
					if(abuf[9]=='0')continue;
					for(j=0;j<8;j++){
						if(abuf[j]!=filename[j])
							break;
						}
					if (j==8){
						for(j=0;j<6;j++)
							abuf2[j]=abuf[10+j];
						size= atoi(abuf2); //文件的大小 
						
						j=ftell(fd)-32;//找到根/子目录下打开文件的fcb信息   再fat。txt中的准确地址 
						
						n=0;
						break;
						}
					}
				n-=32;
				p=FATTABLE[p].nextbl;
				address=(10+p)*BLOCKSIZE;
				fseek(fd,address,SEEK_SET);
			}
	
	}
	//在..目录下打开文件
	else{
		for(i=0;i<Nfcb;i++){
			if(fcb[i].type=='1'&&strcmp(fcb[i].filename,filename)==0){
				j=i;
				size=fcb[j].size;
				break;
				}
		}
	}

	for(i=0;i<Nfile;i++){
		if(file[i].f_state==-1){
			file[i].f_flag=flag;//打开方式  1可读可写  0只读 
			file[i].f_state=1;//使用 
			file[i].f_fcb=j;//文件控制块的地址 
			file[i].f_roffset=0;
			file[i].f_woffset=size;
			j=i;
			break;
			}
		}
	for(i=0;i<Nofile;i++)
		if(u_ofile[i]==-1){
			u_ofile[i]=j;//记录file表下标 
			j=i;
			break;
		}
return j; //记录打开文件的描述符 
}


void ls(){
	
	printf("----------------\n");
	printf("Filename |Type |\n");
    printf("----------------\n");

	int i,p,size,n,num;
	char abuf[33]="0";
	if(systate){    //刚开机为1 
		p=curfcb.firstnode;
		size=curfcb.size;
		fd=fopen("模拟磁盘文件.txt", "r");
		address=(10+p)*BLOCKSIZE;
		fseek(fd,address,SEEK_SET);
		n=size/32;
		while(n>0||FATTABLE[p].nextbl){
			if(n<32)num=n;
			for(i=0;i<num;i++){
				fread(abuf,sizeof(char),8,fd);
				printf("%-9s|",abuf);
				fread(abuf,sizeof(char),2,fd);
				if(abuf[1]=='1')printf("文件 |\n");
				else	printf("目录 |\n");
				printf("----------------\n");
				
				fseek(fd,22,SEEK_CUR);
				}
			n-=32;
			p=FATTABLE[p].nextbl;
			address=(10+p)*BLOCKSIZE;
			fseek(fd,address,SEEK_SET);
		}
	}
	else{
		for(i=0;i<Nfcb;i++){
			if(fcb[i].state=='1'){
				printf("%-9s|",fcb[i].filename);
				if(fcb[i].type=='0')printf("目录 |\n");
				else printf("文件 |\n");
				printf("----------------\n");
			}
		}
	}
	fclose(fd);
}


int Fread(int fp,char *abuff,int n){
	int address,j,size,offset,node,blonum,bytenum,rest;
	char *data;
	char abuf[1025]="0",abuf2[10]="0";
	fd = fopen("模拟磁盘文件.txt", "r");
	
	data=(char *)malloc(n*sizeof(char));  //读取n个数据 
	memset(data,'\0',sizeof(char)*n);     //字符串末尾时结束 


	if(systate){
		address=file[u_ofile[fp]].f_fcb;//找到fcb地址 
		fseek(fd,address,SEEK_SET);
		fread(abuf,sizeof(char),32,fd);	
		
		for(j=0;j<8;j++)
			abuf2[j]=abuf[10+j];//找到文件大小 
		size= atoi(abuf2);
		
		for(j=0;j<3;j++)
			abuf2[j]=abuf[18+j];//第一个文件数据区的物理块号 
		node= atoi(abuf2);
	}
	else{
		size=fcb[file[u_ofile[fp]].f_fcb].size;
		node=fcb[file[u_ofile[fp]].f_fcb].firstnode;
	}
	
	
		offset=file[u_ofile[fp]].f_roffset; //找到读指针 
		blonum=offset/512; //读过的总块数 
		bytenum=offset%512;//读过的最后一个物理块数空闲的开始地方 
		 
		while(blonum>0){//读的超过一块 
			node=FATTABLE[node].nextbl;
			blonum--;  //找到读过的最后一块 
		}
		
		address=(node+10)*BLOCKSIZE+bytenum;//找到读指针的再文件中的地址 

	
		rest=size-offset;//文件的实际大小 减去读指针位置 ，即可读的部分 
	
		if(n>rest){
			blonum=rest/512;
			bytenum=rest%512;
		}
		else{
			blonum=n/BLOCKSIZE;
			bytenum=n%BLOCKSIZE;
			rest=n;
		}
		
		fseek(fd,address,SEEK_SET);
		
		if(blonum>0){ //每次读Bloksize 
			fread(abuf,sizeof(char),BLOCKSIZE,fd);
			strcpy(data,abuf);//读了一次，肯定跳过当前块 
			blonum--;
		}
		
		//？？？？？？两个实现一样 
		while(blonum>0){
			memset(abuf,'\0',sizeof(char)*512);
			fread(abuf,sizeof(char),BLOCKSIZE,fd);
			strcat(data,abuf);
			node=FATTABLE[node].nextbl;
			address=(node+10)*BLOCKSIZE+bytenum;
			blonum--;
		}
		
		memset(abuf,'\0',sizeof(char)*512);
		fread(abuf,sizeof(char),bytenum,fd);//读取剩余的 
		strcat(data,abuf);
		strcpy(abuff,data);
		file[u_ofile[fp]].f_roffset+=rest;
		fclose(fd);
		return rest;
}


int Fwrite(int fp,char *abuff,int n){
	if(file[u_ofile[fp]].f_flag!=1)//打开文件没有写权限
		return -1;
		
	int address,i,j,size,offset,node,blonum,bytenum;
	int abuffstart,writenum;
	char *data;
	char abuf[1025]="0",abuf2[10]="0";
	fd = fopen("模拟磁盘文件.txt", "r+");
	data=(char *)malloc(n*sizeof(char));//动态申请空间 
	memset(data,'\0',sizeof(char)*n);//复制空 

	if(systate){
		address=file[u_ofile[fp]].f_fcb;//找到文件的fcb信息 
		fseek(fd,address,SEEK_SET);
		fread(abuf,sizeof(char),32,fd);
		
		for(j=0;j<8;j++)
			abuf2[j]=abuf[10+j];//文件大小 
		size= atoi(abuf2);
		
		for(j=0;j<3;j++)
			abuf2[j]=abuf[18+j];//文件第一个数据区物理块的位置 
		node= atoi(abuf2);
	}
	else{
		size=fcb[file[u_ofile[fp]].f_fcb].size;
		node=fcb[file[u_ofile[fp]].f_fcb].firstnode;
	}

	while(FATTABLE[node].nextbl!=0)
		node=FATTABLE[node].nextbl;//找到文件的最后一个块
		
	offset=file[u_ofile[fp]].f_woffset;//获取文件的写指针 
	
	blonum=offset/512;//有多少个块 
	bytenum=offset%512;//最后一块空余从哪开始 
	if(size!=0&&bytenum==0)			//文件最后块已占满，申请新块
		node=getblock(node);
		
	address=(node+10)*BLOCKSIZE+bytenum;  //获得最后的物理地址 
	fseek(fd,address,SEEK_SET);
	
	abuffstart=bytenum;
	if(BLOCKSIZE-bytenum>n)//一次可以将abuff中的数据都写到文件最后一个块的剩余部分
		fwrite(abuff,sizeof(char),strlen(abuff),fd);
	/*
	else					//一次不可以写完，先将块的剩余部分写完，再申

请新的块
		writenum=BLOCKSIZE-bytenum;
	for(i=bytenum;i<writenum;i++){
		fwrite("root    \0",sizeof(char),8,fd);
	}*/


	size+=n;				//完成abuff中的数据写到文件，修改fcb中的size

	if(systate){
		address=file[u_ofile[fp]].f_fcb;//fcb地址 
		fseek(fd,address+10,SEEK_SET);
		
		itoa(size,abuf2,10);//写入后的实际大小 
		
		if(strlen(abuf2)<=8){
			for(i=strlen(abuf2);i<8;i++)
				strcat(abuf2," ");
		}
		fwrite(abuf2,sizeof(char),8,fd);
	}
	else{	
		fcb[file[u_ofile[fp]].f_fcb].size+=n;
	}
	
	file[u_ofile[fp]].f_woffset+=n;//更改写指针 
	fclose(fd);
	return 0;
}

void Fclose(int fp){
	file[u_ofile[fp]].f_state=0;
	u_ofile[fp]=-1;
}
 
//去除尾部空格
char *rtrim(char *str){
	if (str == NULL || *str == '\0')
	{
		return str;
	}
 
	int len = strlen(str);
	char *p = str + len - 1;
	while (p >= str  && isspace(*p))
	{
		*p = '\0';
		--p;
	}
 
	return str;

}
