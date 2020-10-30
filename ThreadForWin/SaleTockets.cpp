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
            cout << "����" << pThreadData->saleWindow << "��Ʊ�ɹ�����Ʊ" << --pSaleData->nCount << "�š�" << endl;
        } else {
            cout << "����" << pThreadData->saleWindow << "ֹͣ��Ʊ��Ʊ�����ꡣ" << endl;
        }
        ReleaseMutex(g_hMutex);
    }

    return 0L;
}
