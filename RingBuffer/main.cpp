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
char dequeueBuf[CONSOLE_WIDTH];
char peekBuf[CONSOLE_WIDTH];

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
	char testString[CONSOLE_WIDTH] = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345*";
	testString[_countof(testString) - 1] = '!';
	srand(6);
	while (true)
	{
		int randBufSize = rand() % 239;
		int enqueueSize = rb.Enqueue(testString, sizeof(testString));
		int remainBufSize = CONSOLE_WIDTH - randBufSize;
		char* pTemp = rb.pBuffer_ + rb.front_ + 1;
		/*
		while (pTemp != rb.pBuffer_ + rb.rear_)
		{
			if (!('0' <= *pTemp && *pTemp <= '9' || *pTemp == '!'))
				__debugbreak();
			if (pTemp + 1 == rb.pBuffer_ + BUFFER_SIZE)
				pTemp = rb.pBuffer_;
			else
				++pTemp;
		}
		*/
		int PeekSize = rb.Peek(randBufSize, peekBuf);
		//if (PeekSize != 0)
		//{
		//	for (int i = 0; i < randBufSize; ++i)
		//	{
		//		if (peekBuf[i] == '!' && peekBuf[i + 1] == '!')
		//		{
		//			__debugbreak();
		//		}
		//	}
		//}
		if (PeekSize > 0)
		{
			printArray(PeekSize, peekBuf);
			//if (peekBuf[0] == '!')
				//__debugbreak();
		}

		int dequeueSize = rb.Dequeue(dequeueBuf, randBufSize);
		//printf(dequeueBuf);
		/*
		for (int i = 0; i < randBufSize; ++i)
		{
			if (dequeueBuf[i] == '6' && dequeueBuf[i + 1] != '7')
			{
				__debugbreak();
			}
		}*/
//		printf(dequeueBuf);
		//printArray(randBufSize, dequeueBuf);
		memset(peekBuf, 0, sizeof(dequeueBuf));
		memset(dequeueBuf, 0, sizeof(dequeueBuf));
	}
}

