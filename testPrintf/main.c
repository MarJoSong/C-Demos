#include <stdio.h>
#include <string.h>

int main() {
    int num1 = 1;
    int width1 = 3;
    int num2 = 2;
    int width2 = 8;

    float x = 12.3456f;

    printf("%*d,%*d\n", width1, num1, width2, num2);

    printf("%2$0*1$d,%1$*2$d\n", width1, num1);

    printf("%3$*1$f,%3$*2$.3f\n", width2, 2, x);

    printf("%%%3$0*1$.3f,%3$*2$.*2$f\n", width2, width1, x);

    printf("%.0f\n", 1234567.89);
    printf("aa %a\n", 123456.789);

    printf("%e\n", (double)0);
    printf("1:%d\n", 0);
    printf("2:%.0d\n", 0);

    printf("%o\n", 123456);

    return 0;
}