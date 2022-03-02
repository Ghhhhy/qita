#include <stdlib.h>
#include <stdio.h>  
#include <malloc.h>  
#include <string.h>  
#include <time.h>
#include <math.h>

#include <ctype.h>

#define BLOCKSIZE 512  // ���̿��С 
#define Nofile 10  /*u_ofile��ɷ���ĸ���*/ 
#define Nfile 10  /*file��ɷ���ĸ���*/ 
#define Nfcb 32  /*pipecb�ɷ���ĸ���*/


struct 
{
   char filename[100];
   int  file_fd;   
}open_file[100]; 

/*u_file��*/ 
int u_ofile[Nofile]; 

/*file��*/ 
struct 
{ 
   int f_flag;/*�ɶ���д��־��0�ɶ���1�ɶ���д*/ 
   int f_state;/*��ʾ�˱����״̬��=0��ʾ�˱���û��ʹ�ã��ɷ��䣻=1��ʾ�˱����ڱ�*/
   int f_fcb;/*..�´��ļ����Ӧ��fcb���±꣬��Ŀ¼����Ŀ¼�´��ļ���Ӧ�ļ����ƿ�������ģ������ļ�.txt�еĵ�ַ*/ 
   long f_woffset;/*дָ�룬��д����*/
   long f_roffset;/*��ָ�룬�Ѷ�����*/ 
}file[Nfile];

/*�ļ����ƿ�*/ 
struct fcb
{
   char filename[9];
   char state;//���ñ�־,0��ʾ���ã�1��ʾ������
   char type; //�����ֶΣ�0��ʾĿ¼		�ļ���1��ʾ�����ļ�
   int size;//�ļ�����
   int firstnode;//�ļ�������ͷ���
	}fcb[Nfcb]; 

//Fat�ļ��������
struct FATTABLE
{
    char state;//���ñ�־,0��ʾ���ã�1��ʾ������
	int nextbl;//0��ʾ��ǰ��Ϊ���飬��Ϊ��ָʾ��һ��
}FATTABLE[1000];

//���浱ǰĿ¼��fcb
struct Curfcb		
{
   char filename[9];//                                                                  ǰ��λ�ļ����� 
   char state;//���ñ�־,0��ʾ���ã�1��ʾ������										      9
   char type; //�����ֶΣ�0��ʾĿ¼�ļ���1��ʾ�����ļ�                          		  10
   int size;//�ļ�����																	  ��λ 
   int firstnode;//�ļ�ͷ���															  ��λ 
   int index;//ָ���Ŀ¼�ļ�fcb��Ϣ����ĳһ�Ϊ-1��ʾ��ǰ������Ŀ¼��				  
   int address;//������Ŀ¼�ļ�fcb��Ϣ�ڴ����е�λ�ã����Ժ��޸�size��
}curfcb;

FILE *fd;
int address;	//��дģ����̼�ģ������ļ�.txt�ĵ�ַ
int systate=1;	//0��ǰ�ɴ�����Ŀ¼���ļ�;1��ǰ����һ����Ŀ¼��,�ɴ�����Ŀ¼���ļ�;ϵͳ������Ϊ1 

//Ϊ/root��Ŀ¼��
char curpath[30]="/root";	//���浱ǰĿ¼·��
char fileinit[]="110       ";//�����ļ�fcb�ĳ�ʼ��  1�Ѿ��ã� 1�ļ��� 0�ļ���С 
char dirinit[]="100       ";//Ŀ¼�ļ�fcb�ĳ�ʼ��   1�Ѿ��ã� 0Ŀ¼�� 0�ļ���С 


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

