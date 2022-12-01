#pragma once

#include <iostream>
#include <assert.h>
#include <string>
using namespace std;

namespace fu
{
	template<class T>
	class vector
	{
	public:
		typedef T* iterator;
		typedef const T* const_iterator;
		vector()
			:_start(nullptr),
			_finish(nullptr),
			_endofstorge(nullptr)
		{}

		vector(int n, const T& val = T())
			:_start(nullptr),
			_finish(nullptr),
			_endofstorge(nullptr)
		{
			reserve(n);
			for (int i = 0; i < n; ++i)
			{
				push_back(val);
			}
		}

		vector(size_t n, const T& val = T())
			:_start(nullptr),
			_finish(nullptr),
			_endofstorge(nullptr)
		{ 
			reserve(n);
			for (size_t i = 0; i < n; ++i)
			{
				push_back(val);
			}
		}

		/*传统写法
		vector(const vector<T>& v)
			:_start(nullptr),
			_finish(nullptr),
			_endofstorge(nullptr)
		{
			reserve(v.capacity());
			for (const auto& e : v)
			{
				push_back(e);
			}
		}*/

		template <class InputIterator>
		vector(InputIterator first, InputIterator last)
			:_start(nullptr),
			_finish(nullptr),
			_endofstorge(nullptr)
		{
			while (first != last)
			{
				push_back(*first);
				++first;
			}
		}

		vector(const vector<T>& v)
			:_start(nullptr),
			_finish(nullptr),
			_endofstorge(nullptr)
		{
			vector<T> tmp(v.begin(), v.end());
			swap(tmp);
		}

		//v1 = v1;有代价
		vector<T>& operator=(vector<T> v)
		{
			swap(v);
			return *this;
		}

		~vector()
		{
			delete[] _start;
			_start = _finish = _endofstorge = nullptr;
		}

		iterator begin()
		{
			return _start;
		}

		const_iterator begin() const
		{
			return _start;
		}

		iterator end()
		{
			return _finish;
		}

		const_iterator end() const 
		{
			return _finish;
		}

		T& operator[](size_t pos)
		{
			assert(pos < size());
			return _start[pos];
		}

		/*iostream operator<<(T*& v)
		{
			cout<<
		}*/

		const T& operator[](size_t pos) const
		{
			assert(pos < size());
			return _start[pos];
		}

		void reserve(size_t n)
		{
			if (n > capacity())
			{
				size_t oldSize = size();
				T* tmp = new T[n];
				if (_start)
				{
					////这里在拷贝自定义类型时是浅拷贝
					//memcpy(tmp, _start, sizeof(T) * oldSize);
					
					for (size_t i = 0; i < oldSize; ++i)
					{
						tmp[i] = _start[i];
					}
					delete[] _start;
				}
				_start = tmp;
				_finish = _start + oldSize;
				_endofstorge = _start + n;
			}
		}

		void resize(size_t n, T val = T())
		{
			if (n > capacity())
			{
				reserve(n);
			}
			if (n > size())
			{
				while (_finish < _start + n)
				{
					*_finish = val;
					++_finish;
				}
			}
			else
			{
				_finish = _start + n;
			}
		}

		bool empty() const
		{
			return _finish == _start;
		}

		size_t size() const
		{
			return _finish - _start;
		}

		size_t capacity() const
		{
			return _endofstorge - _start;
		}

		void push_back(const T& x)
		{
			if (_finish == _endofstorge)
			{
				size_t newCapacity = capacity() == 0 ? 4 : capacity() * 2;
				reserve(newCapacity);
			}
			
			*_finish = x;
			++_finish;
		}

		void pop_back()
		{
			assert(!empty());
			--_finish;
		}

		iterator insert(iterator pos, const T& val)//注意野指针
		{
			assert(pos >= _start);
			assert(pos < _finish);
			
			if (_finish == _endofstorge)
			{
				size_t len = pos - _start;
				size_t newCapacity = capacity() == 0 ? 4 : 2 * capacity();
				reserve(newCapacity);
				//扩容导致pos失效
				pos = _start + len;
			}

			//挪动数据
			iterator end = _finish - 1;
			while (end >= pos)
			{
				*(end + 1) = *end;
				--end;
			}

			*pos = val;
			++_finish;

			return pos;
		}

		iterator erase(iterator pos)
		{
			assert(pos >= _start);
			assert(pos < _finish);

			iterator begin = pos + 1;
			while (_finish > begin)
			{
				*(begin - 1) = *(begin);
				++begin;
			}
			--_finish;

			return pos;
		}

		void swap(vector<T> &v)
		{
			std::swap(_start, v._start);
			std::swap(_finish, v._finish);
			std::swap(_endofstorge, v._endofstorge);
		}

		void clear()
		{
			_finish = _start;
		}

	private:
		iterator _start;
		iterator _finish;
		iterator _endofstorge;
	};

	void test1()
	{
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);
		v.push_back(5);
		v.push_back(6);
		v.push_back(7);
		v.pop_back();
		vector<int>::iterator it=find(v.begin(), v.end(), 3);
		v.insert(it, 10);
		for (auto e : v)
		{
			cout << e << ' ';
		}
		cout << endl;
		it= find(v.begin(), v.end(), 3);
		v.erase(it);
		for (auto e : v)
		{
			cout << e << ' ';
		}
		cout << endl;
	}
	void test2()
	{
		vector<int> v;
		v.push_back(1);
		v.push_back(2);
		v.push_back(3);
		v.push_back(4);

		v.clear();
		v.push_back(1);

		for (auto e : v)
		{
			cout << e << ' ';
		}
	}
	
	void test3()
	{
		vector<int> v1(10, 1);
		vector<int> v2 = v1;
		for (auto e : v1)
		{
			cout << e << ' ';
		}
		cout << endl;
		for (auto e : v2)
		{
			cout << e << ' ';
		}
	}

	void test4()
	{
		vector<vector<int>> vv;
		vector<int> v(10, 1);
		vv.push_back(v);
		vv.push_back(v);
		vv.push_back(v);
		vv.push_back(v);
		vv.push_back(v);
		for (int i = 0; i < vv.size(); ++i)
		{
			for (int j = 0; j < v.size(); ++j)
			{
				cout << vv[i][j] << " ";
			}
			cout << endl;
		}
	}

	void test5()
	{
		vector<string> vs;
		string s("abcd", 6);
		vs.push_back(s);
		vs.push_back(s);
		vs.push_back(s);
		vs.push_back(s);
		vs.push_back(s);
		vs.push_back(s);
		
		for (int i = 0; i < vs.size(); ++i)
		{
			for (int j = 0; j < s.size(); ++j)
			{
				cout << vs[i][j] << " ";
			}
			cout << endl;
		}
	}
}