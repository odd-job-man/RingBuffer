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

// 큐에서 원하는 바이트 수만큼 추출하여 임의의 버퍼에 집어넣는 함수
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
	char testString[] = "@234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345*!";
	int len = strlen(testString);
	std::queue<char> q;
	srand((unsigned)time(nullptr));
	while (true)
	{
		memset(peekBuf, 0, sizeof(peekBuf));
		memset(dequeueBuf, 0, sizeof(dequeueBuf));
		memset(tempBuf, 0, sizeof(tempBuf));

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

