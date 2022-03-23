#include <iostream>

#define MaxNum 100                                // 最大进程数
double AverageWT_FCFS = 0, AverageWT_SJF = 0;     // 平均等待时间
double AverageTAT_FCFS = 0, AverageTAT_SJF = 0;   // 平均周转时间
double AverageWTAT_FCFS = 0, AverageWTAT_SJF = 0; // 平均带权周转时间
using namespace std;

// 进程结构体
typedef struct PROCESS
{
    int index;                    // 进程序号
    double ServiceTime;           // 服务时间
    double ArrivalTime;           // 到达时间
    double StartTime;             // 开始时间
    double FinishTime;            // 结束时间
    double TurnArroundTime;       // 周转时间
    double WaitTime;              // 等待时间
    double WeightTurnArroundTime; // 带权周转时间
} PRO[MaxNum];

// 输出各进程的到达时间和服务时间
void display_base(PRO PC, int n)
{
    cout << endl;
    cout << "Process Num\t"
         << "Arrival Time\t"
         << "CPU Burst" << endl;
    for (int t = 0; t < n; t++)
        cout << PC[t].index << "\t\t" << PC[t].ArrivalTime << "\t\t" << PC[t].ServiceTime << endl;
}

// 模拟整个调度过程，输出每个时刻的进程运行状态
void display_status(PRO PC, int n)
{
    cout << endl;
    cout << "Process Num\t"
         << "Start Time\t"
         << "End Time\t"
         << "Ready Queue" << endl;
    for (int t = 0; t < n; t++) // 循环输出每个进程的服务时间内 处在等待队列的进程 即到达时间在当前进程开始和结束时间之间的进程
    {
        cout << PC[t].index << "\t\t" << PC[t].StartTime << "\t\t" << PC[t].FinishTime << "\t\t";
        for (int q = t + 1; q < n; q++)
        {
            if (PC[q].ArrivalTime <= PC[t].FinishTime)
                cout << PC[q].index << " ";
        }
        cout << endl;
    }
}

// 输出各进程的周转时间、带权周转时间和等待时间
void display_time(PRO PC, int n)
{
    cout << endl;
    cout << "Process Num\t"
         << "Turnaround Time\t"
         << "Weighted Turnaround Time\t"
         << "Waiting Time" << endl;
    for (int t = 0; t < n; t++)
        cout << PC[t].index << "\t\t" << PC[t].TurnArroundTime << "\t\t" << PC[t].WeightTurnArroundTime << "\t\t\t\t" << PC[t].WaitTime << endl;
}

// 输出所有进程的的平均周转时间、带权平均周转时间和平均等待时间
void display_average()
{
    cout << endl;
    cout << "Algorithm\t"
         << "Average TAT\t"
         << "Average WTAT\t"
         << "Average WT" << endl;
    cout << "FCFS\t\t" << AverageTAT_FCFS << "\t\t" << AverageWTAT_FCFS << "\t\t" << AverageWT_FCFS << endl;
    cout << "SJF\t\t" << AverageTAT_SJF << "\t\t" << AverageWTAT_SJF << "\t\t" << AverageWT_SJF << endl;
}

// 按到达时间排序
void SortArrival(PRO &PC, int n)
{
    PROCESS temp;
    for (int i = 0; i < n; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
            if (PC[j].ArrivalTime < PC[min].ArrivalTime)
                min = j;
        temp = PC[i];
        PC[i] = PC[min];
        PC[min] = temp;
    }
}

