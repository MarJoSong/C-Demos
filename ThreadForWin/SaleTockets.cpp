//
// Created by Mars on 2020/9/17.
//
#include "SaleTickets.h"

extern HANDLE g_hMutex;

DWORD WINAPI SaleTicket(LPVOID lpParameter) {
    THD_DATA *pThreadData = (THD_DATA*)lpParameter;
    TICKET *pSaleData = pThreadData->pTicket;

    while (pSaleData->nCount > 0) {
        WaitForSingleObject(g_hMutex, INFINITE);
        if (pSaleData->nCount>0) {
            cout << "窗口" << pThreadData->saleWindow << "出票成功！余票" << --pSaleData->nCount << "张。" << endl;
        } else {
            cout << "窗口" << pThreadData->saleWindow << "停止售票！票已售完。" << endl;
        }
        ReleaseMutex(g_hMutex);
    }

    return 0L;
}
