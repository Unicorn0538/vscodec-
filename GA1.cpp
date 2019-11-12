#include<iostream>
#include<string>
using namespace std;

//个体中基因的长度
//根据字符串的改变而改变，手动改变
#define Ds 21  

#define N 1000 //种群的个数
#define slc 0.6 //选择保留的个数
#define cr 0.75 //交叉的概率
#define Mr 0.1 //变异的概率
#define NG 5000 //迭代的次数

struct single  //定义单个句子
{
   char strs[Ds];  //句子
   int ACSII_strs[Ds]; //句子的ASCII值
   int fitness; //单个句子适应度
};
struct single group[N]; //定义句集

void Initial(struct single group[N],int Target[Ds]);  //生成初始种群
void Evolution(struct single group[N],int Target[Ds]); //迭代
void copy_single(struct single *p1,struct single *p2);//将p1中的数据复制到p2中
void sort(struct single group[N]); //按照fitness从小到大的顺序排序group中的个体
void cault_fitness(struct single *indevi, int Target[Ds]); //计算适应度
void copy_single(struct single *p1,struct single *p2);//将p1中的数据复制到p2中
void cross(struct single *p1, struct single *p2); //交配
void Vara(struct single group[N], int Target[Ds]); //生成变异个体
void varation(struct single group[N], int k, int Target[Ds] );//变异
void print_strs(struct single group[N], int k); //打印

int main()
{
    const string TARGET = "Hello World! 19/11/09";
    int Target[Ds];
    for(int i =0; i<Ds; i++)  //目标句子的ASCII值
       Target[i] = (int)TARGET[i];   

    Initial(group,Target); //生成原始数据集

    Evolution(group,Target); //选择，交换，变异

    system("pause");
    return 0;
    
}

void Initial(struct single group[N],int Target[Ds]) //生成句集
{
    for(int i=0;i<N;i++)
    {   
        for(int j=0; j<Ds; j++)
        {
            group[i].ACSII_strs[j]= 32 + rand()%(126-32+1);  //在ASCII值为32~126之间生成每个字符
            
        }
        cault_fitness(&group[i],Target);//计算适应值
    }
}

void  cault_fitness(struct single *idev, int Target[Ds]) //计算单个句子的适应值
{
    int fit_num = 0;
   for(int i=0;i<Ds;i++)
    {
        idev->strs[i] = idev->ACSII_strs[i]; //转换字符 
        if(idev->ACSII_strs[i] == Target[i]) fit_num++;
    }  
    idev->fitness = fit_num; 
}

void copy_single(struct single *p1,struct single *p2)//将p1中的数据复制到p2中
{
    for(int i = 0; i < Ds; i++)
    {
        p2->ACSII_strs[i] = p1->ACSII_strs[i];
        p2->strs[i] = p1->strs[i];
    }
    p2->fitness = p1->fitness;
}

void sort(struct single group[N]) //按照fitness从小到大的顺序排序group中的个体
{
    struct single temp, *p1, *p2;
    
    for(int j=0; j<N-1; j++)
    {
        bool flag = true;
        for(int i=1; i<N-j; i++)
        {
            p1 = &group[i-1];
            p2 = &group[i];
            if(p1->fitness > p2->fitness)
            {
               copy_single(p1, &temp);
               copy_single(p2, p1);
               copy_single(&temp, p2);
               flag = false;
            }
            
        }
        if(flag) break;
    }
}

void cross(struct single *p1, struct single *p2) //交配
{
    int N1 = rand()%Ds; //随机生成需交换的位数
    int tmp;
    for(int i=0; i<N1; i++)
    {
        tmp = p1->ACSII_strs[i];
        p1->ACSII_strs[i] = p2->ACSII_strs[i];
        p2->ACSII_strs[i] = tmp;
    }
}

void Vara(struct single group[N], int Target[Ds]) //生成变异个体
{
    int single_N = rand()%(N + 1); //变异的个体
    int DNA_N = rand()%(Ds +1); //变异的基因位数
    int DNA = rand()%(126-32+1)+32; //变异值
    group[single_N].ACSII_strs[DNA_N] = DNA; //变异
    cault_fitness(&group[single_N],Target);
}

void varation(struct single group[N], int k, int Target[Ds] )
{
    int j = rand()%100;
    if(k < N/10)  //初始时使N1个个体变异
    {
        int N1 = N * Mr;
        for(int i=0; i<N1; i++)
        {
          Vara(group,Target);
        }
    }
    else if(k < N/3 && k >= N/10) //中间时只变异一个个体
    {
        Vara(group,Target);
    }
    else if(k >= N/3 && j > 50) //结尾时减小变异率
    {
        Vara(group,Target);
    }
}

void print_strs(struct single group[N], int k)
{
     
    cout<<"Generation: "<<k<<" string: "<<group[N-1].strs<<endl;
   
}
bool judge(struct single group[N], int Target[Ds])
{
    bool flag = true;
    for(int i=0; i<Ds; i++)
        if(group[N-1].ACSII_strs[i] != Target[i])
        {
            flag = false;
            break;
        }

    return flag;
}
void Evolution(struct single group[N],int Target[Ds])//交换，突变，生成新的子类
{
    for(int k=0; k<NG; k++)
    {
        int N1 = N * (1-slc); //需要筛除的个体数目
        for(int i=1; i<N1;i++) //选择
        {
           copy_single(&group[N-i], &group[N1-i]);
        }

        int N2 = N * cr; //需要交叉的个体数目
        for(int i=0; i<N2; i+=2) //交叉
        {
            cross(&group[i+N-N2], &group[i+(N-N2)/2]);
            cault_fitness(&group[i+N-N2], Target);
            cault_fitness(&group[i+(N-N2)/2], Target);
        }
        
        varation(group,k,Target); //变异

        sort(group);  //排序
        
        print_strs(group,k); //输出最优解

         //判断是否找到
        if(judge(group,Target)) break; 
    }
}