// 计算各项时间 z为记号变量 用于区别FCFS和SJF
void CountTime(PRO &PC, int n, int z)
{
    PC[0].StartTime = PC[0].ArrivalTime;
    PC[0].FinishTime = PC[0].ArrivalTime + PC[0].ServiceTime;
    PC[0].TurnArroundTime = PC[0].FinishTime - PC[0].ArrivalTime;
    PC[0].WaitTime = 0;
    PC[0].WeightTurnArroundTime = PC[0].TurnArroundTime / PC[0].ServiceTime;
    double sumWT = PC[0].WaitTime, sumTAT = PC[0].TurnArroundTime, sumWTAT = PC[0].WeightTurnArroundTime;
    for (int m = 1; m < n; m++)
    {
        if (PC[m].ArrivalTime >= PC[m - 1].FinishTime)
        {
            PC[m].StartTime = PC[m].ArrivalTime;
            PC[m].WaitTime = 0;
        }
        else
        {
            PC[m].StartTime = PC[m - 1].FinishTime;
            PC[m].WaitTime = PC[m - 1].FinishTime - PC[m].ArrivalTime;
        }
        PC[m].FinishTime = PC[m].StartTime + PC[m].ServiceTime;
        PC[m].TurnArroundTime = PC[m].FinishTime - PC[m].ArrivalTime;
        PC[m].WeightTurnArroundTime = PC[m].TurnArroundTime / PC[m].ServiceTime;
        sumWT += PC[m].WaitTime;
        sumTAT += PC[m].TurnArroundTime;
        sumWTAT += PC[m].WeightTurnArroundTime;
    }
    if (z == 1) // 计算FCFS
    {
        AverageWT_FCFS = sumWT / n;
        AverageTAT_FCFS = sumTAT / n;
        AverageWTAT_FCFS = sumWTAT / n;
    }
    if (z == 2) // 计算SJF
    {
        AverageWT_SJF = sumWT / n;
        AverageTAT_SJF = sumTAT / n;
        AverageWTAT_SJF = sumWTAT / n;
    }
}

// FCFS算法
void FCFS(PRO &PC, int n)
{

    SortArrival(PC, n);    // 按到达时间进行排序
    CountTime(PC, n, 1);   // 计算各项时间
    display_status(PC, n); // 模拟整个调度过程，输出每个时刻的进程运行状态
    display_time(PC, n);   // 输出各进程的周转时间、带权周转时间和等待时间
}

// SJF算法
void SJF(PRO &PC, int n)
{
    SortArrival(PC, n); // 先按到达时间进行排序
    // 类比选择排序的思想 从第二个到达的进程开始 遍历当前进程后面的进程 如果有服务时间比当前最短服务时间更短的 则交换位置
    int End = PC[0].ArrivalTime + PC[0].ServiceTime; // 之前服务的结束时间
    int tmin = 1;                                    // 当前最短服务时间的索引
    PROCESS temp;
    for (int x = 1; x < n; x++)
    {
        for (int y = x + 1; y < n; y++)
        {
            if (PC[y].ArrivalTime <= End && PC[y].ServiceTime < PC[tmin].ServiceTime)
                tmin = y;
        }
        // 将当前进程与等待队列中服务时间最短的进程进行交换
        temp = PC[x];
        PC[x] = PC[tmin];
        PC[tmin] = temp;
        End += PC[x].ServiceTime;
    }
    CountTime(PC, n, 2);   // 计算各项时间
    display_status(PC, n); // 模拟整个调度过程，输出每个时刻的进程运行状态
    display_time(PC, n);   // 输出各进程的周转时间、带权周转时间和等待时间
}

int main()
{
    PRO pc;
    int n;
    cout << "请输入进程的数量：";
    cin >> n;
    cout << "请输入每个进程的到达时间和服务时间：" << endl;
    for (int i = 0; i < n; i++)
    {
        pc[i].index = i + 1;
        cin >> pc[i].ArrivalTime >> pc[i].ServiceTime;
    }
    display_base(pc, n);
    int choice;
    cout << endl
         << "输入0-输出平均时间并退出程序  输入1-FCFS  输入2-SJF" << endl
         << "请选择要执行的操作：";
    cin >> choice;
    while (choice != 0)
    {
        if (choice == 1)
            FCFS(pc, n);
        if (choice == 2)
            SJF(pc, n);
        cout << endl
             << "请输入接下来要执行的操作：";
        cin >> choice;
    }
    display_average();
    cout << endl;
    system("pause");
    return 0;
}