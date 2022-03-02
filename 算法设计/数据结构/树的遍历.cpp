#include<stdio.h>
#include<string.h>
#include<stdlib.h> 
/*
1:ab#cd##e##f#g##
2:ab#dg###ce#h##f##
3:-+a##*b##-c##d##/e##f##
*/
typedef struct BinTNode{ 
	char data;	
	struct BinTNode*lchild ,*rchild; 
}BinTNode, *BinTree;
 
typedef BinTNode *pNode;
 
BinTree u[100];
int head,tail;
int m[100];
 
BinTree CreateBiTree(BinTree T)
{

	char c;
	scanf(" %c",&c);
	
	if(c!='#')
	{
		T=(BinTree)malloc(sizeof(BinTNode));
		T->data=c;
		T->lchild=CreateBiTree(T);
		T->rchild=CreateBiTree(T);
	}
	else
	{
		T=NULL;
	}
	return T;
}
void FloorPrint(BinTree T)
{
	pNode temp[30];//����ָ������ 
	int in=0;
	int out=0;
	temp[in]=T;//������ڵ� 
	in++;
	while(in>out)
	 {
	 	if(temp[out])
	 	{
	 		printf("%c ",temp[out]->data);
	 		temp[in++]=temp[out]->lchild;
			temp[in++]=temp[out]->rchild; 
		 }
		 out++;
	  } 

}
int dfs(BinTree h,char p)//����·��
{
    for(int i=0;i<100;i++)
        m[i]=i;
    head=0;
    tail=0;
    memset(u,0,sizeof(u));
    u[tail++]=h;
    if(h->data==p)
        return 0;
    while(head!=tail)
    {
        BinTree t=u[head];
        if(t->lchild!=NULL)
        {
            u[tail++]=t->lchild;
            m[tail-1]=head;
            if(t->lchild->data==p)
                return tail-1;
        }
        if(t->rchild!=NULL)
        {
            u[tail++]=t->rchild;
            m[tail-1]=head;
            if(t->rchild->data==p)
                return tail-1;
        }
        head++;
    }
    return -1;
}
 void PreOrderTraverse(BinTree T)//ǰ����� 
{ 
	if(T)
	{
		printf("%c ",(T->data));
		PreOrderTraverse(T->lchild);
		PreOrderTraverse(T->rchild);
	} 
 } 
void InOrderTraverse(BinTree T)//�������
{
	if(T)
	{	 
		InOrderTraverse(T->lchild); 
		printf("%c ",T->data);
		InOrderTraverse(T->rchild);
	} 
} 
void PostOrderTraverse(BinTree T)//�������
{
	if(T)
	{ 
		PostOrderTraverse(T->lchild);
		PostOrderTraverse(T->rchild);
		printf("%c ",T->data);
	} 
}  
int main()
{
	int a,c=0,sum=0,o=0,i;
    char d;
    char k[100]={0};
    
	BinTree R=NULL; 

	while(1)
	{
		printf("1.�����������洢�ṹ\n");
		printf("2.����������������\n");
		printf("3.����������������\n");
		printf("4.��������ĺ������\n");
		printf("5.��������Ĳ�α���\n");
		printf("6.���������·��\n");
		printf("0.�˳�ϵͳ\n");
		scanf("%d",&a);
		switch(a)
		{
			case 1:
				printf("�����뵱ǰ�ڵ����ݣ�������������,�س��������ݣ�����#ʱ�ÿ�������\n");
				R=CreateBiTree(R);
				printf("�������ɹ���\n");
				break;	
			case 2:
				printf("ǰ����������\n");
				PreOrderTraverse(R);//����������������
				printf("\n");
				break;
			case 3:
				printf("������������\n");
				InOrderTraverse(R);//����������������
				printf("\n");
				break;
			case 4:
				printf("������������\n");
				PostOrderTraverse(R);//��������ĺ������
				printf("\n");
				break;
			case 5:
				printf("������������\n"); 
				FloorPrint(R);//������� 
				printf("\n");
				break;
			case 6:
				printf("������Ҫ���ҽ������:\n");
                scanf("%s",&d);
                o=dfs(R,d);
                if(o==-1)
                {
                    printf("û���ҵ�\n");
                    break;
                }
                printf("����·����\n");
                sum=0;
                while(o!=0)
                {
                    k[sum++]=u[o]->data;
                    o=m[o];
                }
                k[sum]=u[0]->data;
                for(i=sum;i>=0;i--)
                    printf("%c ",k[i]);
                printf("\n");
                break;
			case 0:
				exit(0);//�˳�ϵͳ 
			default:
				printf("�����������������루����\n");
				break;
		}getchar();
	}
	return 0;	
}
/*
// ǰ��������ݹ顤LC144_��������ǰ�����
class Solution {
    ArrayList<Integer> preOrderReverse(TreeNode root) {
        ArrayList<Integer> result = new ArrayList<Integer>();
        preOrder(root, result);
        return result;
    }

    void preOrder(TreeNode root, ArrayList<Integer> result) {
        if (root == null) {
            return;
        }
        result.add(root.val);           // ע����һ��
        preOrder(root.left, result);
        preOrder(root.right, result);
    }
}
// ����������ݹ顤LC94_���������������
class Solution {
    public List<Integer> inorderTraversal(TreeNode root) {
        List<Integer> res = new ArrayList<>();
        inorder(root, res);
        return res;
    }

    void inorder(TreeNode root, List<Integer> list) {
        if (root == null) {
            return;
        }
        inorder(root.left, list);
        list.add(root.val);             // ע����һ��
        inorder(root.right, list);
    }
}
// ����������ݹ顤LC145_�������ĺ������
class Solution {
    public List<Integer> postorderTraversal(TreeNode root) {
        List<Integer> res = new ArrayList<>();
        postorder(root, res);
        return res;
    }

    void postorder(TreeNode root, List<Integer> list) {
        if (root == null) {
            return;
        }
        postorder(root.left, list);
        postorder(root.right, list);
        list.add(root.val);             // ע����һ��
    }
}
*/
