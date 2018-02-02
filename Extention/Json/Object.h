/*
 * Object.h
 *
 *  Created on: 11 θώνÿ 2015 γ.
 *      Author: hudienko_a
 */

#pragma once

#include "Utils.h"

enum Type
{
	UNKNOWN = 0,
	STRING = 1,
	INT = 2,
	BYTE = 3,
	SHORT = 4,
	STRUCT = 5,
	BOOLEAN = 6,
	MASS = 7,
	LONG=8
};

struct Field
{
	const char* Name;
	Type FieldType;
	int Length;
	void* Value;

};

typedef Field FType;

struct Object
{
protected:
	Field* Fields;
	int FieldCount;
public:
	Object()
	{
		Fields =0;
		FieldCount=0;
	}

	virtual ~Object(){
		if(Fields!=0)
			delete[] Fields;
	}

	Field& GetField(int field)
	{
		return Fields[field];
	}

	bool GetFieldByName(char* name, Field* field)
	{
		for(int i=0;i<FieldCount;i++)
		{
			if(_strcmp((char*)Fields[i].Name,name))
			{
				field->FieldType  = Fields[i].FieldType;
				field->Length  = Fields[i].Length;
				field->Name  = Fields[i].Name;
				field->Value  = Fields[i].Value;
				return true;
			}
		}
		return false;
	}

	char* GetFieldName(int field)
	{
		return (char*)Fields[field].Name;
	}

	int GetFieldCount()
	{
		return FieldCount;
	}

	virtual const char* Name() = 0;
	virtual const char* ToString(char* buf) = 0;

private:
};
