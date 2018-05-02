#include <stdio.h>
#include <unistd.h>

#define CRC_POLY   0x04c11db7

int genCRC(char *buf, int len) {
	int CRC = 0, i;
	short readMask;
	unsigned char mask;
	for(i = 0; i < len; i++) {
		for(readMask = 0x80; readMask; readMask >>= 1, mask = 0) {
			if(CRC & 0x80000000) mask = 1;
			CRC <<= 1;
			if(mask) CRC ^= CRC_POLY;
			if(buf[i] & readMask) {
				CRC ^= 1;
				putchar('1');
			}
			else putchar('0');
		}
		putchar(' ');
	}
	putchar('\n');
	return CRC;
}

main() {
	char tabuf[90];
	int i, j;
	int CRC;
	int unCRC;
	char buf[100];
	
	printf("Testing CRC generation.\n");
	for(i = 0; i < 90; i++)
		tabuf[i] = i;
	
	for(i = 0; i <= 90; i+=2) {
		memset(buf, 0, 100);
		for(j = 0; j < i; j++)
			buf[j] = tabuf[j];
		CRC = 0;
		memcpy(buf+i, &CRC, sizeof(int));
		CRC = genCRC(buf, i+4);
		printf("CRC is %08X.\n", CRC);
		//buf[i] = (CRC & 0xFF000000) >> 24;
		//buf[i+1] = (CRC & 0x00FF0000) >> 16;
		//buf[i+2] = (CRC & 0x0000FF00) >> 8;
		//buf[i+3] = CRC & 0x000000FF;
		CRC = htonl(CRC);
		memcpy(buf+i, &CRC, sizeof(int));
		unCRC = genCRC(buf, i+4);
		printf("unCRC is %08X.\n", unCRC);
		if(unCRC == 0)
			printf("Case %d passed.\n", i/2);
		else
			printf("Case %d failed.\n", i/2);
		usleep(100000);
	}
}
