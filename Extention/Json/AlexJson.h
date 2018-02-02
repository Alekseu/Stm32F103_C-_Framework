/*
 * AlexJson.h
 *
 *  Created on: 11 θώνÿ 2015 γ.
 *      Author: hudienko_a
 */
#pragma once
#include "Object.h"
#include <string.h>
#include "Utils.h"


class Json
{
public:

	//todo serialize mass
	template <class T>
	int serialize(T* obj, char* buff)
	{
		char* tmp =  buff;
		int index = 0;
		tmp[index++] = '{';
		Field tmp1;

		int field_count = ((Object*)obj)->GetFieldCount();

		for (int i = 0; i < field_count; i++)
		{
			tmp1 = obj->GetField(i);
			tmp[index++] = '"';
			memcpy(tmp + index, (void*)obj->GetFieldName(i), strlen(obj->GetFieldName(i)));
			index += strlen(obj->GetFieldName(i));
			tmp[index++] = '"';
			tmp[index++] = ':';

			bool bool_field=false;
			char char_field=0;
			short short_field=0;
			int int_field=0;
			long long_field =0;
			int length_field=0;
			char* string_field=0;

			int serialize_length=0;

			switch (tmp1.FieldType)
			{
				case Type::BYTE:
					char_field= *((char*)tmp1.Value);
					itoa(char_field,buf,10);
					length_field = strlen(buf);
					memcpy(tmp + index, buf, length_field);
					index += length_field;
					break;

				case Type::SHORT:
					short_field= *((short*)tmp1.Value);
					itoa(short_field,buf,10);
					length_field = strlen(buf);
					memcpy(tmp + index, buf, length_field);
					index += length_field;
					break;

				case Type::INT:
					int_field= *((int*)tmp1.Value);
					itoa(int_field,buf,10);
					length_field = strlen(buf);
					memcpy(tmp + index, buf, length_field);
					index += length_field;
					break;

				case Type::LONG:
					long_field= *((long*)tmp1.Value);
					itoa(long_field,buf,10);
					length_field = strlen(buf);
					memcpy(tmp + index, buf, length_field);
					index += length_field;
					break;

				case Type::STRING:
					tmp[index++] = '"';
					string_field = (char*)tmp1.Value;
					length_field = strlen(string_field);
					memcpy(tmp + index, string_field,length_field );
					index +=length_field;
					tmp[index++] = '"';
					break;

				case Type::BOOLEAN:
					bool_field= *((bool*)tmp1.Value);

					if(bool_field)
					{
						memcpy(buf,"true",4);
						length_field =4;
					}
					else
					{
						memcpy(buf,"false",5);
						length_field =5;
					}

					memcpy(tmp + index, buf, length_field);
					index += length_field;
					break;

				case Type::STRUCT:
					serialize_length = serialize((Object*)tmp1.Value,tmp + index);
					index += serialize_length;
					break;

				case Type::MASS:

					break;
				case Type::UNKNOWN:
					break;
			}
			tmp[index++] = ',';
		}
		tmp[index-1] = '}';
		tmp[index ] = 0;
		return index;
	}

	//todo check deselialize struct, deserialize mass
	template <class T>
	bool deserialize(char* source , int sourcelength,T* model)
	{
		Object* _model = (Object*)model;
		Field tmp1;

		int index=1;
		int endof=0;

		int true_count=0;
		int false_count=0;

		start:
		memset(buffer,0,255);
		memset(field,0,64);
		memset(value,0,128);

		endof = indexOf(source+index, ',',sourcelength);
		if(endof==0) endof = sourcelength-2;
		memcpy(buffer,source+index,endof);

		if(split_field(buffer,strlen(buffer),field, value))
		{
			if( _model->GetFieldByName(field,&tmp1))
			{
				switch(tmp1.FieldType)
				{
					case Type::BYTE:
						*((char*)tmp1.Value) = atoi(value);
						break;
					case Type::SHORT:
						*((short*)tmp1.Value) = atoi(value);
						break;
					case Type::INT:
						*((int*)tmp1.Value) = atoi(value);
						break;
					case Type::LONG:
						*((long*)tmp1.Value) = atoi(value);
						break;
					case Type::BOOLEAN:

						if(_strcmp(value,(char*)"true"))
						{
							*((bool*)tmp1.Value) = true;
						}
						else
						{
							*((bool*)tmp1.Value) = false;
						}

						break;
					case Type::STRING:
						memset(tmp1.Value,0,tmp1.Length);
						memcpy(tmp1.Value,value, tmp1.Length);

						break;
					case Type::STRUCT:
						deserialize(value,strlen(value),tmp1.Value);

						break;
					case Type::MASS:
						break;
					case Type::UNKNOWN:
						break;
				}
				true_count++;
			}
			else
			{
				false_count++;
			}
		}

		index+=endof+1;

		if(index<sourcelength) goto start;

		return true_count>false_count;
	}

private:

	char buf[20];
	char buffer[255];
	char field[64];
	char value[128];

};
