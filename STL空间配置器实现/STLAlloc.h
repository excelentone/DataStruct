#pragma once
#include <string>
#include <iostream>
using namespace std;


#include <stdarg.h>

//#define _DEBUG_
static string GetFileName(const string& path)
{
	char ch = '/' ;
#ifdef _WIN32
	ch = '\\';
#endif

	size_t pos = path.rfind(ch);
	if(pos == string::npos)
		return path;
	else
		return path.substr(pos+1);
}


// ���ڵ���׷�ٵ�trace log
inline static void _trace_debug( const char * funcName,
	const char * fileName, int line, char* format, ...)
{
#ifdef _DEBUG_
	fprintf(stdout, "[%s:%d]%s", GetFileName(fileName).c_str(), line, funcName);

	// ����û���Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	va_end(args);
#endif
}

#define __TRACE_DEBUG(...) \
	_trace_debug(__FUNCDNAME__, __FILE__, __LINE__, __VA_ARGS__);


typedef void (*OOMHandle)();
// һ���ռ���������������ͷŴ���ڴ�
template<int inst>
class MallocAllocTemplate
{
public:
	static void* Allocate(size_t n)
	{
		void* res = malloc(n);
		__TRACE_DEBUG("һ���ռ����������룺%p-->%d\n",res, n);
		if(0 == res)
			return OOM_Malloc(n);

		return res;
	}

	static void DeAllocate(void* p, size_t size)
	{
		__TRACE_DEBUG("һ���ռ��������ͷţ�%p-->%d\n",p,size);
		free(p);
	}

	static void* Reallocate(void* p, size_t oldSize, size_t newSize)
	{
		void* res = realloc(p, newSize);
		if(0 == res)
			return OOM_Realloc(p, newSize);

		return res;
	}

	static void* OOM_Realloc(void*, size_t n)
	{
		__TRACE_DEBUG("ϵͳ�ռ䲻�㣬���ÿռ䲻�㴦�����\n");
		for (;;)
		{
			if(0 == _pOOMHanle)
				exit(1);
			_pOOMHanle();
			void* res = realloc(p, n);
			if(res)
				return res;
		}
	}

	static void* OOM_Malloc(size_t n)
	{
		for (;;)
		{
			if(0 == _pOOMHanle)
				exit(1);
			_pOOMHanle();
			void* res = malloc(n);
			__TRACE_DEBUG("һ���ռ���������%p-->%d\n",res, n);
				if(res)
					return res;
		}
	}

	static OOMHandle SetMallocHandle(OOMHandle f)
	{
		OOMHandle old = _pOOMHanle;
		_pOOMHanle = f;
		return old;
	}

	static OOMHandle _pOOMHanle;
};

template<int inst>
OOMHandle MallocAllocTemplate<inst>::_pOOMHanle = 0;

typedef MallocAllocTemplate<0> MallocAlloc;

// �����ռ�������
template <int inst>
class DefaultAllocTemplate
{
public:
	static void* Allocate(size_t size)
	{
		if(size > __MAX_BYTES)
		{
			return MallocAlloc::Allocate(size);
		}

		size_t index = FREELIST_INDEX(size);
		Obj* res = _freeList[index];
		if(NULL == res)
		{
			__TRACE_DEBUG("�����ռ����������룺freeList[%d]û�п��õ�С���ڴ�",index);
			return ReFill(ROUND_UP(size));
		}

		_freeList[index] = res->_FreeListLink;
		__TRACE_DEBUG("�����ռ����������룺freeList[%d]:%d--->%p\n",index,size, res);
		return res;
	}

	static void DeAllocate(void* p, size_t size)
	{
		if(size > __MAX_BYTES)
		{
			MallocAlloc::DeAllocate(p, size);
			return;
		}

		size_t index = FREELIST_INDEX(size);
		((Obj*)p)->_FreeListLink = _freeList[index];
		_freeList[index] = (Obj*)p;
	}

	static void* ReFill(size_t size)
	{
		size_t nObjs = 20;
		__TRACE_DEBUG("�����ռ����������룺ChunkAlloc��%d��%d�ֽڵ�С���ڴ�\n", nObjs, size);
		char* chunk = (char*)ChunkAlloc(size, nObjs);

		if(1 == nObjs)
		{
			__TRACE_DEBUG("�����ռ����������룺ChunkAlloc��1��%d�ֽڵ�С���ڴ�\n", size);
			return chunk;
		}

		// nObjs-1
		// ��nobjs-1��size��С���ڴ�
		size_t index = FREELIST_INDEX(size);
		__TRACE_DEBUG("�����ռ����������룺ChunkAlloc��%d��%d�ֽڵ�С���ڴ�\n",nObjs, size);
		__TRACE_DEBUG("�����ռ����������룺��һ�齻���û���%dС���ڴ�ҵ�freeList[%d]:%d", nObjs-1, index, (index+1)*__ALIGN);
		void*res = chunk;
		for (size_t i = 1; i < nObjs; ++i)
		{
			chunk += size;
			((Obj*)chunk)->_FreeListLink = _freeList[index];
			_freeList[index] = (Obj*)chunk;
		}

		return res;
	}