void welcome()//ϵͳ���滶ӭ����
{
	printf("*****************************************************************\n");
	printf("**\t\t <<��ӭ�����ļ�ϵͳ!>>                         **\n");
	printf("**\t\t     �ʺ�һС�����                            **\n");
	printf("*****************************************************************\n");
}
void showchoice()//ѡ����ʾ����
{
    printf("** ����Ϊ������ʾ��Ϣ:                                         **\n");
	printf("*****************************************************************\n");
	printf("**\tcreat_file  filename   :�����ļ�!                      **\n");
    printf("**\tcreat_dir  dirname     :����Ŀ¼!                      **\n");
	printf("**\tcd  dirname            :�л�Ŀ¼��                     **\n");
	printf("**\tquit  system           :�˳�ϵͳ��                     **\n");
    printf("**\tls  -a                 :�г�Ŀ¼/�ļ�!                 **\n");
	printf("**\topen  filename         :���ļ���                     **\n");
    printf("**\tread  filename         :��ȡ�ļ���                     **\n");
    printf("**\twrite  filename        :д�ļ���                       **\n");
    printf("**\tclose  filename        :�ر��ļ�!                      **\n");
    printf("**\tprint  FILE            :��ӡFILE��                   **\n");
    printf("**\tprint  FAT             :��ӡFAT��                    **\n");
    printf("**\tprint  FCB             :��ӡFCB��                    **\n");
    printf("**\tprint  OPEN_FIle       :��ӡ��ǰ�Ѵ��ļ���           **\n");
	printf("*****************************************************************\n");
}
int main()
{
	char filename[9],dirname[9]; //�м����  �ļ���/Ŀ¼�� 
	int flag,i,canshu,n;
	
	char abuf[100]="0";
	initialize();
	startsystem();
	
	int system=1; 
	welcome();
	showchoice(); 
	
	while(system){
		i = -1;
		printf("%s��",curpath);  //��������Ϊroot 
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
				printf("�����ļ��ɹ�\n");
				quit();
			}
			break;
		case 1:
			flag=addir(flag2);
			if(flag){
				printf("����Ŀ¼�ɹ�\n");
				quit();
			}
			break;
		case 2:
			flag=cd(flag2);
			if(flag){
				printf("����ɹ�\n");
				if(flag==1){
					strcpy(curpath,"/");
					strcat(curpath,flag2);
				}
				else{
					strcat(curpath,"/");  //flag=1  ��ʱΪroot/��Ŀ¼������ʽ 
					strcat(curpath,flag2);
				}
			}
			else printf("������������Ŀ¼��\n");
			break;
		case 3:
			system=0;
			printf("�˳��ɹ�\n");
			break;
		case 4:
			root();
			break;
		case 5:
			ls();
			break;
		case 6:
			printf("�����ļ��򿪷�ʽ(ֻ��:R,�ɶ���д:RW��:");  //�򿪵ķ�ʽ 
			char open_method[10];
			scanf("%s",open_method);
			if(strcmp(open_method, "R")==0)
			    canshu = 0;
			else
			    canshu = 1; 
			 
			if(canshu!=0&&canshu!=1)
				printf("�򿪷�ʽ�����ļ���ʧ��\n");
			else
				{
				char  filenametemp[20];
				strcpy(filenametemp,flag2);
				flag=Fopen(flag2,canshu);//����fd 
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
			if(u_ofile[flag]==-1){//�ж϶�ȡ�ļ��Ƿ񱻱��� 
				printf("�޴˴��ļ���ʶ��\n");
				break;
			}
			printf("�����ȡ���ݵ��ֽ���:");
			scanf("%d",&n);
			Fread(flag,abuf,n); //�ֽ�������ȡ���ݷ���abuf�� 
			printf("��������Ϊ��%s\n",abuf);
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
			if(u_ofile[flag]==-1){//�ж�д���ļ��Ƿ񱻱��� 
				printf("�޴˴��ļ���ʶ��\n");
				break;
			}
			printf("����Ҫд������:");
			getchar();
			gets(abuf);//flag�Ǵ��ļ������� 
			if(Fwrite(flag,abuf,strlen(abuf))==-1)
				printf("�Դ򿪵��ļ�û��дȨ�ޣ�\n");
			else {
				printf("д��ɹ�\n");
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
// ��ȡ��ǰĿ¼��root����fcb����Ϣ 
void startsystem()  
{
	int i,j;
	char abuf[32]=" ",abuf2[8]="0";
	fd = fopen("ģ������ļ�.txt", "r");
	address=3*BLOCKSIZE;
	fseek(fd,address,SEEK_SET);
	
	
	
	
	//��һ��Ӧ���Ǵ��ڴ���   ��ȡfat�����Ϣ 
	//��һ�����Ǻܶ�        ��fat����ģ�3+1����  ��ʼ��1000*��3+1�����ֽڸ�FATTABLE 
	for(i=0;i<1000;i++){
		fread(abuf,sizeof(char),1,fd);
		FATTABLE[i].state=abuf[0];
		fread(abuf,sizeof(char),3,fd);
		FATTABLE[i].nextbl= atoi(abuf);
	}
	
	
	//�ڶ��� 
	address=1*BLOCKSIZE;
	fseek(fd,address,SEEK_SET);
	
	
	//32�ֽ�  ��ȡroot ��fcb��Ϣ 
	fread(abuf,sizeof(char),32,fd);
	Setcurfcb(abuf); //��ǰĿ¼ΪrootĿ¼ 
	
	 
	 
	 
	 //��Ŀ¼��Ϊ��������� Ϊ512*2 = 32 * 32 
	
	//������32��fcb
	//fcb����Ϣ�����ٵڶ���  ��Ŀ¼�� 
//	ע��512/32 = 16 
	for(i=0;i<32;i++){
		for(j=0;j<8;j++)//�׸�abufΪroot��fcb 
			fcb[i].filename[j]=abuf[j];
		fcb[i].state=abuf[8];  //1 ��ʾ��ʹ���� 
		fcb[i].type=abuf[9];
		for(j=0;j<8;j++)
			abuf2[j]=abuf[10+j];
		fcb[i].size= atoi(abuf2);		
		for(j=0;j<3;j++)
			abuf2[j]=abuf[18+j];
		fcb[i].firstnode= atoi(abuf2);
		fread(abuf,sizeof(char),32,fd);  ///�ٶ�32���ֽ���Ϣ 
	}
	fclose(fd);
}


int getblock(int mode){//modeΪ0��ʾ�״�����飬��Ϊ0��ʾ��һ����
	int i;  //��Ϊ0 ��ʾ�������������� 	
	//989 �� ������ 
	for(i=0;i<1000;i++)  //��һ��Ϊ1 
		if(FATTABLE[i].state=='0')	break; 
	//��ʱ��Ȼ�ж����������  �������	 
	if(i<1000){
		FATTABLE[i].state='1';
		if(mode)	FATTABLE[mode].nextbl=i;     
		return i+1; //���صõ��������� 
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



//���õ�ǰ��curfcb 
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






























//ģ�����1000*512bit,��ʼ��ȫ0
void initialize(){
	int i;
	char s[512];
    //u_ofile���ʼ��ȫ��Ϊ-1��δʹ�� 
	for(i=0;i<Nofile;i++) 
       		u_ofile[i] = -1; 
    //file���ʼ��ȫ��Ϊ-1��δʹ�� 
    for(i=0;i<Nfile;i++){
       		file[i].f_state= -1;
			file[i].f_flag=-1;
			file[i].f_fcb=-1;
			file[i].f_woffset=-1;
			file[i].f_roffset=-1;
	}	
    //�ļ�������ʱ���Ż����if��䣬���г�ʼ�������ļ� 
	if((fd = fopen("ģ������ļ�.txt", "r")) == NULL){
		printf("����ģ����̳ɹ�\n");
	//�������ʼ��һ�������512�ֽ�Ϊ0	
	memset(s,'0',sizeof(char)*512);	
	//��ʼ�������ļ�����1000��������ʼ��Ϊ0 
	fd=fopen("ģ������ļ�.txt", "w");
	for(i=0;i<1000;i++)
		fwrite(s,sizeof(char),512,fd);	
	fclose(fd);	
	//ģ����̲�����ʱ����Ŀ¼ҲҪ������� 
	fd = fopen("ģ������ļ�.txt", "r+");
	address=1*BLOCKSIZE;
	fseek(fd,address,SEEK_SET); //�ض����ļ��Ŀ�ͷ 
	fwrite("root    \0",sizeof(char),8,fd);
	fwrite("1",sizeof(char),1,fd);
	fwrite("0",sizeof(char),1,fd);
	fwrite("0       ",sizeof(char),8,fd);
	fwrite("1",sizeof(char),3,fd);
	//������ fat�����1 ,��ʾ��һ������鱻ʹ�ã�����Ŀ¼ʹ���� 
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
	printf("Filename          |�ļ�������|\n");
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
	printf("����  |State |NextBlock|\n");
	for(int i=0;i<10;i++)
	{
		if( FATTABLE[i].state=='0')
		    break;
		printf("------------------------\n");
		
		printf("��%d�� |%-6s|%-9s|\n",i+1,"ʹ��","����"); 
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
		    strcpy(flag,"��");
		else
		    strcpy(flag,"��&д");
	    printf("----------------------------------------------\n"); 
	    printf("%-8d|%-5s|%-6s|%-6d|%-8d|%-7d|\n",u_ofile[i],flag,"ʹ��",file[i].f_fcb,file[i].f_woffset,file[i].f_roffset);
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
		    strcpy(flag,"Ŀ¼");
		else
		    strcpy(flag,"�ļ�");
	    printf("----------------------------------------\n"); 
	    printf("%-9s|%-6s|%-5s|%-5d|��%d��     |\n",fcb[i].filename,"ʹ��",flag,fcb[i].size,fcb[i].firstnode);
	}
	printf("----------------------------------------\n"); 
	return ;
	    
}

int Ff_d(char *name,int flag){//����Ŀ¼���ļ�
	int i,j;
	if(strlen(name)<=8){
		for(i=strlen(name);i<8;i++)
			strcat(name," ");
	}
	for(i=0;i<Nfcb;i++){
		if(fcb[i].state=='0'){
			for(j=0;j<8;j++)
				fcb[i].filename[j]=*(name+j);                        //��  �ĸ�����
			fcb[i].state='1';
			if(flag)
				fcb[i].type='1';//�����ļ� 
			else 
				fcb[i].type='0';//����Ŀ¼ 
			fcb[i].size=0;
			fcb[i].firstnode=getblock(0);
			break;
		}
	}
	if(i==Nfcb){
		printf("fcb��������������Ŀ¼���ļ�ʧ��\n");
		return 0;}
	return 1;
}

//������Ŀ¼�µ��ļ�����Ŀ¼
int Frootf_d(char *name,int flag){
	int i,size,offset,p,blocknum;
	char abuf[33]="0",a[3]="0",abuf2[9]="0";
	blocknum=getblock(0);	//�ȷ����,ʧ����������-1
	if(blocknum<0){
		printf("\n�����ʧ�ܣ�����ʧ��\n");
		return 0;
	}
	if(strlen(name)<=8){	//���������ո�
		for(i=strlen(name);i<8;i++)
			strcat(name," ");
	}
	size=curfcb.size;			//�޸ĵ�ǰfcb��curfcb��size��Ϣ�����ó�firstnode����
	//�����һ�����
	p=curfcb.firstnode;	
	while(FATTABLE[p].nextbl!=0)
		p=FATTABLE[p].nextbl;
	// ��ʱ p= ���һ�������
	//��ʱsizeΪʵ�ʵĴ�С  ������ ����fcb����˼         n*512% 512 = 0	
	offset=size%BLOCKSIZE;  //�ж�size��С�Ƿ��Ѿ����  һ�������Ĵ�С
	 //offsetΪ���һ�������ĳ�������   
	if(size!=0&&offset==0)	//Ŀ¼�ļ�������ռ���������¿�
		p=getblock(p);//p������ 
		
//    ���� fcb��size��С 
	curfcb.size+=32;
	fd=fopen("ģ������ļ�.txt", "r+");
	if(curfcb.index>=0)	 //Ϊ0�Ǹ�Ŀ¼ ����Ϊ��Ŀ¼ 
		//�޸ĵ�ǰfcb��curfcb��size��Ϣ����ǰĿ¼��Ϊ��Ŀ¼
		fcb[curfcb.index].size=curfcb.size;//��Ҫ���޸ĵ�size��Ϣд��ģ������ļ�.txt�ļ�
	else
	{
		address=curfcb.address;
		fseek(fd,address,SEEK_SET);
		fread(abuf,sizeof(char),32,fd);
		itoa(curfcb.size,abuf2,10);//ת����10���� 
		for(i=0;i<8;i++){
			if(i<strlen(abuf2))
				abuf[i+10]=abuf2[i];
			else
				abuf[i+10]=' ';
			}
		fseek(fd,address,SEEK_SET);
		fwrite(abuf,sizeof(char),32,fd);
	}
	
	
	address=(10+p)*BLOCKSIZE+offset;  //�ƶ�������������һ��λ��
	 
	fseek(fd,address,SEEK_SET);
	fwrite(name,sizeof(char),8,fd);//д���ļ���
 	
	if(flag) 
		fwrite(fileinit,sizeof(char),10,fd);
	else 
		fwrite(dirinit,sizeof(char),10,fd);	//���ļ�����Ŀ¼���Լ�������Ϣд�뵱ǰĿ¼�ļ���
	
	
	itoa(blocknum,a,10);//������Ŀ��Ҳд��
	fwrite(a,sizeof(char),3,fd);
	fclose(fd);
	return 1;
}


int Fcreat(char *filename){
	int flag;
	flag = check(filename,1);//����ļ��� 

	if(flag==0){ 
		printf("�ļ����ظ�\n");
		return 0;
	}
		 
	if(systate==0)  //��Ŀ¼�� 
		flag=Ff_d(filename,1);
		
	else if(systate==1)  //��Ŀ¼�� 
		flag=Frootf_d(filename,1);
	return flag;
	
}

int addir(char *dirname){
	int flag;
	flag=check(dirname,0); //���Ŀ¼�� 
	
	
	if(flag==0) 
		{printf("Ŀ¼���ظ�\n");
		 return 0;}
	if(systate==0)       //��Ŀ¼�´���Ŀ¼/�ļ� 
		flag=Ff_d(dirname,0);
	else if(systate==1)  //��Ŀ¼�´����ļ�/Ŀ¼ 
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
	
	//�ļ�/Ŀ¼���̶�8λ 
	if(strlen(dirname)<=8){
		for(i=strlen(dirname);i<8;i++)
			strcat(dirname," ");
	}
	
	if(systate){//��ǰΪ��Ŀ¼��
		p=curfcb.firstnode;
		size=curfcb.size;
		fd=fopen("ģ������ļ�.txt", "r");
		address=(10+p)*BLOCKSIZE;//�Ҹ�Ŀ¼�������� 
		fseek(fd,address,SEEK_SET);
		n=size/32;//�õ���Ŀ¼�µ�fcb��Ϣ     ��n��fcb 
		 
		while(n>0||FATTABLE[p].nextbl){
			if(n<32) num=n;
			
			for(i=0;i<num;i++){
				fread(abuf,sizeof(char),32,fd);
				if(abuf[9]=='1') continue; //a[9]=0ΪĿ¼��1Ϊ�ļ� 
				for(j=0;j<8;j++)
					if(abuf[j]!=dirname[j])//�ж�cd��Ŀ¼���Ƿ���ͬ 
						break;
				if (j==8){//Ŀ¼������ 
					Setcurfcb(abuf);   //�����µĵ�ǰcurfcb 
					curfcb.index=-1;//����Ϊ��Ŀ¼ 
					curfcb.address=(int)ftell(fd)-32; //���浱ǰ��Ŀ¼��fcb��Ϣ����Ŀ¼���������У� 
//					C �⺯�� long int ftell(FILE *stream) ���ظ����� stream �ĵ�ǰ�ļ�λ�á�
					n=0;
					break;
				}
			}
			//n-=32;
			p=FATTABLE[p].nextbl;//��ȡ��һ������� 
			address=(11+p)*BLOCKSIZE;
			fseek(fd,address,SEEK_SET);
		}
		if(n==0) return 2;  //Ŀ¼�ļ����ִ��� 
		else return 0;
		fclose(fd);
	}
	
	else {	//��ǰĿ¼Ϊ/..
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
	fd=fopen("ģ������ļ�.txt","r+");
	address=3*BLOCKSIZE;
	fseek(fd,address,SEEK_SET);
	for(i=0;i<10;i++){
		if(FATTABLE[i].state=='0')continue;
		abuf[0]=FATTABLE[i].state;
		itoa(FATTABLE[i].nextbl,abuf2,10);

		for(j=0;j<3;j++){
			if(j<strlen(abuf2))
				abuf[j+1]=abuf2[j];//���� 
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


int check(char *name,int flag){	//flagΪ0��ʾ���Ŀ¼����Ϊ1��ʾ����ļ���
	char abuf[33]="0",f;
	int p,size,n,i,j,address,num=16;
	
    if(strlen(name)<=8){
	for(i=strlen(name);i<8;i++)
			strcat(name," "); //�ļ����̶�Ϊ8λ�� 
    }
		
		
	if(systate){   //��Ŀ¼ 
		if(flag)f='0'; //���ļ��� 
		else f='1';//��Ŀ¼�� 
		
		p=curfcb.firstnode;//��һ��������   �������ĵ�һ������� 
		
		
		size=curfcb.size;//�ļ���С   ���ֽ�Ϊ��λ�� 
		
		fd=fopen("ģ������ļ�.txt", "r");
		address=(10+p)*BLOCKSIZE;    //��������12����鿪ʼ 
		fseek(fd,address,SEEK_SET);  //��ȡ��Ŀ¼�µ���Ŀ¼���ļ���fcb 
		
		
		n=size/32;  //n ��ʾn��fcb��Ϣ����Ϊһ��fcb��32���ֽ�
		// ע��  32/32 = 1��   �˺���n��fcb  ��n���ļ���Ŀ¼��Ϣ 
		
		//  32 * 16 = 512 
		//��һ�δ���wang���ļ�ʱ��n=0�� Fta��1��.nextbl=0 
		while(n>0||FATTABLE[p].nextbl){ //������������ֹһ������� 	
			if(n<16)  num=n;  //num��ֵΪ16 һ������鹲��16��fcb��Ϣ 
			for(i=0;i<num;i++){   // f = 0 ���ļ��� f = 1��Ŀ¼ 
				fread(abuf,sizeof(char),32,fd);
				//abuf��0��Ŀ¼�� 1���ļ� 
				if(abuf[9]==f)continue;     //f0=���ļ���1��Ŀ¼ 
				//�ļ���Ӧ�ļ�/Ŀ¼��ӦĿ¼�Ž��м�����ֳ�ͻ������ 
				for(j=0;j<8;j++){    //���ļ��������ֳ�ͻ���� 
					if(abuf[j]!=name[j])
						break;
					}
				if (j==8){
					fclose(fd);
					return 0;//���ظ���
					}
				}
			n-=16;  //�ҵ���һ������� 
			p=FATTABLE[p].nextbl;
			address=(11+p)*BLOCKSIZE;
			fseek(fd,address,SEEK_SET);
		}
	}
	
	
	else{   //��Ŀ¼ 
		for(i=0;i<Nfcb;i++){
			if(fcb[i].state=='1'&&strcmp(name,fcb[i].filename)==0)
				return 0;	
			}
		}
		
		 
		return 1;//���ظ���
}


int Fopen(char *filename,int flag){//0��ʾ�ɶ���1��ʾ�ɶ���д
	char abuf[33]="0",abuf2[10]="0";
	int p,size,n,i,j,address,num=32;
	if(strlen(filename)<=8){
		for(i=strlen(filename);i<8;i++)
			strcat(filename," ");
	}
	
	n=check(filename,1);
	if(n){
		printf("�ļ��������ڣ����ļ�ʧ��!\n");
		return -1;
	}
		
	//��Ŀ¼����Ŀ¼�´��ļ�
	if(systate){
			p=curfcb.firstnode;
			size=curfcb.size;
			fd=fopen("ģ������ļ�.txt", "r");
			address=(10+p)*BLOCKSIZE;
			fseek(fd,address,SEEK_SET);
			
			n=size/32;//n��fcb��Ϣ 
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
						size= atoi(abuf2); //�ļ��Ĵ�С 
						
						j=ftell(fd)-32;//�ҵ���/��Ŀ¼�´��ļ���fcb��Ϣ   ��fat��txt�е�׼ȷ��ַ 
						
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
	//��..Ŀ¼�´��ļ�
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
			file[i].f_flag=flag;//�򿪷�ʽ  1�ɶ���д  0ֻ�� 
			file[i].f_state=1;//ʹ�� 
			file[i].f_fcb=j;//�ļ����ƿ�ĵ�ַ 
			file[i].f_roffset=0;
			file[i].f_woffset=size;
			j=i;
			break;
			}
		}
	for(i=0;i<Nofile;i++)
		if(u_ofile[i]==-1){
			u_ofile[i]=j;//��¼file���±� 
			j=i;
			break;
		}
return j; //��¼���ļ��������� 
}


void ls(){
	
	printf("----------------\n");
	printf("Filename |Type |\n");
    printf("----------------\n");

	int i,p,size,n,num;
	char abuf[33]="0";
	if(systate){    //�տ���Ϊ1 
		p=curfcb.firstnode;
		size=curfcb.size;
		fd=fopen("ģ������ļ�.txt", "r");
		address=(10+p)*BLOCKSIZE;
		fseek(fd,address,SEEK_SET);
		n=size/32;
		while(n>0||FATTABLE[p].nextbl){
			if(n<32)num=n;
			for(i=0;i<num;i++){
				fread(abuf,sizeof(char),8,fd);
				printf("%-9s|",abuf);
				fread(abuf,sizeof(char),2,fd);
				if(abuf[1]=='1')printf("�ļ� |\n");
				else	printf("Ŀ¼ |\n");
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
				if(fcb[i].type=='0')printf("Ŀ¼ |\n");
				else printf("�ļ� |\n");
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
	fd = fopen("ģ������ļ�.txt", "r");
	
	data=(char *)malloc(n*sizeof(char));  //��ȡn������ 
	memset(data,'\0',sizeof(char)*n);     //�ַ���ĩβʱ���� 


	if(systate){
		address=file[u_ofile[fp]].f_fcb;//�ҵ�fcb��ַ 
		fseek(fd,address,SEEK_SET);
		fread(abuf,sizeof(char),32,fd);	
		
		for(j=0;j<8;j++)
			abuf2[j]=abuf[10+j];//�ҵ��ļ���С 
		size= atoi(abuf2);
		
		for(j=0;j<3;j++)
			abuf2[j]=abuf[18+j];//��һ���ļ��������������� 
		node= atoi(abuf2);
	}
	else{
		size=fcb[file[u_ofile[fp]].f_fcb].size;
		node=fcb[file[u_ofile[fp]].f_fcb].firstnode;
	}
	
	
		offset=file[u_ofile[fp]].f_roffset; //�ҵ���ָ�� 
		blonum=offset/512; //�������ܿ��� 
		bytenum=offset%512;//���������һ������������еĿ�ʼ�ط� 
		 
		while(blonum>0){//���ĳ���һ�� 
			node=FATTABLE[node].nextbl;
			blonum--;  //�ҵ����������һ�� 
		}
		
		address=(node+10)*BLOCKSIZE+bytenum;//�ҵ���ָ������ļ��еĵ�ַ 

	
		rest=size-offset;//�ļ���ʵ�ʴ�С ��ȥ��ָ��λ�� �����ɶ��Ĳ��� 
	
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
		
		if(blonum>0){ //ÿ�ζ�Bloksize 
			fread(abuf,sizeof(char),BLOCKSIZE,fd);
			strcpy(data,abuf);//����һ�Σ��϶�������ǰ�� 
			blonum--;
		}
		
		//����������������ʵ��һ�� 
		while(blonum>0){
			memset(abuf,'\0',sizeof(char)*512);
			fread(abuf,sizeof(char),BLOCKSIZE,fd);
			strcat(data,abuf);
			node=FATTABLE[node].nextbl;
			address=(node+10)*BLOCKSIZE+bytenum;
			blonum--;
		}
		
		memset(abuf,'\0',sizeof(char)*512);
		fread(abuf,sizeof(char),bytenum,fd);//��ȡʣ��� 
		strcat(data,abuf);
		strcpy(abuff,data);
		file[u_ofile[fp]].f_roffset+=rest;
		fclose(fd);
		return rest;
}


int Fwrite(int fp,char *abuff,int n){
	if(file[u_ofile[fp]].f_flag!=1)//���ļ�û��дȨ��
		return -1;
		
	int address,i,j,size,offset,node,blonum,bytenum;
	int abuffstart,writenum;
	char *data;
	char abuf[1025]="0",abuf2[10]="0";
	fd = fopen("ģ������ļ�.txt", "r+");
	data=(char *)malloc(n*sizeof(char));//��̬����ռ� 
	memset(data,'\0',sizeof(char)*n);//���ƿ� 

	if(systate){
		address=file[u_ofile[fp]].f_fcb;//�ҵ��ļ���fcb��Ϣ 
		fseek(fd,address,SEEK_SET);
		fread(abuf,sizeof(char),32,fd);
		
		for(j=0;j<8;j++)
			abuf2[j]=abuf[10+j];//�ļ���С 
		size= atoi(abuf2);
		
		for(j=0;j<3;j++)
			abuf2[j]=abuf[18+j];//�ļ���һ��������������λ�� 
		node= atoi(abuf2);
	}
	else{
		size=fcb[file[u_ofile[fp]].f_fcb].size;
		node=fcb[file[u_ofile[fp]].f_fcb].firstnode;
	}

	while(FATTABLE[node].nextbl!=0)
		node=FATTABLE[node].nextbl;//�ҵ��ļ������һ����
		
	offset=file[u_ofile[fp]].f_woffset;//��ȡ�ļ���дָ�� 
	
	blonum=offset/512;//�ж��ٸ��� 
	bytenum=offset%512;//���һ�������Ŀ�ʼ 
	if(size!=0&&bytenum==0)			//�ļ�������ռ���������¿�
		node=getblock(node);
		
	address=(node+10)*BLOCKSIZE+bytenum;  //������������ַ 
	fseek(fd,address,SEEK_SET);
	
	abuffstart=bytenum;
	if(BLOCKSIZE-bytenum>n)//һ�ο��Խ�abuff�е����ݶ�д���ļ����һ�����ʣ�ಿ��
		fwrite(abuff,sizeof(char),strlen(abuff),fd);
	/*
	else					//һ�β�����д�꣬�Ƚ����ʣ�ಿ��д�꣬����

���µĿ�
		writenum=BLOCKSIZE-bytenum;
	for(i=bytenum;i<writenum;i++){
		fwrite("root    \0",sizeof(char),8,fd);
	}*/


	size+=n;				//���abuff�е�����д���ļ����޸�fcb�е�size

	if(systate){
		address=file[u_ofile[fp]].f_fcb;//fcb��ַ 
		fseek(fd,address+10,SEEK_SET);
		
		itoa(size,abuf2,10);//д����ʵ�ʴ�С 
		
		if(strlen(abuf2)<=8){
			for(i=strlen(abuf2);i<8;i++)
				strcat(abuf2," ");
		}
		fwrite(abuf2,sizeof(char),8,fd);
	}
	else{	
		fcb[file[u_ofile[fp]].f_fcb].size+=n;
	}
	
	file[u_ofile[fp]].f_woffset+=n;//����дָ�� 
	fclose(fd);
	return 0;
}

void Fclose(int fp){
	file[u_ofile[fp]].f_state=0;
	u_ofile[fp]=-1;
}
 
//ȥ��β���ո�
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
