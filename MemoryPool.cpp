#include "MemoryPool.h"
#include <stdlib.h>

#define _MEM_USE 1 //使用
#define _MEM_IDLE 0 //空闲

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
	//找到合适的内存块
	_NODE* p = m_Memory;
	for (; p; p = p->next)
	{
		if (p->size >= size + (int)sizeof(_NODE) && p->use == _MEM_IDLE)
			break;
	}

	//有合适的内存块
	if (p)
	{
		//找到要分配内存区的节点地址
		_NODE* q = (_NODE*)((char*)p + (sizeof(_NODE) + p->size) - (size + sizeof(_NODE)));

		//设置新节点相关
		q->pre = p;
		q->next = p->next;
		q->size = size;
		q->use = _MEM_USE;

		//设置新节点的前序节点相关
		p->next = q;
		p->size -= size + sizeof(_NODE);

		//设置新节点的后序节点相关
		if (q->next)
			q->next->pre = q;

 		return q + 1;
	}
	//无合适的内存块
	else
		return malloc(size);
}

void CMemoryPool::Free(void* memory)
{
	//找到memory所在的内存块
	_NODE* p = m_Memory;
	for (; p; p = p->next)
	{
		if (p + 1 == memory && p->use == _MEM_USE)
			break;
	}

	//p就是memory所属的内存块
	if (p)
	{
		//前有节点、后有节点
		if (p->pre && p->next)
		{
			//前用、后用
			if (p->pre->use == _MEM_USE && p->next->use == _MEM_USE)
				p->use = _MEM_IDLE;
			//前闲、后用
			else if (p->pre->use == _MEM_IDLE && p->next->use == _MEM_USE)
			{
				p->pre->next = p->next;
				p->pre->size += p->size + sizeof(_NODE);
				p->next->pre = p->pre;
			}
			//前用、后闲
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
		//前有节点、后无节点(最初节点的内存是不可能拿出去用的)
		else
		{
			//前用
			if (p->pre->use == _MEM_USE)
				p->use = _MEM_IDLE;
			//前闲
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