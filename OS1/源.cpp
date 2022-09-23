#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
using namespace std;

typedef struct Process {//进程控制块Process
    char ProcessName[15];//进程名
    int ProcessArrivetime;//进程到达时间
    int ProcessServicetime;//进程服务时间
    int ProcessPriority;//进程初始优先权
    int ProcessFinish;
    int ProcessCycle;
    struct Process* next;//进程间的链接指针

    enum P_s { x = 'W', y = 'R', z = 'F' } ProcessStation;//进程状态，Wait/Run/Finish
}Process;


int timeclock = 0;
const int priority = 2;//优先权+2

void Init(Process* P, int n) {//创建用户进程
    Process* t = P, * s;
    int i = n;
    cout << "******************初始化进程列表中，请稍后******************" << endl;
    while (i--) {
        s = (Process*)malloc(sizeof(Process));
        cout << "请输入进程名称 | 进程到达时间 | 进程服务时间 | 进程优先权：" << endl;
        cin >> s->ProcessName >> s->ProcessArrivetime >> s->ProcessServicetime >> s->ProcessPriority;
        s->ProcessStation = Process::x;
        s->next = NULL;
        t->next = s;
        t = t->next;
    }

    cout << "************************初始化完成************************" << endl;
    getchar();
    getchar();
    system("cls");//清屏
}


void Print(Process* P, Process* S, Process* F) {//显示函数
    Process* t = P->next;
    cout << "——————————————————————————————————————————时刻：" << timeclock << endl;
    if (t != NULL)
    {
        cout << "【未到达进程】：" << endl;
        cout << "进程名 | 到达时间 | 服务时间 | 优先权 | 进程状态" << endl;
        while (t != NULL)
        {
            cout << left << setw(14) << t->ProcessName << setw(10) << t->ProcessArrivetime << setw(10) << t->ProcessServicetime << setw(10) << t->ProcessPriority << setw(10) << (char)t->ProcessStation << endl;
            t = t->next;
        }
    }
    t = S->next;
    if (t != NULL)
    {
        cout << "【已到达进程】：" << endl;
        cout << "进程名 | 到达时间 | 服务时间 | 优先权 | 进程状态" << endl;
        while (t != NULL)
        {
            cout << left << setw(14) << t->ProcessName << setw(10) << t->ProcessArrivetime << setw(10) << t->ProcessServicetime << setw(10) << t->ProcessPriority << setw(10) << (char)t->ProcessStation << endl;
            t = t->next;
        }
    }//在执行一个时间单位后进程优先权变化，服务时间变化，状态变化

    t = F->next;
    if (t != NULL)
    {
        cout << "【已完成进程】：" << endl;
        cout << "进程名 | 到达时间 | 服务时间 | 优先权 | 进程状态 | 完成时间 | 周转时间" << endl;
        while (t != NULL)
        {
            cout << left << setw(14) << t->ProcessName << setw(10) << t->ProcessArrivetime << setw(10) << '-' << setw(10) << '-' << setw(10) << (char)t->ProcessStation << setw(10) << t->ProcessFinish << setw(10) << t->ProcessCycle << endl;
            t = t->next;
        }
    }//当服务时间为0时，状态变为Finish
    cout << endl;
}

