#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include "RingBuffer.h"
#include <windows.h>
#include <queue>

constexpr int CONSOLE_WIDTH = 237;

RingBuffer rbTemp;
RingBuffer rb;
char dequeueBuf[CONSOLE_WIDTH + 2];
char peekBuf[10000 + 2];

char tempBuf[10000 + 2];

void printArray(int width, char* pBuf) {
    for (int i = 0; i < width; ++i) {
		printf("%c", pBuf[i]);
    }
}

// ť���� ���ϴ� ����Ʈ ����ŭ �����Ͽ� ������ ���ۿ� ����ִ� �Լ�
void extractBytes(std::queue<char>& myQueue, char* buffer, int bytes) {
	int index = 0;
	while (bytes > 0 && !myQueue.empty()) {
		buffer[index++] = myQueue.front();
		myQueue.pop();
		--bytes;
	}
}

int main()
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	//char testString[] = "@234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345*!";
	char testString[] = "48321748932174893027398229111749@@@@@@@@@@@@@@@@87248392749208472!!!!!!!!!!!!!!9834***********724092873489207482390470294@@@@@@@@@@@@";
	int len = strlen(testString);
	std::queue<char> q;
	srand(1);
	//srand((unsigned)time(nullptr));
	int i = 0;
	while (true)
	{
		//memset(dequeueBuf, 0, sizeof(dequeueBuf));
		//++i;
		//int enqueueSize = rb.Enqueue(testString, len);
		//int dequeueSize = rb.Dequeue(dequeueBuf, len);
		//printArray(len, dequeueBuf);
		memset(peekBuf, 0, sizeof(peekBuf));
		memset(tempBuf, 0, sizeof(tempBuf));
		memset(dequeueBuf, 0, sizeof(dequeueBuf));

		int enqueueSize = 0;
		int dequeueSize = 0;

		for (int i = 0; i < len; ++i)
		{
			q.push(testString[i]);
		}
		
		int qSize = q.size();
		int rbSize = rb.GetFreeSize();
		if (rbSize == 0)
		{
			dequeueSize = rb.Dequeue(dequeueBuf, rand() % rb.GetUseSize() + 1);
			printArray(dequeueSize, dequeueBuf);
			continue;
		}

		int RandomByteToExtract = rand() % (qSize > rbSize ? rbSize : qSize) + 1;

		int index = 0;
		extractBytes(q, tempBuf, RandomByteToExtract);
		enqueueSize = rb.Enqueue(tempBuf, RandomByteToExtract);
		dequeueSize = rb.Dequeue(dequeueBuf, rand() % RAND_MAX + 1);
		if(dequeueSize > 0)
			printArray(dequeueSize, dequeueBuf);

		if (q.size() > 100000)
		{
			memset(dequeueBuf, 0, sizeof(dequeueBuf));
			dequeueSize = rb.Dequeue(dequeueBuf, rb.GetUseSize());
			printArray(dequeueSize, dequeueBuf);
			while (q.size() > 0)
			{
				putchar(q.front());
				q.pop();
			}
		}
	}
}

