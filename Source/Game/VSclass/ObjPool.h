#pragma once
template <class T>
class ObjPool
{
public:
	ObjPool() {}
	ObjPool(int type) {
		pool_type = type;
	}
	~ObjPool() = default;
	void add_obj(int type, int count) { // this should be only execute once or not?
		int start_index = pool[type].size();
		for (int i = 0; i < count + 50; i++) { // for safety, its ok to remove it
			pool[type].push_back(T(type));
			pool[type].back().set_pool_id(i + start_index);
			free_vec[type].push_back(i + start_index);
		}
	}
	void add_obj(T obj, int count) {
		int type = obj.get_id();
		int start_index = pool[type].size();
		for (int i = 0; i < count + 50; i++) {
			obj.set_pool_id(i + start_index);
			pool[type].push_back(obj);
			free_vec[type].push_back(i + start_index);
		}
	}

	T& get_obj(int type) {
		VS_ASSERT(pool[type].size() > 0, "The pool is empty, please add more object to the pool");
		// keep first for reseting
		VS_ASSERT(free_vec[type].size() > 0, "exceeded pool limit");
		T& obj = pool[type][free_vec[type].back()];
		free_vec[type].pop_back();
		return obj;
	}
	T* get_obj_ptr(int type) {
		VS_ASSERT(pool[type].size() > 0, "The pool is empty, please add more object to the pool");
		VS_ASSERT(free_obj[type].size() > 0, "exceeded pool limit");
		T* obj = &pool[type][free_vec[type].back()];
		free_vec[type].pop_back();
		return obj;
	}
	vector<reference_wrapper<T>>& get_obj(int type, int count) {
		vector<reference_wrapper<T>> obj_list;
		for (int i = 0; i < count; i++) {
			obj_list.push_back(get_obj(type));
		}
		return obj_list;
	}
	vector<T*>& get_obj_ptr(int type, int count) {
		vector <T*> obj_vec;
		for (int i = 0; i < count; i++) {
			obj_vec.push_back(get_obj_ptr(type));
		}
		return obj_vec;
	}
	void free_obj(T& obj) {
		int pid = obj.get_pool_id(), type = obj.get_id();
		free_vec[type].push_back(pid);
		pool[type][pid] = pool[type][0];
		pool[type][pid].set_pool_id(pid);
	}
	void free_obj_ptr(T* obj) {
		int pid = obj->get_pool_id(), type = obj->get_id();
		free_vec[type].push_back(pid);
		pool[type][pid] = pool[type][0];
		pool[type][pid].set_pool_id(pid);
	}
	int pool_type;
private:
	map<int, vector<T>> pool;
	map<int, vector<int>> free_vec;
};
