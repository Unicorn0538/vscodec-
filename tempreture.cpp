#include <iostream>
#include <iomanip>
using namespace std;

double ctok(double c);   //没有定义函数
int main()
{
    double c=0;
    cout<<"Celsius: ";
    cin>>c;              //变量错误
    double k = ctok(c);  //参数错误
    cout<<"Kelvin:"<< k <<'/n';  
    system("pause")   ;
    return 0;
}

double ctok(double c)   //参数类型错误
{
    double k = c + 273.5;
    return k;
}
