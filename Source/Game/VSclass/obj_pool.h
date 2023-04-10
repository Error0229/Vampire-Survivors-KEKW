#pragma once
template <class T>
class obj_pool
{
public:
	obj_pool(int);
	~obj_pool();
	void add_obj(int type, int count);
	T& get_obj(int type);
	vector<T&> get_obj(int type, int count);
	void free_obj(T&);
	int pool_type;
private:
	vector<T&> pool;
};
