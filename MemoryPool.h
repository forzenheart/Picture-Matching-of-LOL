#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#define _DEF_INIT_SIZE 1024 * 1024 * 4

class CMemoryPool
{
	struct _NODE
	{
		_NODE* pre; //前面内存块地址
		_NODE* next; //后面内存块地址
		int size; //内存块大小
		int use; //内存块是(1)否(0)使用
	};

	int m_Size; //初始总大小
	_NODE* m_Memory; //内存池

	CMemoryPool(const CMemoryPool& that);
	CMemoryPool& operator = (const CMemoryPool& that);
public:
	CMemoryPool(int Size = _DEF_INIT_SIZE);
	~CMemoryPool();

	void* Allocate(int size);
	void Free(void* memory);
};

#endif