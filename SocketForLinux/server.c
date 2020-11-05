#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <strings.h>
#include <pthread.h>

#define MAXLINE 1024
#define NUM_THREADS 20

typedef struct __st_deal_conn {
    int *connectfd;
    struct sockaddr_in *p_cliaddr;
} st_deal_conn;

// 线程的运行函数
void* deal_connection(void* args)
{
    int n;
    char ip[MAXLINE];
    char buf[MAXLINE];
    st_deal_conn *p_deal_conn_st = (st_deal_conn *)args;

    while (1) {
        n = read(*p_deal_conn_st->connectfd, buf, MAXLINE);
        /*
         * char *inet_ntoa(struct in_addr in);
         * 将Internet主机地址(网络字节序)转换为IPv4点状十进制符号的字符串。
         * 该字符串在静态分配的缓冲区中返回，后续调用将覆盖该缓冲区。
         *
         * const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
         * 支持ipv6，转换结果保存在src
         */
        if (n<=0) {
            printf("Host [%s] lost connection.\n",
                   inet_ntop(PF_INET, &p_deal_conn_st->p_cliaddr->sin_addr, ip, MAXLINE));
            break;
        }
        printf("Read len [%d] bytes from host:[%s] on port:[%d]\n", n,
               inet_ntop(PF_INET, &p_deal_conn_st->p_cliaddr->sin_addr, ip, MAXLINE),
               ntohs(p_deal_conn_st->p_cliaddr->sin_port));
        for (int i = 0; i < n; i++) {
            buf[i] = toupper(buf[i]);
        }
        write(*p_deal_conn_st->connectfd, buf, n);
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    struct sockaddr_in servaddr;
    socklen_t servaddr_len;
    int listenfd;
    int flag, len = sizeof(int);

    listenfd = socket(PF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr_len = sizeof(servaddr);
    servaddr.sin_family = PF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(7692);

    if (-1== setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, len)) {
        printf("set port reuseable error.\n");
        exit(1);
    }

    bind(listenfd, (struct sockaddr *)&servaddr, servaddr_len);

    listen(listenfd, 20);

    int i=0;
    pthread_t deal_threads[NUM_THREADS];
    struct sockaddr_in *p_cliaddr;
    socklen_t *p_cliaddr_len;
    st_deal_conn *p_deal_conn_st;

    printf("Host:[%s] accepting connections on port:[%d]\n",
           inet_ntoa(servaddr.sin_addr), ntohs(servaddr.sin_port));
    while (i<NUM_THREADS) {
        int *connectfd = (int *)malloc(sizeof(int));
        p_cliaddr_len = (socklen_t *)malloc(sizeof(socklen_t));
        p_cliaddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
        //初始化及socklen_t长度赋初值是必须的，否则客户端信息取不到
        *p_cliaddr_len = sizeof(struct sockaddr_in);
        bzero(p_cliaddr, *p_cliaddr_len);

        *connectfd = accept(listenfd, (struct sockaddr *)p_cliaddr, p_cliaddr_len);
        p_deal_conn_st = (st_deal_conn *)malloc(sizeof(st_deal_conn));
        p_deal_conn_st->connectfd = connectfd;
        p_deal_conn_st->p_cliaddr = p_cliaddr;

        if (pthread_create(&deal_threads[i++], NULL, deal_connection, p_deal_conn_st)) {
            printf("create deal thread failed.\n");

            close(listenfd);
            exit(1);
        } else {
            printf("Host:[%s] request connection on port:[%d].\n",
                   inet_ntoa(p_cliaddr->sin_addr), ntohs(p_cliaddr->sin_port));
        }
    }

    return 0;
}