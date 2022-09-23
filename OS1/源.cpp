#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
using namespace std;

typedef struct Process {//���̿��ƿ�Process
    char ProcessName[15];//������
    int ProcessArrivetime;//���̵���ʱ��
    int ProcessServicetime;//���̷���ʱ��
    int ProcessPriority;//���̳�ʼ����Ȩ
    int ProcessFinish;
    int ProcessCycle;
    struct Process* next;//���̼������ָ��

    enum P_s { x = 'W', y = 'R', z = 'F' } ProcessStation;//����״̬��Wait/Run/Finish
}Process;


int timeclock = 0;
const int priority = 2;//����Ȩ+2

void Init(Process* P, int n) {//�����û�����
    Process* t = P, * s;
    int i = n;
    cout << "******************��ʼ�������б��У����Ժ�******************" << endl;
    while (i--) {
        s = (Process*)malloc(sizeof(Process));
        cout << "������������� | ���̵���ʱ�� | ���̷���ʱ�� | ��������Ȩ��" << endl;
        cin >> s->ProcessName >> s->ProcessArrivetime >> s->ProcessServicetime >> s->ProcessPriority;
        s->ProcessStation = Process::x;
        s->next = NULL;
        t->next = s;
        t = t->next;
    }

    cout << "************************��ʼ�����************************" << endl;
    getchar();
    getchar();
    system("cls");//����
}


void Print(Process* P, Process* S, Process* F) {//��ʾ����
    Process* t = P->next;
    cout << "������������������������������������������������������������������������������������ʱ�̣�" << timeclock << endl;
    if (t != NULL)
    {
        cout << "��δ������̡���" << endl;
        cout << "������ | ����ʱ�� | ����ʱ�� | ����Ȩ | ����״̬" << endl;
        while (t != NULL)
        {
            cout << left << setw(14) << t->ProcessName << setw(10) << t->ProcessArrivetime << setw(10) << t->ProcessServicetime << setw(10) << t->ProcessPriority << setw(10) << (char)t->ProcessStation << endl;
            t = t->next;
        }
    }
    t = S->next;
    if (t != NULL)
    {
        cout << "���ѵ�����̡���" << endl;
        cout << "������ | ����ʱ�� | ����ʱ�� | ����Ȩ | ����״̬" << endl;
        while (t != NULL)
        {
            cout << left << setw(14) << t->ProcessName << setw(10) << t->ProcessArrivetime << setw(10) << t->ProcessServicetime << setw(10) << t->ProcessPriority << setw(10) << (char)t->ProcessStation << endl;
            t = t->next;
        }
    }//��ִ��һ��ʱ�䵥λ���������Ȩ�仯������ʱ��仯��״̬�仯

    t = F->next;
    if (t != NULL)
    {
        cout << "������ɽ��̡���" << endl;
        cout << "������ | ����ʱ�� | ����ʱ�� | ����Ȩ | ����״̬ | ���ʱ�� | ��תʱ��" << endl;
        while (t != NULL)
        {
            cout << left << setw(14) << t->ProcessName << setw(10) << t->ProcessArrivetime << setw(10) << '-' << setw(10) << '-' << setw(10) << (char)t->ProcessStation << setw(10) << t->ProcessFinish << setw(10) << t->ProcessCycle << endl;
            t = t->next;
        }
    }//������ʱ��Ϊ0ʱ��״̬��ΪFinish
    cout << endl;
}

void Sort(Process* P, Process* S) {//����������������Ȩ��������Ȩ��ͬʱ����ȴ�����ʱ����Ľ�����ǰ��
    //���µ���ĺ͸�ִ��������ȼ���ȣ����µ������ǰ���������ȶԳ���һ������Ľڵ������ڽ��µ���Ĳ��룬������ձ����ȹ���
    Process* t, * t_former, * former, * s_t, * s_n, * s_np, * tmp, * first;
    s_n = NULL;
    s_np = NULL;

    first = S->next;

    if (first != NULL && first->ProcessStation == 'R')
    {
        S->next = first->next; //����׽ڵ�
        first->ProcessStation = Process::x;
    }
    else
        first = NULL; //����׽ڵ���ܻ��ת���ڳ�ʼ״̬�����ж��Ƿ���>=1���ڵ㣬���ж��Ƿ���>=2���ڵ�
                     //ֱ���жϻ�Խ�磬<=1���ڵ�������״̬

    if (S->next != NULL && S->next->next != NULL)
    {
        s_np = S->next;
        s_n = s_np->next;
    }

    while (s_n != NULL)
    {

        former = S;
        s_t = former->next; //������ָ�븺����Ҫ�Ƚϣ�����
        s_np->next = s_n->next; //�Ȱγ�������λ��

        while (s_t != NULL) {

            if (s_n->ProcessPriority < s_t->ProcessPriority)

                break;

            s_t = s_t->next;
            former = former->next;
        }
        s_n->next = former->next;
        former->next = s_n;

        if (s_n == s_np->next) { //����˽ڵ��ֻص�ԭλ
            s_np = s_np->next;
            s_n = s_n->next;
        }

        else
            s_n = s_np->next;

    }

    t_former = P;//�µ���Ľڵ�
    t = t_former->next;
    while (t != NULL) {
        if (t->ProcessArrivetime == timeclock) {
            tmp = t;//���˽ڵ���
            t_former->next = tmp->next;
            t = t_former->next;
            former = S;//����
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

    former = S;//�׽ڵ㰲��
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


void Repeal(Process* S, Process* F) {//����״̬ΪFinish�Ľ���
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
//ģ��ʵ�ֶ�̬������Ȩ���ȣ�
void Schedule(Process* P, Process* S, Process* F) {//���Ⱥ�����ÿ�δӵȴ����ж��׵�������Ȩ��ߵĽ���ִ�У�״̬�仯

    //����ֵԽ������ȨԽ��,ÿ����һ��ʱ�䵥λ����Ȩ+n;����ֵԽС����ȨԽ�ߣ�ÿ����һ��ʱ�䵥λ����Ȩ-n
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


void AllQuit(Process* F) {//�ͷź���
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
    cout << "========================������Ȩ����========================" << endl;
    cout << "��������Ҫ���еĽ���������:" << endl;
    cin >> num;
    Init(P, num);//�����û�����
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
    cout << "************************����س�����************************";
    getchar();
    AllQuit(fin_proc);
    free(fin_proc); fin_proc = NULL;
    free(P);
    P = NULL;
    free(sort_proc);
    sort_proc = NULL;
    return 0;

}



