/*
 * LinkedList.h
 *
 *  Created on: 27 марта 2017 г.
 *      Author: hudienko_a
 */

#ifndef EXTENTION_LINKEDLIST_H_
#define EXTENTION_LINKEDLIST_H_

#include <stddef.h>

template <class T>
struct Node
{
	T value;
	Node<T>* next;
	Node<T>* prev;
};


template<class T>
class LList
{
	public:

		LList()
		{
			first=NULL;
			length =0;
		}

		virtual ~LList()
		{

		}

		T operator[] (int pos)
		{
			 Node<T>* temp = first;
			 for ( int i = 0; i < length; i++)
			{
				if(i==pos)
				{
					return temp->value;
				}
				temp=temp->next;
			}
			return NULL;
		}

		 void add(T* d )
		 {
			 	 Node<T>* newnode = new Node<T>; // создаём новый элемент
				 newnode->value = *d; // вводим в него данные

				 if(first == NULL)   // если это первый элемент в списке
				 {
					 newnode->next = NULL;
					 newnode->prev = NULL;
					 first = newnode; // first  указывает на него
					 length++;
				 }
				 else
				 {
					 Node<T>* temp = first; // создаём времменный указатель
					 for ( int i = 0; i < length-1; i++){temp=temp->next;} // цикл

						 newnode->prev = temp;
						 newnode->next = NULL; // добавляем перед времменным
						 temp->next = newnode;
						 length++;
				 }
		 }

		 void add(T d )
				 {
					 	 Node<T>* newnode = new Node<T>; // создаём новый элемент
						 newnode->value = d; // вводим в него данные

						 if(first == NULL)   // если это первый элемент в списке
						 {
							 newnode->next = NULL;
							 newnode->prev = NULL;
							 first = newnode; // first  указывает на него
							 length++;
						 }
						 else
						 {
							 Node<T>* temp = first; // создаём времменный указатель
							 for ( int i = 0; i < length-1; i++){temp=temp->next;} // цикл

								 newnode->prev = temp;
								 newnode->next = NULL; // добавляем перед времменным
								 temp->next = newnode;
								 length++;
						 }
				 }

		 void remove(T d)
		 {
			 if(first == NULL) return ;
			 Node<T>* cur = first;
			 for(int i=0;i<length;i++)
			 {
				 if(cur->value==d)
				 {
					  cur->prev->next = cur->next;
					  cur->next->prev =  cur->prev;
					  length--;
					 delete cur;
					  return;
				 }
				 cur = cur->next;
			 }
		 }


		 void view()
		 {
				Node<T>* temp = first;
				for ( int i = 0; i < length; i++)
				{
					//trace_printf("value : %u \n",temp->value.value);
					temp=temp->next;
				}
		 }

		 void clean()
		 {
			  if(first == NULL) return ;
			  Node<T>* last = first;
			  Node<T>* cur = NULL;
			  for(int i=0;i<length-1;i++){last=last->next;}

			  for(int i=length;i>0;i--)
			  {
				  cur = last;
				  last = cur->prev;
				  last->next = NULL;
				  delete cur;
			  }

			  first = NULL;
			  length=0;
		 }

	private:
		Node<T>* first;
		int length;
};



#endif /* EXTENTION_LINKEDLIST_H_ */
