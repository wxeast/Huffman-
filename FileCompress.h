#pragma  once 
#include"HuffmanTree.h"
#include<stdio.h>
typedef long long LongType;

//字符信息
struct CharInf
{
	char _ch;//读取的字符
	LongType _count;//字符的个数
	string _code;//字符编码
	CharInf(LongType count = 0)
		:_count(count)
	{}
	bool operator !=(const CharInf& ch)const
	{
		return  ch._count != _count;
	}
	CharInf operator+(const CharInf & ch)
	{
		return CharInf(_count + ch._count);
	}
	bool operator < (const CharInf& ch)
	{
		return _count < ch._count;
	}
};

//压缩
class FileCompress
{
	typedef HuffmanTreeNode<CharInf> Node;
public:
	FileCompress()
	{
		for (int i = 0; i < 256; ++i)
		{
			_infs[i]._ch = i;
			_infs[i]._count = 0;
		}	
	}
	void  CreateHuffTree(const char* name)
	{
		//生成哈夫曼树
		assert(name);
		FILE*  fr;
		fopen_s(&fr, name, "r");
		assert(fr);
		char ch = fgetc(fr);
		while (ch != EOF)
		{
			_infs[(unsigned char)ch]._count++;
			ch = fgetc(fr);
		}
		CharInf invalid;
		invalid._count = 0;
		_huff.Swap(HuffmanTree<CharInf>(_infs, 256, invalid));
		fclose(fr);

	}
	void Compress(const char* name)
	{
		//生成哈夫曼树
		CreateHuffTree(name);
		//获得哈夫曼编码
		GetHuffCode(_huff.GetRoot());
		//压缩
		string compressfile = name;
		compressfile += ".huff";
		FILE * fw = fopen(compressfile.c_str(), "wb");
		FILE * fr = fopen(name, "r");
		char ch = fgetc(fr);
		char data = 0;
		int  index = 0;
		while (ch != EOF)
		{
			string &code = _infs[(unsigned char)ch]._code;
			for (size_t i = 0; i < code.size(); ++i)
			{
				data |= code[i] - '0';
				index++;
				if (index == 8)
				{
					fputc(data,fw);
					data = 0;
					index = 0;
				}
				else
				{
					data <<= 1;
				}
			}
			ch = fgetc(fr);
		}
		if (index != 0)
		{
			data <<= (7-index);
			fputc(data,fw);
		}
		fclose(fr);
		fclose(fw);
	}
	void  UnCompress(const char * filename)
	{
		//得到文件名 
		assert(filename);
		string name(filename);
		size_t pos = name.rfind('.');
		name = name.replace(pos, name.size() - pos, ".unhuff");
		//解压
		FILE* fw = fopen(name.c_str(), "w");
		FILE* fr = fopen(filename, "rb");
		Node* root = _huff.GetRoot();
		Node* cur = root;
		LongType count = cur->_weight._count;
		while (1)
		{
			char ch = fgetc(fr);
			int  index = 7;
			char data = 1;
			while (cur&&index >= 0)
			{
				if (ch & (data << index))
				{
					cur = cur->_right;
				}
				else
				{
					cur = cur->_left;
				}   
				--index;
				if (cur->_left == NULL && cur->_right == NULL)
				{
					char value = cur->_weight._ch;
					fputc(value,fw);
					count--;
					cur = root;
				}
				if (count == 0)
				{
					fclose(fr);
					fclose(fw);
					return;
				}
			}
		}
	}
protected:
	//得到字符的huffman编码
	void GetHuffCode(Node* root)
	{
		if (root == NULL)
			return;
		//root为叶子结点
		if (root->_left == NULL&&root->_right == NULL)
		{
			Node* parent = root->_parent;
			Node* cur = root;
			string& code = _infs[(unsigned char)(root->_weight._ch)]._code;
			while (parent)
			{
				if (parent->_left == cur)
				{
					code.push_back('0');
				}
				else
				{
					code.push_back('1');
				}
				cur = parent;
				parent = cur->_parent;
			}
			std::reverse(code.begin(),code.end());
			return;
		}
		GetHuffCode(root->_left);
		GetHuffCode(root->_right);
	}
private:
	CharInf _infs[256];
	HuffmanTree<CharInf> _huff;
};


void  Test()
{
	FileCompress fc;
	fc.Compress("Input");
	fc.UnCompress("Input.huff");
}
