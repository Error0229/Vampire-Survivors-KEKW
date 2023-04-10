#include "stdafx.h"
#include "../config.h"
#include "../../Library/gameutil.h"
#include "VSUtil.h"
#include "VSObject.h"
#include "obj_pool.h"
template<class T>
obj_pool<T>::obj_pool(int type)
{
	pool_type = type;
}
template<class T>
obj_pool<T>::~obj_pool()
{
}
template<class T>
void obj_pool<T>::add_obj(int type, int count)
{
	for (int i = 0; i < count; i++) {
		pool.push_back(T(type));
	}
}
template<class T>
T& obj_pool<T>::get_obj(int type)
{
	if (pool.size() == 0) {
		add_obj(type, 1);
	}
	T& obj = pool.back();
	pool.pop_back();
	return obj;
}
template <class T>
vector<T&> obj_pool<T>::get_obj(int type, int count)
{
	vector<T&> obj_list;
	for (int i = 0; i < count; i++) {
		obj_list.push_back(get_obj(type));
	}
	return obj_list;
}
template<class T>
void obj_pool<T>::free_obj(T& obj)
{
	pool.push_back(obj);
}