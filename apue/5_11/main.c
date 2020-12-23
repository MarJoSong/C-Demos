#include "apue.h"

void    pr_stdio(const char *, FILE *);
int     is_unbuffered(FILE *);
int     is_linebuffered(FILE *);
int     buffer_size(FILE *);

int
main(void) {
    FILE *fp;

    fputs("enter any character\n", stdout);
    if (getchar() == EOF)
        err_sys("getchar error");
    fputs("one line to standard error\n", stderr);

    pr_stdio("stdin", stdin);
    pr_stdio("stdout", stdout);
    pr_stdio("stderr", stderr);
    return 0;
}

void
pr_stdio(const char *name, FILE *fp)
{
    printf("stream = %s, ", name);
    if (is_unbuffered(fp))
        printf("unbuffered");
    else if (is_linebuffered(fp))
        printf("line buffered");
    else
        printf("fully buffered");
    printf(", buffer size = %d\n", buffer_size(fp));
}

#if defined(_IO_UNBUFFERED)

int
is_unbuffered(FILE *fp)
{
    return (fp->_flags & _IO_UNBUFFERED);
}

int
is_linebuffered(FILE *fp)
{
    return (fp->_flags & _IO_LINE_BUF);
}

int
buffer_size(FILE *fp)
{
    return (fp->_IO_buf_end - fp->_IO_buf_base);
}

#elif defined(_SNBF)

int
is_unbuffered(FILE *fp)
{
    return (fp->_flags & __SNBF);
}

int
is_linebuffered(FILE *fp)
{
    return (fp->_flags & __SLBF);
}

int
buffer_size(FILE *fp)
{
    return (fp->_bf._size);
}

#elif defined(_IONBF)

int
is_unbuffered(FILE *fp)
{
    return (fp->_flags & _IONBF);
}

int
is_linebuffered(FILE *fp)
{
    return (fp->_flags & _IOLBF);
}

int
buffer_size(FILE *fp)
{
    return (fp->_IO_buf_end - fp->_IO_buf_base);
}

#else

#error unknown stdio implementation!

#endif
