/*
 * Extention.h
 *
 *  Created on: 05.09.2013
 *      Author: YVShvets
 */

#ifndef EXTENTION_H_
#define EXTENTION_H_

#include <string.h>

template <class T>
class List
{
public:

	List()
	{
		_len = 0;
		_data = 0;
	}

	int Lendth() { return _len; }

	void Clear()
	{ 
		_len = 0; 
		
		if (_data != 0)	delete [] _data;
	
		_data = 0; 
	}

	void Add(T val)
	{
		if (_data == 0)
		{
			_data = new T[1];

			_data[_len++] = val;
		}
		else
		{
			T* tmp = new T[_len];

			memcpy(tmp, _data, _len);

			delete [] _data;

			_data = new T[_len + 1];

			memcpy(_data, tmp, _len);

			delete [] tmp;

			_data[_len++] = val;
		}
	}

	T operator [](int index)
	{
		if (index > _len - 1 || index < 0) return 0;

		return _data[index];
	}

	const char* ToString()
	{
		return (const char*) _data;
	}

	virtual ~List() { delete[] _data; }


private:
	T* _data;

	int _len;
};

template <class Key, class Val>
class Dictionary
{
public:

	Dictionary()
	{

	}

	void Add(Key key, Val val)
	{
		_key.Add(key);
		_val.Add(val);
	}

	void Delete(Key key)
	{

	}

	void Clear()
	{
		_key.Clear();
		_val.Clear();
	}

	Val operator[] (Key key)
	{
		for (int i = 0; i < _key.Length(); i++)
		{
			if (_key[i] == key)
			{
				return _val[i];
			}
		}

		return 0;
	}

private:
	List<Key> _key;
	List<Val> _val;
};

#endif /* EXTENTION_H_ */
