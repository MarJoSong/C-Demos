#include "stdafx.h"
#include "SaleTickets.h"

HANDLE g_hMutex = nullptr;     //互斥量

typedef struct __THREAD_DATA {
    int nMaxNum;
    char strThreadName[NAME_LINE];

    __THREAD_DATA() : nMaxNum(0) {
        memset(strThreadName, 0, NAME_LINE * sizeof(char));
    }
} THREAD_DATA;

DWORD WINAPI ThreadProc1(LPVOID lpParameter) {
    for (int i=0; i<5; i++) {
        cout << "子线程:i = " << i << endl;
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
        cout << "主线程:i = " << i << endl;
        Sleep(100);
    }
}

void Demo2() {
    g_hMutex = CreateMutex(nullptr, FALSE, nullptr);

    THREAD_DATA threadData1, threadData2;
    threadData1.nMaxNum = 5;
    strcpy_s(threadData1.strThreadName, NAME_LINE, "线程1");
    threadData2.nMaxNum = 10;
    strcpy_s(threadData2.strThreadName, NAME_LINE, "线程2");

    HANDLE hThread1 = CreateThread(nullptr, 0, ThreadProc2, &threadData1, 0, nullptr);
    HANDLE hThread2 = CreateThread(nullptr, 0, ThreadProc2, &threadData2, 0, nullptr);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    for (int i=0; i<5; ++i) {
        WaitForSingleObject(g_hMutex, INFINITE);
        cout << "主线程 === " << i << endl;
        Sleep(1000);
        ReleaseMutex(g_hMutex);
    }
}

void Demo3() {
    g_hMutex = CreateMutex(nullptr, FALSE, NULL);
    TICKET ticket;
    ticket.nCount = 100;
    strcpy(ticket.strTicketName, "杭州-->上海");

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
        cout << "窗口" << threadSale[i].saleWindow << "开始出售" << threadSale[i].pTicket->strTicketName << "的票" << endl;
        ReleaseMutex(g_hMutex);
        //CloseHandle(hThread[i]);//不可以在调用Wait前关闭句柄，否则wait不会阻塞
    }

    //等待所有线程结束再退出函数，因为threadSale开辟在栈空间，不加线程等待数据就会过期
    WaitForMultipleObjects(THREAD_NUM, hThread, true, INFINITE);
    endTime = clock();
    cout << "100张票全部售完，用时" << (double)(endTime-startTime) / CLOCKS_PER_SEC << "秒。" << endl;
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
