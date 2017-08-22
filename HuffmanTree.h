#pragma once
#include"Heap.h"


template<class T>
struct HuffmanTreeNode
{
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;
	T _weight;
	HuffmanTreeNode(const  T &  value)
		:_weight(value), _left(NULL), _right(NULL)
		, _parent(NULL)
	{}
};

template<class  T>
class  HuffmanTree
{
	typedef  HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
		:_root(NULL)
	{}
	HuffmanTree(const  T  * array,size_t n ,const  T& invalid = T())
	{
		struct Compare
		{
			bool operator()(Node *  left, Node * right)
			{
				return left->_weight < right->_weight;
			}
		};
		//构建一个小堆
		Heap<Node*, Compare> hp;
		for (size_t i = 0; i < n; ++i)
		{
			if (array[i] != invalid)
			{
				hp.Push(new  Node(array[i]));
			}
		}
		//使用贪心算法来构造有个哈夫曼树
		while (hp.Size()>1)
		{
			//得到最小的数放到最底部
			Node  *  left = hp.Top();
			hp.Pop();
			Node  *  right = hp.Top();
			hp.Pop();
			Node * cur = new  Node(left->_weight + right->_weight );
			//左右结点链好
			cur->_left = left;
			cur->_right = right;
			left->_parent = cur;
			right->_parent = cur;
			hp.Push(cur);
		}
		_root = hp.Top();
	}
	Node*  GetRoot()
	{
		return  _root;
	}
	void Destroy(Node * root)
	{
		if (root == NULL)
			return;
		Destroy(root->_left);
		Destroy(root->_right);
		delete root;
	}
	void  Swap(HuffmanTree<T>& hf)
	{
		std::swap(hf._root,_root);
	}
	~HuffmanTree()
	{
		Destroy(_root);
		_root = NULL;
	}
private:
	Node  * _root; 
};


void TestHuff()
{
	int array[] = {1,2,3,4,5,6};
	HuffmanTree<int> huff(array,6);
}