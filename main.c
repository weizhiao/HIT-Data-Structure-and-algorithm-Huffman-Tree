#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct treenode
{
    int frequency;
    char data;
    struct treenode* parent;
    struct treenode* lchild;
    struct treenode* rchild;
};

struct minheap
{
    struct treenode* node[500];
    int n;
};

struct huffmant
{
    struct treenode*head;
    struct treenode node[200];
    int n;
};

struct codenode{
    char ch;
    char bits[50];
};

struct huffmancode{
    int n;
    struct codenode Code[100];
};

typedef struct huffmant* HuffmanT;
typedef struct minheap* MinHeap;
typedef struct treenode* Tree;
typedef struct huffmancode* HuffmanCode;

HuffmanT MakeTreeNull();
void ChangeHuffmanCodeToHuffmanT(HuffmanCode H,HuffmanT T);
void CreateHuffmanT(HuffmanT T);
void AddTreeNode(HuffmanT T,char ch);
void ReadFile(HuffmanT T);
void ReadHuffmanCode(HuffmanCode H,HuffmanT T);
MinHeap MakeHeapNull();
void InsertHeap(MinHeap heap,Tree node);
int IsHeapEmpty(MinHeap heap);
Tree DeleteMin(MinHeap heap);
HuffmanCode MakeHuffmanCodeNull();
void CharSetHuffmanEncoding(HuffmanT T,HuffmanCode H);
void SaveCodeTable(HuffmanCode H,HuffmanT T);
void PrintfCodeTable(HuffmanCode H);
void PrintAndCreateHuffmanFile(HuffmanCode H);
char* FindCodeFromCodeTable(HuffmanCode H,char ch);
void setOne(void *destinaton, int bits);
void setZero(void *destination, int bits);
void ReverseTxtFile(HuffmanCode H,HuffmanT T);
char* HuffmanCodeToChar(HuffmanT T,char*get_str);
void ShowMenu();

int main()
{
    HuffmanT T=MakeTreeNull();
    HuffmanCode H=MakeHuffmanCodeNull();
    while(1){
        ShowMenu();
        int c;
        while(scanf("%d",&c)!=1||c>3||c<0){
            printf("输入错误，请重新输入！\n");
            fflush(stdin);
        }
        switch(c){
    case 0:
        system("pause");
        return 0;
    case 1:
        ReadFile(T);
        printf("哈夫曼编码表为：\n");
        CreateHuffmanT(T);
        CharSetHuffmanEncoding(T,H);
        PrintfCodeTable(H);
        SaveCodeTable(H,T);
        PrintAndCreateHuffmanFile(H);
        system("pause");
        system("cls");
        break;
    case 2:
        ReadHuffmanCode(H,T);
        CreateHuffmanT(T);
        PrintfCodeTable(H);
        ReverseTxtFile(H,T);
        system("pause");
        system("cls");
        break;
        }
    }
}

void ShowMenu(){
    printf("##############################################\n");
    printf("##1、读入txt文件，创建哈夫曼编码表并编码文件##\n");
    printf("##2、读入哈夫曼编码表和bin文件，并还原文件  ##\n");
    printf("##0、退出程序                               ##\n");
    printf("##############################################\n");
}

HuffmanT MakeTreeNull()
{
    HuffmanT T=malloc(sizeof(struct huffmant));
    T->n=-1;
    T->head=NULL;
    return T;
}

void AddTreeNode(HuffmanT T,char ch)
{
    int flag=0;
    for(int i=0; i<=T->n; i++)
    {
        if(ch==T->node[i].data)
        {
            (T->node[i].frequency)++;
            flag=1;
            break;
        }
    }
    if(flag==0)
    {
        T->n++;
        T->node[T->n].data=ch;
        T->node[T->n].frequency=1;
        T->node[T->n].lchild=NULL;
        T->node[T->n].rchild=NULL;
        T->node[T->n].parent=NULL;
    }
}

void ReadHuffmanCode(HuffmanCode H,HuffmanT T){
    FILE* fp;
    fp=fopen("CodeTable.txt","r");
    fread(H,sizeof(struct huffmancode),1,fp);
    fread(T,sizeof(struct huffmant),1,fp);
    fclose(fp);
}

void ReadFile(HuffmanT T)
{
    FILE *fp;
    char ch;
    fp=fopen("target.txt","r");
    while((ch=fgetc(fp))!=EOF)
    {
        AddTreeNode(T,ch);
    }
    fclose(fp);
}

void CreateHuffmanT(HuffmanT T)
{
    MinHeap heap=MakeHeapNull();
    for(int i=0; i<=T->n; i++)
    {
        InsertHeap(heap,&T->node[i]);
    }
    for(int i=T->n+1; !IsHeapEmpty(heap); i++)
    {
        Tree p1=DeleteMin(heap);
        Tree p2=DeleteMin(heap);
        p1->parent=p2->parent=&T->node[i];
        T->node[i].lchild=p1;
        T->node[i].rchild=p2;
        T->node[i].frequency=p1->frequency+p2->frequency;
        if(!IsHeapEmpty(heap))
        {
            InsertHeap(heap,&T->node[i]);
        }
        else{
            T->head=&T->node[i];
        }
    }
}

MinHeap MakeHeapNull()
{
    MinHeap heap=malloc(sizeof(struct minheap));
    heap->n=0;
    return heap;
}

void InsertHeap(MinHeap heap,Tree node)
{
    (heap->n)++;
    int parent=heap->n/2;
    int child=heap->n;
    while(parent!=0&&node->frequency<heap->node[parent]->frequency)
    {
        heap->node[child]=heap->node[parent];
        child=parent;
        parent=parent/2;
    }
    heap->node[child]=node;
}

