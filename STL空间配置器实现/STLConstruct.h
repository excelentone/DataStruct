#pragma once
#include "TypeTraits.h"
#include "IteratorTraits.h"

// 效率  通用型
template<class T1, class T2>
inline void Construct(T1* p, const T2& value)
{
	new(p) T1(value);
}

template<class T>
inline void Destroy(T* p)
{
	cout<<typeid(T).name()<<endl;
	if(p)
		p->~T();
}


// O(N)
// int 
// string-->
// 
template<class Iterator>
void _Destroy(Iterator first, Iterator last, TrueType)
{
	cout<<"O(1)"<<endl;
}

template<class Iterator>
void _Destroy(Iterator first, Iterator last, FalseType)
{
	cout<<"O(N)"<<endl;
	while(first != last)
	{
		Destroy(&(*first));
		first++;
	}
}

template<class Iterator>
void Destroy(Iterator first, Iterator last)
{
	TypeTraits<IteratorTraits<Iterator>::ValueType>::hasTrivialDestructor del;
	_Destroy(first, last, del);
}