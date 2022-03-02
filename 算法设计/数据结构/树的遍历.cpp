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
	pNode temp[30];//创建指针数组 
	int in=0;
	int out=0;
	temp[in]=T;//保存根节点 
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
int dfs(BinTree h,char p)//查找路径
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
 void PreOrderTraverse(BinTree T)//前序遍历 
{ 
	if(T)
	{
		printf("%c ",(T->data));
		PreOrderTraverse(T->lchild);
		PreOrderTraverse(T->rchild);
	} 
 } 
void InOrderTraverse(BinTree T)//中序遍历
{
	if(T)
	{	 
		InOrderTraverse(T->lchild); 
		printf("%c ",T->data);
		InOrderTraverse(T->rchild);
	} 
} 
void PostOrderTraverse(BinTree T)//后序遍历
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
		printf("1.建立二叉树存储结构\n");
		printf("2.求二叉树的先序遍历\n");
		printf("3.求二叉树的中序遍历\n");
		printf("4.求二叉树的后序遍历\n");
		printf("5.求二叉树的层次遍历\n");
		printf("6.求给定结点的路径\n");
		printf("0.退出系统\n");
		scanf("%d",&a);
		switch(a)
		{
			case 1:
				printf("请输入当前节点数据：（请输入整数,回车推入数据，输入#时置空子树）\n");
				R=CreateBiTree(R);
				printf("树创建成功！\n");
				break;	
			case 2:
				printf("前序遍历结果：\n");
				PreOrderTraverse(R);//求二叉树的先序遍历
				printf("\n");
				break;
			case 3:
				printf("中序遍历结果：\n");
				InOrderTraverse(R);//求二叉树的中序遍历
				printf("\n");
				break;
			case 4:
				printf("后序遍历结果：\n");
				PostOrderTraverse(R);//求二叉树的后序遍历
				printf("\n");
				break;
			case 5:
				printf("层序遍历结果：\n"); 
				FloorPrint(R);//层序遍历 
				printf("\n");
				break;
			case 6:
				printf("请输入要查找结点数据:\n");
                scanf("%s",&d);
                o=dfs(R,d);
                if(o==-1)
                {
                    printf("没有找到\n");
                    break;
                }
                printf("查找路径：\n");
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
				exit(0);//退出系统 
			default:
				printf("输入有误！请重新输入（）：\n");
				break;
		}getchar();
	}
	return 0;	
}
/*
// 前序遍历·递归·LC144_二叉树的前序遍历
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
        result.add(root.val);           // 注意这一句
        preOrder(root.left, result);
        preOrder(root.right, result);
    }
}
// 中序遍历·递归·LC94_二叉树的中序遍历
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
        list.add(root.val);             // 注意这一句
        inorder(root.right, list);
    }
}
// 后序遍历·递归·LC145_二叉树的后序遍历
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
        list.add(root.val);             // 注意这一句
    }
}
*/
