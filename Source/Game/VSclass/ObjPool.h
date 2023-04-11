#pragma once
//#pragma once
//template <class T>
//class ObjPool
//{
//public:
//    ObjPool() {}
//    ObjPool(int type) : pool_type(type) {}
//    ~ObjPool() {}
//
//    void add_obj(int id, int count)
//    {
//        auto& obj_list = pool[id];
//        obj_list.reserve(obj_list.size() + count);
//        for (int i = 0; i < count; i++)
//        {
//            obj_list.emplace_back(id);
//        }
//    }
//
//    T& get_obj(int id)
//    {
//        auto& obj_list = pool[id];
//        if (obj_list.empty())
//        {
//            add_obj(id, 1);
//        }
//        T& obj = obj_list.back();
//        obj_list.pop_back();
//        return obj;
//    }
//
//    std::vector<std::reference_wrapper<T>> get_obj(int id, int count)
//    {
//        auto& obj_list = pool[id];
//        std::vector<std::reference_wrapper<T>> result;
//        result.reserve(count);
//        for (int i = 0; i < count; i++)
//        {
//            if (obj_list.empty())
//            {
//                add_obj(id, count - i);
//            }
//            result.emplace_back(obj_list.back());
//            obj_list.pop_back();
//        }
//        return result;
//    }
//
//    void free_obj(T& obj)
//    {
//        int id = obj.get_id();
//        auto& obj_list = pool[id];
//        obj_list.emplace_back(std::move(obj));
//    }
//
//    int pool_type = 0;
//
//private:
//    std::map<int, std::vector<T>> pool;
//};

template <class T>
class ObjPool
{
public:
	ObjPool() {}
	ObjPool(int type) {
		pool_type = type;
	}
	~ObjPool(){}
	void add_obj(int id, int count) { // this should be only execute once or not?
		int start_index = pool[id].size();
		for (int i = 0; i < count; i++) {
			pool[id].push_back(T(id));
			pool[id].back().set_pool_id(i + start_index);
			🔞[id].push_back(i + start_index);
		}
	}

	T& get_obj(int id) {
		VS_ASSERT(pool[id].size() > 0, "The pool is empty, please add more object to the pool");
		// keep first for reseting
		VS_ASSERT(🔞[id].size() > 0, "exceeded pool limit");
		
		T& obj = pool[id][🔞[id].back()];
		// obj.set_pool_id(🔞[id].back());
		🔞[id].pop_back();
		return obj;
	}
	vector<reference_wrapper<T>> get_obj(int id, int count) {
		vector<reference_wrapper<T>> obj_list;
		for (int i = 0; i < count; i++) {
			obj_list.push_back(get_obj(id));
		}
		return obj_list;
	}
	void free_obj(T& obj) {
		int id = obj.get_pool_id(), obj_id = obj.get_id();
		🔞[obj_id].push_back(id);
		pool[obj_id][id] = pool[obj_id][0];
		pool[obj_id][id].set_pool_id(id);
	}
	int pool_type;
private:
	map<int, vector<T>> pool;
	map<int, vector<int>> 🔞;
};
