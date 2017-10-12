#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_CHAR_KINDS 128//�ַ��������ֵ����
#define MAX_NUM 1000//�ַ�����󳤶ȡ���
FILE *in, *ou;

typedef struct TreeNode
{
	int weight;
	int id;
	short isLeaf;
	char data;
	char bin;//0����1����
	struct TreeNode *parent;
	struct TreeNode *lChild, *rChild;
} TreeNode;

typedef struct
{
	char data;
	char code[MAX_CHAR_KINDS];
} Code;

//�ַ������á���
void ReverseStr(char *str)
{
    int i;
    char c;
    int length;
    length = strlen(str);
    for (i = 0; i < length / 2; i++)
    {
        c = str[length - 1 - i];
        str[length - 1 - i] = str[i];
        str[i] = c;
    }
}

void PreOrderTraverse(TreeNode *t)
{
    if ((t->rChild == NULL) && (t->lChild == NULL))
    {
        t->isLeaf = 1;
    }
    else
    {
        t->isLeaf = 0;
    }
    if (t->parent != NULL)
    {
        t->id = 2 * t->parent->id + (int)t->bin - 48;
    }
    else
    {
        t->id = 1;
        t->bin = ' ';
    }
    if (t->isLeaf == 1)
    {
        fprintf(ou, "%6d%5c%8d   '%c'\n", t->id, t->bin, t->isLeaf, t->data);
    }
    else
    {
        fprintf(ou, "%6d%5c%8d\n", t->id, t->bin, t->isLeaf);
    }
    if (t->lChild != NULL)
    {
        PreOrderTraverse(t->lChild);
    }
    if (t->rChild != NULL)
    {
        PreOrderTraverse(t->rChild);
    }
}

int main()
{
	char str[MAX_NUM];
	char pwd[MAX_NUM];
	TreeNode *HFTree;
	int i, j;
	int length;//�ַ������ȡ���
	int count;//��ͬ�ַ���������
	TreeNode *tree[MAX_CHAR_KINDS];//��ʼ�ļ���С������
	TreeNode *eachChar[MAX_CHAR_KINDS];
	TreeNode *temp, *p;
	Code *HFCode;
	int codeBit;
	short existed;

	//���룬��ʼ������
	printf("Input string to encode:\n");
	gets(str);
	printf("\n");
	length = strlen(str);
	count = 0;

	//��ʼͳ���ַ����и����ַ����ֵĴ�������
	for (i = 0; i < length; i++)
	{
		existed = 0;
		for (j = 0; j < count; j++)
		{
			if (str[i] == tree[j]->data)
			{
				tree[j]->weight++;
				existed = 1;
				break;
			}
		}
		//����������е��ַ����ø��º���װ����
		if (existed == 0)
		{
			tree[count] = (TreeNode *)malloc(sizeof(TreeNode));
			tree[count]->weight = 1;
			tree[count]->data = str[i];
			tree[count]->parent = NULL;
			tree[count]->lChild = NULL;
			tree[count]->rChild = NULL;
			eachChar[count] = tree[count];//���ݡ���
			count++;
		}
	}

	//�Ƿ����롣��
	if (count == 0)
	{
        printf("No char!\n");
        getch();
        return (0);
    }
    else if (count == 1)
    {
        printf("At least 2 kinds of char!\n");
        getch();
        return (0);
    }

	//ð�ݣ����򡣡�
	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - 1 - i; j++)
		{
			if (tree[j]->weight > tree[j+1]->weight)
			{
				temp = tree[j];
				tree[j] = tree[j+1];
				tree[j+1] = temp;
			}
		}
	}

	//����Huffman������
	for (i = 1; i < count; i++)
	{
        temp = (TreeNode *)malloc(sizeof(TreeNode));
        temp->lChild = tree[i-1];
        temp->rChild = tree[i];
        temp->parent = NULL;
        temp->weight = tree[i-1]->weight + tree[i]->weight;
        tree[i-1]->parent = temp;
        tree[i-1]->bin = '0';
        tree[i]->parent = temp;
        tree[i]->bin = '1';
        tree[i] = temp;
        for (j = i; j < count - 1; j++)
        {
            if (tree[j]->weight > tree[j+1]->weight)
            {
                temp = tree[j];
				tree[j] = tree[j+1];
				tree[j+1] = temp;
            }
            else
            {
                break;
            }
        }
	}
	HFTree = tree[count-1];

	//����Huffman Tree��HuffmanTree.txt�ļ��С���
	if ((ou = fopen("HuffmanTree.txt", "w")) == NULL)
	{
        printf("Cannot open file HuffmanTree.txt\n");
        getch();
        exit(0);
    }
    fprintf(ou, "%6s%5s%8s%6s\n", "NodeID", "Bin", "IsLeaf", "Data");
    PreOrderTraverse(HFTree);
    fclose(ou);

    //ð�ݺ�������ַ��Ĺ��������롣��
    for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - 1 - i; j++)
		{
			if (eachChar[j]->weight > eachChar[j+1]->weight)
			{
				temp = eachChar[j];
				eachChar[j] = eachChar[j+1];
				eachChar[j+1] = temp;
			}
		}
	}
    HFCode = (Code *)malloc(count * sizeof(Code));
    for (i = 0; i < count; i++)
    {
        temp = eachChar[i];
        HFCode[i].data = temp->data;
        codeBit = 0;
        while (temp->parent != NULL)
        {
            HFCode[i].code[codeBit] = temp->bin;
            temp = temp->parent;
            codeBit++;
        }
        HFCode[i].code[codeBit] = '\0';
        ReverseStr(HFCode[i].code);
    }

	//�����ͬʱ���浽HuffmanCode.txt�ļ����
	if ((ou = fopen("HuffmanCode.txt", "w")) == NULL)
	{
        printf("Cannot open file HuffmanCode.txt\n");
        getch();
        exit(0);
    }
	printf("%5s%8s%15s\n", "char", "count", "Huffman code");
	fprintf(ou, "%5s%8s%15s\n", "char", "count", "Huffman code");
	for (i = 0; i < count; i++)
	{
		printf("  '%c'%8d%15s\n", eachChar[i]->data, eachChar[i]->weight, HFCode[i].code);
		fprintf(ou, "  '%c'%8d%15s\n", eachChar[i]->data, eachChar[i]->weight, HFCode[i].code);
	}
	printf("\n");
	fclose(ou);

    //���롣��
    printf("Input string to decode(only including '0' & '1'):\n");
    gets(str);
    length = strlen(str);
    p = HFTree;
    j = 0;
    codeBit = 0;
    for (i = 0; i < length; i++)
    {
        if (str[i] == '0')
        {
            p = p->lChild;
        }
        else if (str[i] == '1')
        {
            p = p->rChild;
        }
        else
        {
            printf("\nInput illegal..\n");
            getch();
            exit(0);
        }
        if ((p->lChild == NULL) && (p->lChild == NULL))
        {
            pwd[codeBit] = p->data;
            p = HFTree;
            codeBit++;
        }
    }
    pwd[codeBit] = '\0';
    printf("\nThe code is: \"%s\"\n", pwd);

    getch();
	return (0);
}
