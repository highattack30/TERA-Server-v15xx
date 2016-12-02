#ifndef LINKLIST_H
#define LINKLIST_H

#include<vector>

#include "typeDefs.h"

#define N_ARY_LIST_MAX 25

template <class T> class link_list
{
public:
	link_list();
	void remove();

	T _val;
	link_list<T>
		*next,
		*prev;
};

template <class T> class nAry_tree
{
public:
	nAry_tree();
	

	T _val;

	bool add(T val);
	void remove();

	nAry_tree * prev;
	nAry_tree<T> * next;
};



template<class T>
link_list<T>::link_list()
{
	_val = (T)0;
	next = prev = nullptr;
}

template<class T>
inline void link_list<T>::remove()
{
	_val = (T)0;

	if (prev)
		prev->next = next;

	if (next)
		next->prev = prev;
}

template<class T>
inline nAry_tree<T>::nAry_tree()
{
	next = new nAry_tree<T>[N_ARY_LIST_MAX];
	memset(next, 0, sizeof(nAry_tree<T>) * N_ARY_LIST_MAX);
	prev = nullptr;
	_val = (T)0;
}

template<class T>
inline void nAry_tree<T>::remove()
{
	for (uint32 i = 0; i < N_ARY_LIST_MAX; i++)
		if (next[i])
			next[i]->prev = prev;

	prev = nullptr;
	_val = (T)0;
}

template<class T>
bool nAry_tree<T>::add(T val)
{
	for (uint32 i = 0; i < length; i++)
		if (!next[i])
		{
			next[i] = new nAry_tree<T>();
			next[i]->prev = this;
			next[i]->_val = val;
			return TRUE;
		}

	return FALSE;
}

#endif