Tree DeleteMin(MinHeap heap)
{
    Tree MinNode=heap->node[1];
    Tree temp=heap->node[heap->n--];
    int parent=1;
    int child=2;
    while(child<=heap->n)
    {
        if(child<heap->n&&heap->node[child]->frequency>heap->node[child+1]->frequency)
        {
            child++;
        }
        if(temp->frequency<heap->node[child]->frequency)
            break;
        heap->node[parent]=heap->node[child];
        parent=child;
        child=parent*2;
    }
    heap->node[parent]=temp;
    return MinNode;
}

int IsHeapEmpty(MinHeap heap)
{
    if(heap->n==0)
        return 1;
    return 0;
}

HuffmanCode MakeHuffmanCodeNull(){
    HuffmanCode H=malloc(sizeof(struct huffmancode));
    H->n=-1;
    return H;
}

void CharSetHuffmanEncoding(HuffmanT T,HuffmanCode H){
    H->n=T->n;
    char temp[50];
    temp[49]='\0';
    for(int i=0;i<=T->n;i++){
        int start=49;
        H->Code[i].ch=T->node[i].data;
        Tree p=&T->node[i];
        while(p->parent!=NULL){
            if(p==p->parent->lchild){
                temp[--start]='0';
            }
            else{
                temp[--start]='1';
            }
            p=p->parent;
        }
        strcpy(H->Code[i].bits,&temp[start]);
    }
}

void PrintfCodeTable(HuffmanCode H){
    for(int i=0;i<=H->n;i++){
        printf("%c:",H->Code[i].ch);
        printf("%s\n",H->Code[i].bits);
    }
}

void SaveCodeTable(HuffmanCode H,HuffmanT T){
    FILE *fp;
    fp=fopen("CodeTable.txt","w");
    fwrite(H,sizeof(struct huffmancode),1,fp);
    fwrite(T,sizeof(struct huffmant),1,fp);
    fclose(fp);
}

char* FindCodeFromCodeTable(HuffmanCode H,char ch){
    for(int i=0;i<=H->n;i++){
        if(ch==H->Code[i].ch){
            return H->Code[i].bits;
        }
    }
    return NULL;
}

void PrintAndCreateHuffmanFile(HuffmanCode H){
    FILE *fp1;
    fp1=fopen("target.txt","r");
    FILE *fp2;
    fp2=fopen("HuffmanTFile.bin","wb");
    char ch;
    int i=0;
    char num[10000]={0};
    while((ch=fgetc(fp1))!=EOF){
        char *temp=FindCodeFromCodeTable(H,ch);
        printf("%s",temp);
        int j=i;
        for(;i<j+strlen(temp);i++){
            if(temp[i-j]=='1'){
                setOne(num,i);
            }
            else{
                setZero(num,i);
            }
        }
    }
    fclose(fp1);
    int left=8-i%8;
    fwrite(&left,1,1,fp2);
    int bits=i/8+(i/8?1:0);
    int count=0;
    while(1){
        fwrite(num+count,1,1,fp2);
        count++;
        if(count==bits) break;
    }
    fclose(fp2);
}

void setZero(void *destination, int bits) {
	char *des = (char *)destination;
	char zero[8] = { 0b01111111,0b10111111,0b11011111,0b11101111,0b11110111,0b11111011,
		0b11111101,0b11111110 };
	int bit = bits / 8;
	bits = bits % 8;
	des += bit;
	*des = *des&zero[bits];
}

void setOne(void *destinaton, int bits) {
	char *des = (char *)destinaton;
	char one[8] = { 0b10000000,0b01000000,0b00100000,0b00010000, 0b00001000,
		0b00000100, 0b00000010, 0b00000001 };
	int bit = bits / 8;
	bits = bits % 8;
	des += bit;
	*des = *des|one[bits];
}

char* HuffmanCodeToChar(HuffmanT T,char *get_str){
    char *s=malloc(sizeof(char)*10000);
    int i=0;
    int j=0;
    while(get_str[i]!='\0'){
        Tree p=T->head;
        while(p->lchild!=NULL&&p->rchild!=NULL){
            if(get_str[i]=='0'){
                p=p->lchild;
            }
            else{
                p=p->rchild;
            }
            i++;
        }
        s[j++]=p->data;
    }
    s[j]='\0';
    return s;
}

void ReverseTxtFile(HuffmanCode H,HuffmanT T){
    FILE *fp;
	int get_left;
	int get_bits = 0;
	char get_num[10000];
	fp = fopen("HuffmanTFile.bin","rb");
	fread(&get_left,1,1,fp);
	while(fread(get_num+get_bits,1,1,fp)==1){
		get_bits++;
	}
	fclose(fp);
	char get_str[200000]={0};
	int count = 0;
	char judge[] = { 0b10000000,0b01000000,0b00100000,0b00010000, 0b00001000,
		0b00000100, 0b00000010, 0b00000001};
	for (int i = 0; i < get_bits-1; i++) {
		for (int bit = 0; bit < 8; bit++) {
			get_str[count++] = ((get_num[i] & judge[bit]) == judge[bit]) ? '1' : '0';
		}
	}
	for (int bit = 0; bit < 8-get_left; bit++) {
			get_str[count++] = ((get_num[get_bits-1] & judge[bit]) == judge[bit]) ? '1' : '0';
	}
    get_str[count]='\0';
    char*s=HuffmanCodeToChar(T,get_str);
    printf("%s",s);
    FILE* fp1;
    fp1=fopen("ReverseFile.txt","w");
    fwrite(s,sizeof(char),strlen(s)+1,fp1);
    fclose(fp1);
}
