#pragma once
#include "../common.hpp"

#include <stack>

namespace Object_Pool {
	u32 index = 0;
	vector<u32> index_list;
	vector<u32> delete__pool;
}

class Object_ID {
public:
	Object_ID(wstring object_name = L"") {
		if (object_name == L"")
			name = L"Object" + to_wstring(Object_Pool::index);
		else
			name = object_name;

		Object_Pool::index_list.push_back(Object_Pool::index);

		if (Object_Pool::delete__pool.size() > 0)
		{
			Object_Pool::index = Object_Pool::delete__pool[Object_Pool::delete__pool.size() - 1];
			Object_Pool::delete__pool.pop_back();
		}

		else
			Object_Pool::index++;
	};

	~Object_ID() {
		Object_Pool::delete__pool.push_back(id);
	}

	wstring get_name() const { return name; };
	u32 get_id() const { return id; };

	void set_name(wstring name) { this->name = name; };
	void set_id(u32 id) { this->id = id; };

private:
	wstring name;
	u32 id;
};
