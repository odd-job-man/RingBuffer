#include "RingBuffer.h"
#include <math.h>
#include <memory.h>

RingBuffer::RingBuffer(void)
{
	pBuffer_ = new char[BUFFER_SIZE + 1];
	front_ = rear_ = 0;
}

RingBuffer::~RingBuffer(void)
{
	delete pBuffer_;
}

int RingBuffer::GetUseSize(void)
{
	if (rear_ - front_ >= 0)
	{
		return rear_ - front_;
	}
	else
	{
		return BUFFER_SIZE - (front_ - rear_ - 1);
	}
}

int RingBuffer::GetFreeSize(void)
{
	return BUFFER_SIZE - GetUseSize();
}

int RingBuffer::Enqueue(IN char* pSource, IN size_t sizeToPut)
{
	int freeSize = GetFreeSize();	
	if (sizeToPut > freeSize)
	{
		// 반환하는 쪽에서는 연결을 끊어버려야함.
		return 0;
	}
	int DirectSize = DirectEnqueueSize();

	int firstSize = DirectSize > sizeToPut ? sizeToPut : DirectSize;
	int secondSize = sizeToPut - firstSize;

	memcpy_s(pBuffer_ + rear_ + 1, firstSize, pSource, firstSize);
	rear_ = (rear_ + firstSize) % (BUFFER_SIZE + 1);
	if (secondSize <= 0)
		return 1;
	pSource += firstSize;
	memcpy_s(pBuffer_, secondSize, pSource, secondSize);
	rear_ = (rear_ + secondSize) % (BUFFER_SIZE + 1);
	return 0;
}


int RingBuffer::Dequeue(OUT char* pDest, IN size_t sizeToRead)
{
	int useSize = GetUseSize();
	if (sizeToRead > useSize)
	{
		// 들어있는 데이터보다 많은 데이터를 읽으려고 하면 그냥 반환한다.
		return 0;
	}
	int directSize = DirectDequeueSize();
	int firstSize = directSize > sizeToRead ? sizeToRead : directSize;
	int secondSize = sizeToRead - firstSize;

	memcpy_s(pDest, firstSize, pBuffer_ + front_ + 1, firstSize);
	front_ = (front_ + firstSize) % (BUFFER_SIZE + 1);
	if (secondSize <= 0)
		return 1;
	pDest += firstSize;
	memcpy_s(pDest, secondSize, pBuffer_, secondSize);
	front_ = (front_ + secondSize) % (BUFFER_SIZE + 1);
	return 0;
}

void RingBuffer::ClearBuffer(void)
{
	rear_ = front_;
}

int RingBuffer::DirectEnqueueSize(void)
{
	if (rear_ >= front_)
	{
		return BUFFER_SIZE - rear_;
	}
	else
	{
		return front_ - rear_ - 1;
	}
}

int RingBuffer::DirectDequeueSize(void)
{
	if (rear_ >= front_)
	{
		return rear_ - front_;
	}
	else
	{
		return BUFFER_SIZE + 1 - front_ + rear_;
	}
}

int RingBuffer::MoveRear(IN int sizeToMove)
{
	return 1;
}


