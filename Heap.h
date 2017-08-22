#pragma once  
//实现一个堆
template<class T>
struct Less
{
	bool operator()(const T &left, const T& right)
	{
		return  left < right;
	}
};
template<class T>
struct Greater
{
	bool operator()(const T &left, const T& right)
	{
		return  left > right;
	}
};

#include<iostream>
using  namespace std;
#include<vector>
#include<assert.h>
template<class  T,class Compare = Less<T>>
class  Heap
{
public:
	Heap()
	{}
	Heap(T * array, int n)
		:_data(array,array+n)
	{
		for (int i = (n - 2) >> 1; i >= 0; --i)
		{
			AdjustDown(i);
		}
	}
	void Push(const T&  value)
	{
		_data.push_back(value);
		AdjustUp(_data.size()-1);
	}
	void Pop()
	{
		assert(_data.size());
		std::swap(_data[0],_data[_data.size() - 1]);
		_data.pop_back();
		AdjustDown(0);
	}
	bool Empty()
	{
		return _data.size() == 0;
	}
	size_t Size()
	{
		return _data.size();
	}
	T & Top()
	{
		assert(_data.size());
		return _data[0];
	}
protected:
	//向下调整
	void AdjustDown(int  k)
	{
		int  parent = k;
		int  child = parent * 2 + 1;
		int  len = _data.size();
		while (child < len)
		{
			if (child + 1 < len && Compare()(_data[child + 1], _data[child]))
			{
				child++;
			}
			if (Compare()(_data[child], _data[parent]))
			{
				std::swap(_data[child],_data[parent]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}
	//向上调整
	void AdjustUp(int k)
	{
		int  child = k;
		int  parent = (child - 1) >> 1;
		while (parent >= 0)
		{
			if (Compare()(_data[child], _data[parent]))
			{
				std::swap(_data[child],_data[parent]);
				child = parent;
				parent = (child - 1) >> 1;
			}
			else
			{
				break;
			}
		}
	}
private:
	vector<T> _data;
};