#include "../include/hexdump.h"
#include <stdio.h>

void hexdump(void *pSrc, int len)
{
	unsigned char *line;
	int i;
	int thisline;
	int offset;

	line = (unsigned char *)pSrc;
	offset = 0;

	while (offset < len) {
		printf("%04x ", offset);
		thisline = len - offset;

		if (thisline > 16)
			thisline = 16;

		for (i = 0; i < thisline; i++)
			printf("%02x ", line[i]);

		for (; i < 16; i++)
			printf("   ");

		for (i = 0; i < thisline; i++)
			printf("%c", (line[i] >= 0x20 && line[i] < 0x7f) ? line[i] : '.');

		printf("\n");
		offset += thisline;
		line += thisline;
	}
}
