/*
 * ShareStorage.h
 *
 *  Created on: 30 ����� 2017 �.
 *      Author: hudienko_a
 */

#ifndef EXTENTION_SHARESTORAGE_SHARESTORAGE_H_
#define EXTENTION_SHARESTORAGE_SHARESTORAGE_H_

#define STORAGE_COUNT 100

struct PTR
{
	void* obj;
	unsigned int counter;
};

 class ShareStorage
{
public:
	static ShareStorage* SSObj;

	ShareStorage();

	int Add(void* obj);

	void Modify(void* obj,int index);

	void Remove(void* obj, int index);

	bool Check(void* obj, int index);

	int getIndex(void* obj);

	void* getObj(int index);

private:
	PTR _ptrs[STORAGE_COUNT];
};


#endif /* EXTENTION_SHARESTORAGE_SHARESTORAGE_H_ */
