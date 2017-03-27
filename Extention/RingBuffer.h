/*
 * RingBuffer.h
 *
 *  Created on: 27 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef EXTENTION_RINGBUFFER_H_
#define EXTENTION_RINGBUFFER_H_


template<uint16_t bufSize, uint16_t count>
class RingBuffer
{
public:
	RingBuffer()
	{
		_position=0;
		_head=0;
		_tail=0;
	}

	bool Add(uint8_t* data, uint16_t length)
	{
		if(_position+length<bufSize)
		{
			_pointers[_tail] = _buff+_position;
			_lengths[_tail] = length;
			memcpy(_buff+_position,data,length);
			_position+=length;
		}
		else
		{
			_position=0;
			_pointers[_tail] = _buff+_position;
			_lengths[_tail] = length;
			memcpy(_buff+_position,data,length);
			_position+=length;
		}

		if(++_tail>=count)
		{
			_tail=0;
		}
		return true;
	}

	bool Get(uint8_t* data, uint16_t* length)
	{
		if(_head!=_tail)
		{
			memcpy(data,_pointers[_head],_lengths[_head]);
			*length = _lengths[_head];
			if(_head++>=count-1)
			{
				_head=0;
			}
			return true;
		}
		return false;
	}

private:

	uint8_t _buff[bufSize];
	uint8_t* _pointers[count];
	uint16_t _lengths[count];
	uint16_t _position;
	uint8_t _head,_tail;

};


#endif /* EXTENTION_RINGBUFFER_H_ */
