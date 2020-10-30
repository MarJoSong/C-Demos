#include "stdafx.h"
#include "SaleTickets.h"

HANDLE g_hMutex = nullptr;     //������

typedef struct __THREAD_DATA {
    int nMaxNum;
    char strThreadName[NAME_LINE];

    __THREAD_DATA() : nMaxNum(0) {
        memset(strThreadName, 0, NAME_LINE * sizeof(char));
    }
} THREAD_DATA;

DWORD WINAPI ThreadProc1(LPVOID lpParameter) {
    for (int i=0; i<5; i++) {
        cout << "���߳�:i = " << i << endl;
        Sleep(100);
    }
    return 0L;
}

DWORD WINAPI ThreadProc2(LPVOID lpParameter) {
    THREAD_DATA *pThreadData = (THREAD_DATA*)lpParameter;

    for (int i=0; i < pThreadData->nMaxNum; ++i) {
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << pThreadData->strThreadName << " --- " << i << endl;
        Sleep(100);
        ReleaseMutex(g_hMutex);
    }
    return 0L;
}

void Demo1() {
    HANDLE thread = CreateThread(nullptr, 0, ThreadProc1, nullptr, 0, nullptr);
    CloseHandle(thread);

    for (int i=0; i<5; i++) {
        cout << "���߳�:i = " << i << endl;
        Sleep(100);
    }
}

void Demo2() {
    g_hMutex = CreateMutex(nullptr, FALSE, nullptr);

    THREAD_DATA threadData1, threadData2;
    threadData1.nMaxNum = 5;
    strcpy_s(threadData1.strThreadName, NAME_LINE, "�߳�1");
    threadData2.nMaxNum = 10;
    strcpy_s(threadData2.strThreadName, NAME_LINE, "�߳�2");

    HANDLE hThread1 = CreateThread(nullptr, 0, ThreadProc2, &threadData1, 0, nullptr);
    HANDLE hThread2 = CreateThread(nullptr, 0, ThreadProc2, &threadData2, 0, nullptr);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    for (int i=0; i<5; ++i) {
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "���߳� === " << i << endl;
        Sleep(1000);
        ReleaseMutex(g_hMutex);
    }
}

void Demo3() {
    g_hMutex = CreateMutex(nullptr, FALSE, NULL);
    TICKET ticket;
    ticket.nCount = 100;
    strcpy(ticket.strTicketName, "����-->�Ϻ�");

    const int THREAD_NUM = 8;
    THD_DATA threadSale[THREAD_NUM];
    HANDLE hThread[THREAD_NUM];
    clock_t startTime, endTime;

    startTime = clock();
    for (int i=0; i<THREAD_NUM; i++) {
        threadSale[i].pTicket = &ticket;
        threadSale[i].saleWindow = i;

        hThread[i] = CreateThread(nullptr, 0, SaleTicket, &threadSale[i], 0, nullptr);
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "����" << threadSale[i].saleWindow << "��ʼ����" << threadSale[i].pTicket->strTicketName << "��Ʊ" << endl;
        ReleaseMutex(g_hMutex);
        //CloseHandle(hThread[i]);//�������ڵ���Waitǰ�رվ��������wait��������
    }

    //�ȴ������߳̽������˳���������ΪthreadSale������ջ�ռ䣬�����̵߳ȴ����ݾͻ����
    WaitForMultipleObjects(THREAD_NUM, hThread, true, INFINITE);
    endTime = clock();
    cout << "100��Ʊȫ�����꣬��ʱ" << (double)(endTime-startTime) / CLOCKS_PER_SEC << "�롣" << endl;
    for (auto x : hThread) {
        CloseHandle(x);
    }
}

int main() {
    //Demo1();
    //Demo2();
    Demo3();

    system("pause");
}
