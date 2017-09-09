#pragma once

struct InputIteratorTag
{};

struct OutputIteratorTag
{};

struct ForwardIteratorTag : public InputIteratorTag
{};

struct BidirectionalIteratorTag : public ForwardIteratorTag
{};

struct RandomAccessIteratorTag : public BidirectionalIteratorTag
{};

// Ч��-->��
//ListIterator<int>
template<class Iterator>
struct IteratorTraits
{
	typename typedef Iterator::ValueType ValueType;  // ���������
	typename typedef Iterator::DifferenceType DifferenceType; // ������֮��ľ���
	typename typedef Iterator::Reference Reference;  // ��������
	typename typedef Iterator::Pointer Pointer;      // ָ��
	typename typedef Iterator::IteratorCategory IteratorCategory;  // ���ֵ�����������
};


//string*
template<class T>
struct IteratorTraits<T*>
{
	typedef T ValueType;
	typedef T& Reference;
	typedef T* Pointer;
	typedef int DifferenceType;
	typedef RandomAccessIteratorTag IteratorCategory;
};

template<class T>
struct IteratorTraits<const T*>
{
	typedef T ValueType;
	typedef const T& Reference;
	typedef const T* Pointer;
	typedef int DifferenceType;
	typedef RandomAccessIteratorTag IteratorCategory;
};


// 1��List ���α���--->O(N)
// 2��vector--> last - first O(1)---> ���������
// ����ĵ�����
template<class Iterator>
int _Distance(Iterator first, Iterator last, RandomAccessIteratorTag)
{
	return last - first;
}

template<class InputIterator>
int _Distance(InputIterator first, InputIterator last, InputIteratorTag)
{
	int count = 0;
	while(first != last)
	{
		count++;
		++first;
	}

	return count;
}

template<class InputIterator>
int Distance(InputIterator first, InputIterator last)
{
	return _Distance(first, last, IteratorTraits<InputIterator>::IteratorCategory());
}


template<class Iterator>
class ReverseIterator
{
public:
	typename typedef Iterator::ValueType ValueType;  // ���������
	typename typedef Iterator::DifferenceType DifferenceType; // ������֮��ľ���
	typename typedef Iterator::Reference Reference;  // ��������
	typename typedef Iterator::Pointer Pointer;      // ָ��
	typename typedef Iterator::IteratorCategory IteratorCategory;  // ���ֵ�����������


	typedef ReverseIterator<Iterator> Self;
public:
	ReverseIterator()
	{}

	ReverseIterator(Iterator cur)
		: _cur(cur)
	{}

	ReverseIterator(const Self& s)
		: _cur(s._cur)
	{}

	Reference operator*()
	{
		Iterator it(_cur);
		return *(--it);
	}

	Pointer operator->()
	{
		return _cur.operator->();
	}

	Self& operator++()
	{
		_cur.operator--();
		return *this;
	}

	Self operator++(int)
	{
		Self temp(*this);
		_cur.operator--();
		return temp;
	}

	Self& operator--()
	{
		_cur.operator++();
		return *this;
	}

	Self operator--(int)
	{
		Self temp(*this);
		_cur.operator++();
		return temp;
	}

	bool operator==(const Self& s)
	{
		return _cur == s._cur;
	}

	bool operator!=(const Self& s)
	{
		return _cur != s._cur;
	}

private:
	Iterator _cur;
};