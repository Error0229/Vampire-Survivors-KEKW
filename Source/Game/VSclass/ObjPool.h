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
	void add_obj(int id, int count) {
		for (int i = 0; i < count; i++) {
			pool[id].push_back(T(id));
		}
	}

	T& get_obj(int id) {
		if (pool[id].size() == 0) {
			add_obj(id, 1);
		}
		T& obj = pool[id].back();
		pool[id].pop_back();
		return obj;
	}
	vector<reference_wrapper<T>> get_obj(int id, int count) {
		vector<reference_wrapper<T>> obj_list;
		for (int i = 0; i < count; i++) {
			obj_list.push_back(get_obj(id));
		}
		return obj_list;
	}
	void free_obj(T&) {
		pool[T.get_id()].push_back(obj);
	}
	int pool_type;
private:
	map<int, vector<T>> pool;
};
