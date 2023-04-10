#pragma once
template <class T>
class ObjPool
{
public:
	ObjPool() {}
	ObjPool(int) {
		pool_type = type;
	}
	~ObjPool(){}
	void add_obj(int type, int count) {
		for (int i = 0; i < count; i++) {
			pool.push_back(T(type));
		}
	}

	T& get_obj(int type) {
		if (pool.size() == 0) {
			add_obj(type, 1);
		}
		T& obj = pool.back();
		pool.pop_back();
		return obj;
	}
	vector<reference_wrapper<T>> get_obj(int type, int count) {
		vector<reference_wrapper<T>> obj_list;
		for (int i = 0; i < count; i++) {
			obj_list.push_back(get_obj(type));
		}
		return obj_list;
	}
	void free_obj(T&) {
		pool.push_back(obj);
	}
	int pool_type;
private:
	vector<T> pool;
};
