#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "RingBuffer.h"
#include <windows.h>
constexpr int CONSOLE_WIDTH = 237;

RingBuffer rb;
char dequeueBuf[CONSOLE_WIDTH + 2];
char peekBuf[CONSOLE_WIDTH + 2];

void printArray(int width, char* pBuf) {
    for (int i = 0; i < width; ++i) {
		printf("%c", pBuf[i]);
    }
}
int main()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	char testString[] = "@234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345*!";
	srand(6);
	while (true)
	{
		int randBufSize = rand() % 239;
		int enqueueSize = rb.Enqueue(testString, sizeof(testString) - 1);
		int remainBufSize = CONSOLE_WIDTH - randBufSize;
		char* pTemp = rb.pBuffer_ + rb.front_ + 1;
		memset(peekBuf, 0, sizeof(peekBuf));
		memset(dequeueBuf, 0, sizeof(dequeueBuf));
		int PeekSize = rb.Peek(randBufSize, peekBuf);
		//printf(peekBuf);
		int dequeueSize = rb.Dequeue(dequeueBuf, randBufSize);
		printf(dequeueBuf);
	}
}

