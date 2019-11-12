#include<iostream>
#include<string>
#include<cmath>
using namespace std;

#define Ds 16  //个体中基因的长度
#define N 100 //种群的个数
#define slc 0.6 //选择保留个数的概率
#define cr 0.9 //交叉的概率
#define Mr 0.2  //变异的概率
#define NG 5000 //迭代的次数
#define x_Start -1.0 //x的最小取值
#define x_finish 15.0 //x的最大取值
#define Effective_digit 0.1 //精确数

struct single  
{
   int Binary_x[Ds];
   double fitness;
};
struct single group[N]; //定义句集

void Initial(struct single group[N]);  //生成初始种群
void Evolution(struct single group[N]); //迭代
void copy_single(struct single *p1,struct single *p2);//将p1中的数据复制到p2中
void sort(struct single group[N]); //按照fitness从小到大的顺序排序group中的个体
void  cault_fitness_single(struct single *idev); //计算适应度
void copy_single(struct single *p1,struct single *p2);//将p1中的数据复制到p2中
void cross(struct single *p1, struct single *p2); //交配
void Vara(struct single group[N]); //生成变异个体
void varation(struct single group[N], int k );//变异
void print_x(struct single group[N], int k); //打印

int main()
{
    Initial(group); //生成原始数据集

    Evolution(group); //选择，交换，变异

    system("pause");
    return 0;
    
}

void Initial(struct single group[N]) //生成二进制编码
{
    for(int i=0;i<N;i++)
    {   
        for(int j=0; j<Ds; j++)
        {
            group[i].Binary_x[j]= rand()%2; 
        }
        cault_fitness_single(&group[i]);//计算适应值
    }
}

double get_x(struct single *idev) //将二进制编码转为浮点数
{
    int X_tmp = 0, sum = 0;
    for(int i=0; i<Ds; i++)
    {
       X_tmp += idev->Binary_x[i] * pow(2,i);
       sum += pow(2,i);
    }
    double x;
    x =  x_Start + X_tmp * ((x_finish - x_Start) / sum);
    return x;
}

void  cault_fitness_single(struct single *idev) 
{
    double x = get_x(idev);
    idev->fitness = x*sin(x);
}

void copy_single(struct single *p1,struct single *p2)//将p1中的数据复制到p2中
{
    for(int i = 0; i < Ds; i++)
        p2->Binary_x[i] = p1->Binary_x[i];
    p2->fitness = p1->fitness;
}

void sort(struct single group[N]) //按照fitness从大到小的顺序排序group中的个体
{
    struct single temp, *p1, *p2;
    
    for(int j=0; j<N-1; j++)
    {
        bool flag = true;
        for(int i=1; i<N-j; i++)
        {
            p1 = &group[i-1];
            p2 = &group[i];
            if(p1->fitness < p2->fitness)
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
        tmp = p1->Binary_x[i];
        p1->Binary_x[i] = p2->Binary_x[i];
        p2->Binary_x[i] = tmp;
    }
}

void Vara(struct single group[N]) //生成变异个体
{
    int single_N = rand()%(N + 1); //变异的个体
    int DNA_N = rand()%(Ds +1); //变异的基因位数
    int DNA = (single_N > N/2)?1:0; //变异值
    group[single_N].Binary_x[DNA_N] = DNA; //变异
    cault_fitness_single(&group[single_N]);
}

void varation(struct single group[N], int k)
{
    int j = rand()%100;
    if(k < N/10)  //初始时使N1个个体变异
    {
        int N1 = N * Mr;
        for(int i=0; i<N1; i++)
        {
          Vara(group);
        }
    }
    else if(k < N/3 && k >= N/10) //中间时只变异一个个体
    {
        Vara(group);
    }
    else if(k >= N/3 && j > 50) //结尾时减小变异率
    {
        Vara(group);
    }
}

void print_x(struct single group[N], int k)
{
     
    cout<<"Generation: "<<k<<" result: "<<get_x(&group[N-1])<<endl;
   
}
bool judge(struct single group[N])
{
    bool flag = false;
    double sum =0.0, average;
    for(int i=0; i<N; i++)
    {
        sum += group[i].fitness;
    }
    average = sum/N;
    if(fabs(average - group[N-1].fitness) < Effective_digit)flag = true;
    return flag;
}

void Evolution(struct single group[N])//交换，突变，生成新的子类
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
            cault_fitness_single(&group[i+N-N2]);
            cault_fitness_single(&group[i+(N-N2)/2]);
        }
        
        varation(group,k); //变异

        sort(group);  //排序
        
        print_x(group,k); //输出最优解
        
        //判断是否找到
        if(judge(group)) break; 
    }
}
