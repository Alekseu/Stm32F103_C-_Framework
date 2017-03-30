/*
 * SPtr.h
 *
 *  Created on: 30 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef EXTENTION_SPTR_H_
#define EXTENTION_SPTR_H_

#include "shareStorage/shareStorage.h"


template<typename T>
class SPtr
{
public:

	SPtr(T* obj)
	{
		_ptr =obj;
		_index = ShareStorage::SSObj->Add(obj);
	}

	// конструктор копирования
	SPtr(const SPtr &obj)
	{
		_ptr  = obj._ptr;
		_index = ShareStorage::SSObj->Add(_ptr);
	}


	T* get()
	{
		_ptr = (T*)ShareStorage::SSObj->getObj(_index);
		return _ptr;
	}

	T* operator->()
	{
		_ptr = (T*)ShareStorage::SSObj->getObj(_index);
		return _ptr;
	}

	~SPtr()
	{
		ShareStorage::SSObj->Remove(_ptr,_index);
	}

private:
	T* _ptr;
	int _index;
};



#endif /* EXTENTION_SPTR_H_ */
