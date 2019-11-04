#include <iostream>
#include <string>
#include <vector>
using namespace std;


class LCS {
public:
    int LongestCommonSubsequence(string text1, string text2) 
    {
        int len1 = text1.length();
        int len2 = text2.length();
        int longest_length, length[len1+1][len2+1];

        for(int i = 1; i <= len1; i++)
            length[i][0] = 0;

        for(int i = 1; i <= len2; i++)
            length[0][i] = 0;

        for(int i = 1; i <= len1; i++)
            for(int j = 1; j <= len2; j++)
            {
                if(text1[i-1] == text2[j-1]) 
                   length[i][j] = length[i-1][j-1] + 1;
                
                else if(length[i-1][j] >= length[i][j-1])
                   length[i][j] = length[i-1][j];
                
                else 
                   length[i][j] = length[i][j-1];
            }
        
        longest_length = length[len1][len2];
        return longest_length;
    }
};


class WIS {
public:
    int WeightedIntervalScheduling(vector<int> &startTime, vector<int> &endTime, vector<int> &profit) 
    {
        int maximum_profit;
        int size = startTime.size();
        int p[size+1],OPT[size+1];
        OPT[0] = 0;
        for(int i=0; i<=size; i++) p[i] = 0;

        sort(startTime, endTime, profit, size);

        for(int i=1; i <= size; i++)
        {
           if(startTime[i] >= endTime[i-1])p[i+1] = i;
           else {
               int j = i - 1;
               while(j > 0)
               {
                   if(startTime[i] >= endTime[--j])
                   {
                       p[i+1] = j + 1;
                       break;
                   }
               }
           }
        }

        for(int i=1; i<=size; i++)
        {
            if(OPT[p[i]] + profit[i-1] >= OPT[i-1])
               OPT[i] = OPT[p[i]] + profit[i-1];
            else OPT[i] = OPT[i-1];
        }

        maximum_profit = OPT[size];
        return maximum_profit;
    }

    void sort(vector<int> &startTime, vector<int> &endTime, vector<int> &profit, int size)
    {
        bool flag = true;
         for(int i = 0; i < size; i++)
         {
            flag = true; 
            for(int j = 0; j < size-i-1; j++)
            {
                if(endTime[j] > endTime[j+1])
                {
                    swap(startTime[j], startTime[j+1]);
                    swap(endTime[j], endTime[j+1]);
                    swap(profit[j], profit[j+1]);
                    flag = false;
                }
            }
            if(!flag) break;
         }
    }
};


class KP {
public:
    int KnapsackProblem(vector<int> &weight, vector<int> &value, int W) 
    {
        int maximum_value, size=value.size(); 
        int OPT[size+1][W+1]={0};
        for(int i=0; i<=size;i++)
        for(int j=0; j<=W;j++)OPT[i][j]=0;

        for(int i = 1; i <= size; i++)
            for(int j = 1; j <= W; j++)
            {
                if(weight[i-1] > j) OPT[i][j] = OPT[i-1][j];
                else if(OPT[i-1][j-weight[i-1]] + value[i-1] > OPT[i-1][j])
                         OPT[i][j] = OPT[i-1][j-weight[i-1]] + value[i-1];
                     else OPT[i][j] = OPT[i-1][j];
            }

        maximum_value = OPT[size][W];
        return maximum_value;
    }
};


// For testing
int main(int argc, const char * argv[]) {
    
    //*********** Test Longest Common Subsequence ***********//
    string text1 = "springtime";
    string text2 = "printing";
    LCS lcs = LCS();
    cout << "Test Longest Common Subsequence..." << endl;
    cout << lcs.LongestCommonSubsequence(text1, text2) << endl;
    cout << "The expected output: 6" << endl;
    cout << endl;
    
    //********** Test Weighted Interval Scheduling **********//
    vector<int> startTime = {1,2,3,4,6};
    vector<int> endTime = {3,5,9,6,9};
    vector<int> profit = {20,20,100,70,60};
    WIS wis = WIS();
    cout << "Test Weighted Interval Scheduling..." << endl;
    cout << wis.WeightedIntervalScheduling(startTime, endTime, profit) << endl;
    cout << "The expected output: 150" << endl;
    cout << endl;
    
    //*************** Test Knapsack Problem ****************//
    vector<int> weight = {1, 2, 5, 6, 7};
    vector<int> value = {1, 6, 18, 22, 28};
    int W = 11;
    KP kp = KP();
    cout << "Test Knapsack Problem..." << endl;
    cout << kp.KnapsackProblem(weight, value, W) << endl;
    cout << "The expected output: 40" << endl;
    cout << endl;
    
    system("pause");
    return 0;
}
