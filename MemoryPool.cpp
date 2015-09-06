#include "MemoryPool.h"
#include <stdlib.h>

#define _MEM_USE 1 //ʹ��
#define _MEM_IDLE 0 //����

CMemoryPool::CMemoryPool(const CMemoryPool& that)
{}
CMemoryPool& CMemoryPool::operator = (const CMemoryPool& that)
{return *this;}

CMemoryPool::CMemoryPool(int Size)
:
m_Size(Size),
m_Memory((_NODE*)malloc(m_Size))
{
	m_Memory->pre = 0;
	m_Memory->next = 0;
	m_Memory->size = m_Size - sizeof(_NODE);
	m_Memory->use = _MEM_IDLE;
}

CMemoryPool::~CMemoryPool()
{
	free(m_Memory);
}

void* CMemoryPool::Allocate(int size)
{
	//�ҵ����ʵ��ڴ��
	_NODE* p = m_Memory;
	for (; p; p = p->next)
	{
		if (p->size >= size + (int)sizeof(_NODE) && p->use == _MEM_IDLE)
			break;
	}

	//�к��ʵ��ڴ��
	if (p)
	{
		//�ҵ�Ҫ�����ڴ����Ľڵ��ַ
		_NODE* q = (_NODE*)((char*)p + (sizeof(_NODE) + p->size) - (size + sizeof(_NODE)));

		//�����½ڵ����
		q->pre = p;
		q->next = p->next;
		q->size = size;
		q->use = _MEM_USE;

		//�����½ڵ��ǰ��ڵ����
		p->next = q;
		p->size -= size + sizeof(_NODE);

		//�����½ڵ�ĺ���ڵ����
		if (q->next)
			q->next->pre = q;

 		return q + 1;
	}
	//�޺��ʵ��ڴ��
	else
		return malloc(size);
}

void CMemoryPool::Free(void* memory)
{
	//�ҵ�memory���ڵ��ڴ��
	_NODE* p = m_Memory;
	for (; p; p = p->next)
	{
		if (p + 1 == memory && p->use == _MEM_USE)
			break;
	}

	//p����memory�������ڴ��
	if (p)
	{
		//ǰ�нڵ㡢���нڵ�
		if (p->pre && p->next)
		{
			//ǰ�á�����
			if (p->pre->use == _MEM_USE && p->next->use == _MEM_USE)
				p->use = _MEM_IDLE;
			//ǰ�С�����
			else if (p->pre->use == _MEM_IDLE && p->next->use == _MEM_USE)
			{
				p->pre->next = p->next;
				p->pre->size += p->size + sizeof(_NODE);
				p->next->pre = p->pre;
			}
			//ǰ�á�����
			else if (p->pre->use == _MEM_USE && p->next->use == _MEM_IDLE)
			{
				p->use = _MEM_IDLE;
				p->size += p->next->size + sizeof(_NODE);
				p->next = p->next->next;
				if (p->next)
					p->next->pre = p;
			}
			else
			{
				p->pre->size += p->size + p->next->size + 2 * sizeof(_NODE);
				p->pre->next = p->next->next;
				if (p->pre->next)
					p->pre->next->pre = p->pre;
			}
		}
		//ǰ�нڵ㡢���޽ڵ�(����ڵ���ڴ��ǲ������ó�ȥ�õ�)
		else
		{
			//ǰ��
			if (p->pre->use == _MEM_USE)
				p->use = _MEM_IDLE;
			//ǰ��
			else
			{
				p->pre->next = p->next;
				p->pre->size += p->size + sizeof(_NODE);
			}
		}
	}
	else
		free(p);
}