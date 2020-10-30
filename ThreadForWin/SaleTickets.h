//
// Created by Mars on 2020/9/17.
//

#ifndef THREADFORWIN_SALETICKETS_H
#define THREADFORWIN_SALETICKETS_H

#include "stdafx.h"

#define NAME_LINE   40

typedef struct __TICKET {
    int nCount;
    char strTicketName[NAME_LINE];

    __TICKET() : nCount(0) {
        memset(strTicketName, 0, NAME_LINE * sizeof(char));
    }
}TICKET;

typedef struct __THD_DATA {
    TICKET* pTicket;
    int saleWindow;

    __THD_DATA() : pTicket(nullptr) {}
}THD_DATA;

DWORD WINAPI SaleTicket(LPVOID lpParameter);

#endif //THREADFORWIN_SALETICKETS_H