void Sort(Process* P, Process* S) {//排序函数：按照优先权排序。优先权相同时进入等待队列时间早的进程在前。
    //若新到达的和刚执行完的优先级相等，则新到达的在前，所以首先对除第一个以外的节点排序，在将新到达的插入，最后插入刚被调度过的
    Process* t, * t_former, * former, * s_t, * s_n, * s_np, * tmp, * first;
    s_n = NULL;
    s_np = NULL;

    first = S->next;

    if (first != NULL && first->ProcessStation == 'R')
    {
        S->next = first->next; //抽出首节点
        first->ProcessStation = Process::x;
    }
    else
        first = NULL; //这个首节点可能会空转或处于初始状态，先判断是否有>=1个节点，再判断是否有>=2个节点
                     //直接判断会越界，<=1个节点是有序状态

    if (S->next != NULL && S->next->next != NULL)
    {
        s_np = S->next;
        s_n = s_np->next;
    }

    while (s_n != NULL)
    {

        former = S;
        s_t = former->next; //这两个指针负责需要比较，插入
        s_np->next = s_n->next; //先拔出，再找位置

        while (s_t != NULL) {

            if (s_n->ProcessPriority < s_t->ProcessPriority)

                break;

            s_t = s_t->next;
            former = former->next;
        }
        s_n->next = former->next;
        former->next = s_n;

        if (s_n == s_np->next) { //如果此节点又回到原位
            s_np = s_np->next;
            s_n = s_n->next;
        }

        else
            s_n = s_np->next;

    }

    t_former = P;//新到达的节点
    t = t_former->next;
    while (t != NULL) {
        if (t->ProcessArrivetime == timeclock) {
            tmp = t;//将此节点抽出
            t_former->next = tmp->next;
            t = t_former->next;
            former = S;//插入
            s_t = former->next;
            while (s_t != NULL)
            {
                if (tmp->ProcessPriority < s_t->ProcessPriority)
                    break;
                former = former->next;
                s_t = s_t->next;
            }
            tmp->next = former->next;
            former->next = tmp;
        }
        else
        {
            t = t->next;
            t_former = t_former->next;
        }
    }

    if (first == NULL)
        return;

    former = S;//首节点安放
    s_t = former->next;
    while (s_t != NULL)
    {
        if (first->ProcessPriority < s_t->ProcessPriority)
            break;
        former = former->next;
        s_t = s_t->next;
    }
    first->next = former->next;
    former->next = first;

}


void Repeal(Process* S, Process* F) {//撤销状态为Finish的进程
    Process* rep = S->next;
    S->next = S->next->next;
    rep->ProcessStation = Process::z;
    rep->ProcessFinish = timeclock + 1;
    rep->ProcessCycle = rep->ProcessFinish - rep->ProcessArrivetime;
    rep->next = NULL;
    Process* fin = F;
    while (fin->next != NULL)
        fin = fin->next;
    fin->next = rep;

}
//模拟实现动态高优先权优先：
void Schedule(Process* P, Process* S, Process* F) {//调度函数，每次从等待队列队首调度优先权最高的进程执行，状态变化

    //若数值越大优先权越高,每运行一个时间单位优先权+n;若数值越小优先权越高，每运行一个时间单位优先权-n
    if (S->next == NULL)
        return;
    Process* schedule = S->next;
    schedule->ProcessStation = Process::y;
    Print(P, S, F);
    schedule->ProcessServicetime--;
    schedule->ProcessPriority += priority;
    schedule = schedule->next;
    while (schedule != NULL)
    {
        schedule->ProcessPriority -= priority;
        schedule = schedule->next;

    }

    schedule = S->next;
    if (schedule->ProcessServicetime == 0)
        Repeal(S, F);
}


void AllQuit(Process* F) {//释放函数
    Process* quit, * t = F;
    while (t->next != NULL) {
        quit = t->next;
        t->next = quit->next;
        free(quit);
    }
}

int main()

{
    Process* P;
    P = (Process*)malloc(sizeof(Process));
    P->next = NULL;
    int num;
    cout << "========================高优先权优先========================" << endl;
    cout << "请输入需要运行的进程总数量:" << endl;
    cin >> num;
    Init(P, num);//创建用户进程
    Process* fin_proc, * sort_proc;
    sort_proc = (Process*)malloc(sizeof(Process));
    sort_proc->next = NULL;
    fin_proc = (Process*)malloc(sizeof(Process));
    fin_proc->next = NULL;
    while (P->next != NULL || sort_proc->next != NULL)
    {
        Sort(P, sort_proc);
        Schedule(P, sort_proc, fin_proc);
        timeclock++;
    }

    Print(P, sort_proc, fin_proc);
    cout << "************************输入回车结束************************";
    getchar();
    AllQuit(fin_proc);
    free(fin_proc); fin_proc = NULL;
    free(P);
    P = NULL;
    free(sort_proc);
    sort_proc = NULL;
    return 0;

}



