#ifndef _MEMORY_POOL_H_
#define _MEMORY_POOL_H_

#define _DEF_INIT_SIZE 1024 * 1024 * 4

class CMemoryPool
{
	struct _NODE
	{
		_NODE* pre; //ǰ���ڴ���ַ
		_NODE* next; //�����ڴ���ַ
		int size; //�ڴ���С
		int use; //�ڴ����(1)��(0)ʹ��
	};

	int m_Size; //��ʼ�ܴ�С
	_NODE* m_Memory; //�ڴ��

	CMemoryPool(const CMemoryPool& that);
	CMemoryPool& operator = (const CMemoryPool& that);
public:
	CMemoryPool(int Size = _DEF_INIT_SIZE);
	~CMemoryPool();

	void* Allocate(int size);
	void Free(void* memory);
};

#endif