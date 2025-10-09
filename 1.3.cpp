#include<iostream>
#include <vector>
#include <cassert>
using namespace std;
template<typename T>
class Ele//三元组，用来储存元素行列和值
{//满足最基本需求，没有进行过多封装
public:
	int row, col;
	T value;
	Ele(int row1 = 0, int col1 = 0, T value1 = 0) 
	{
		row = row1; col = col1; value = value1; 
	}
	friend ostream& operator<<(ostream& os, const Ele<T>& e) 
	{
		os << "Row: " << e.row << ", Col: " << e.col << ", Value: " << e.value;
		return os;
	}
	Ele(const Ele& b)
	{
		row = b.row;
		col = b.col;
		value = b.value;
	}
	Ele& operator=(const Ele& b) {
		if (this == &b)return *this;
		row = b.row;
		col = b.col;
		value = b.value;
		return *this;
	}
};

template<typename T>
class SparseMatrix //自定义稀疏矩阵类
{
public:
	Ele<T> * eles; //稀疏矩阵用三元组的数组存储
	int count;
	int max_row, max_col;
	SparseMatrix(T** nums, int row, int col) 
	{	//先排除一定的不合法输入
		if (row <= 0 || col <= 0 || nums == nullptr) {
			throw std::invalid_argument("Invalid matrix dimensions or null pointer.");
		}
		//先统计个数
		max_row = row;
		max_col = col;
		count = 0;
		for (int i = 0; i < row; i++) 
		{
			for (int j = 0; j < col; j++) 
			{
				if (nums[i][j] != 0) 
				{
					count++;
				}
			}
		}
		eles = new Ele<T>[count];
		int index = 0;
		//再存储非零元素
		for (int i = 0; i < row; i++) 
		{
			for (int j = 0; j < col; j++) 
			{
				if (nums[i][j] != 0) 
				{
					assert(index < count);//确保不越界
					eles[index]=Ele<T>(i, j, nums[i][j]);
					index++;
				}
			}
		}
	}

	void show_all() 
	{	//输出所有非零元素
		for (int i = 0; i < count;i++) 
		{
			cout << eles[i] << endl;
		}
	}	

	~SparseMatrix() 
	{
		delete[] eles;
		eles = nullptr;
	}
	SparseMatrix operator=(const SparseMatrix& b)
	{
		if (this == *b)
		{
			return *this;
		}
		max_row=b.max_row;
		max_col = b.max_col;
		count = b.count;
		delete[] eles;
		eles = nullptr;
		eles = new Ele<T>[count];
		for (int i = 0; i < count; i++)
		{
			eles[i] = b.eles[i];

		}
		return *this;
	}
	SparseMatrix (const SparseMatrix& b)
	{
		
		max_row = b.max_row;
		max_col = b.max_col;
		count = b.count;
		delete[] eles;
		eles = new Ele<T>[count];
		for (int i = 0; i < count; i++)
		{
			eles[i] = b.eles[i];
		}
		
	}

	void FastTranspose() 
	{
		// 快速转置算法的实现
		int* info = new int[2 * max_col];
		for (int i = 0; i < max_col; ++i) info[i] = 0; // RowSize初始化
		for (int i = 0; i < max_col; ++i) info[max_col + i] = 0; // RowStart初始化

		Ele<T>* copy_list = new Ele<T>[count];
		
		for (int i = 0; i < count; i++)//统计每一列的非零元素个数
		{
			info[eles[i].col]++;
		}
		for (int i = 1; i < max_col; i++)//计算每一列第一个非零元素在转置后的位置
		{
			info[i+max_col] = info[max_col+i - 1] + info[i - 1];

		}
		for (int i = 0; i < count; i++)//将元素放到转置后的位置
		{
			int pos = info[max_col+eles[i].col];
			copy_list[pos] = Ele<T>(eles[i].col, eles[i].row, eles[i].value); 
			info[max_col+eles[i].col]++;
		
		}
		delete[] eles;
		eles = new Ele<T>[count];
		for (int i = 0; i < count; i++)//将元素放到转置后的位置
		{
			eles[i] = copy_list[i];

		}
		//删除临时数组
		delete[] copy_list;
		delete[] info;
		info = nullptr;
		copy_list = nullptr;

	}
	

};
int main()
{
	//test Ele begin
	cout << "Test Ele class:" << endl;
	Ele<int> e(1, 2, 3);
	cout << e << endl; // 输出: Row: 1, Col: 2, Value: 3
	Ele<double> e2(4, 5, 6.7);
	cout << e2 << endl; // 输出: Row: 4, Col: 5, Value: 6.7
	//test end
	//test SparseMatrix begin
	cout << "Test SparseMatrix class:" << endl;
	//创建矩阵并用指针表示
	int matrix_test2[5][8] = {
		{0,0,3,0,4,0,0,0},
		{0,0,5,7,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,2,6,0,0,0,8,0},
		{0,0,0,9,0,0,0,0}
	};
	int* matrix_ptr[5];
	for (int i = 0; i < 5; ++i)
	{
		matrix_ptr[i] = matrix_test2[i];
	}
	//创建实例并测试
	SparseMatrix<int>* sm = nullptr;
	try {
		sm = new SparseMatrix<int>(matrix_ptr, 5, 8);
		sm->show_all();
	}
	catch (const std::invalid_argument& e) {
		cout << "Caught exception: " << e.what() << endl;
	}

	if (sm) {
		cout << "After Fast Transpose:" << endl;
		sm->FastTranspose();
		sm->show_all();
		delete sm;
	}
}