	static void* ChunkAlloc(size_t size, size_t& n)
	{
		size_t totalByte = n*size;
		size_t leftByte = _endFree - _startFree;

		if(leftByte >= totalByte)
		{
			__TRACE_DEBUG("�ڴ�ؿ����ṩ20��С���ڴ�\n");
			void* res = _startFree;
			_startFree += totalByte;
			return res;
		}
		else if(leftByte >= size)
		{
			n = leftByte/size;
			__TRACE_DEBUG("�ڴ�ؿ����ṩ%d��С���ڴ�\n", n);
			void* res = _startFree;
			_startFree = _startFree + n*size;
			return res;
		}
		else
		{
			__TRACE_DEBUG("�ڴ�ؿռ䲻��\n");
			// �����ڴ����ʣ�಻��һ��size��С���ڴ�
			if(leftByte > 0)
			{
				size_t index = FREELIST_INDEX(leftByte);
				((Obj*)_startFree)->_FreeListLink = _freeList[index];
				_freeList[index] = (Obj*)_startFree;
			}

			// �ڴ����ϵͳ��Ҫ�ռ�
			size_t get2Bytes = 2*totalByte + ROUND_UP(_HeapSize>>4);
			_startFree = (char*)malloc(get2Bytes);
			if(NULL == _startFree)  // ϵͳ�ռ䲻��
			{
				__TRACE_DEBUG("ϵͳ�ռ䲻�����������������Ҹ���Ŀռ�\n");
				size_t index = FREELIST_INDEX(size);
				for (; index < __MAX_BYTES/__ALIGN; ++index)
				{
					if(_freeList[index])
					{
						__TRACE_DEBUG("���������и���Ŀռ�freeList[%d]:%d\n", index, (index+1)*__ALIGN);
						_startFree = (char*)_freeList[index];
						_freeList[index] = _freeList[index]->_FreeListLink;
						_endFree = _startFree + (index+1)*__ALIGN;
						return ChunkAlloc(size, n);
					}
				}

				__TRACE_DEBUG("��������û�и���Ŀռ䣬��һ���ռ��������п�");
				/*_endFree = 0;*/ // 
				_startFree = (char*)MallocAlloc::Allocate(get2Bytes);
			}
			_HeapSize += get2Bytes;
			_endFree = _startFree + get2Bytes;
		}

		return ChunkAlloc(size, n);
	}

	static  size_t FREELIST_INDEX(size_t bytes)
	{
		return (((bytes) + __ALIGN-1)/__ALIGN - 1);
	}

	static size_t ROUND_UP(size_t bytes)
	{
		return (((bytes) + __ALIGN-1) & ~(__ALIGN - 1));
	}

	enum {__ALIGN = 8};
	enum {__MAX_BYTES = 128};
	enum {__NFREELISTS = __MAX_BYTES/__ALIGN};

	union Obj
	{
		Obj* _FreeListLink;
		char _clientData[1];
	};

	static Obj* _freeList[__NFREELISTS];
	static char* _startFree;
	static char* _endFree;
	static size_t _HeapSize;
};

template<int inst>
typename DefaultAllocTemplate<inst>::Obj* DefaultAllocTemplate<inst>::_freeList[__NFREELISTS] = {0};

template<int inst>
char* DefaultAllocTemplate<inst>::_startFree = 0;

template<int inst>
char* DefaultAllocTemplate<inst>::_endFree = 0;

template<int inst>
size_t DefaultAllocTemplate<inst>::_HeapSize = 0;


#ifdef USE_MALLOC
typedef MallocAlloc _Alloc;
#else
typedef DefaultAllocTemplate<0> _Alloc;
#endif

// STL--> 1��Ч��
template<class T, class Alloc>
class SimpleAlloc
{
public:
	static T* Allocate(size_t n)
	{
		return (0 == n)? 0:(T*)Alloc::Allocate(n*sizeof(T));
	}

	static T* Allocate(void)
	{
		return (T*)Alloc::Allocate(sizeof(T));
	}

	static void DeAllocate(T* p, size_t n)
	{
		if(p)
		{
			Alloc::DeAllocate(p, n*sizeof(T));
		}
	}

	static void DeAllocate(T* p)
	{
		if(p)
		{
			Alloc::DeAllocate(p, sizeof(T));
		}
	}
};