#pragma once
#include "../common.hpp"
#include "../Interface/i_component.hpp"

class Object {
public:
	Object();

	void create();
	void destroy();
	
	template<typename T> T* add_component();
	
	const u32 get_id() const;
	


	void add_child(const Object& child) const;
	void delete_child(Object& child);


private:
	string name;
	UINT id;

	bool has_parent;
	bool active;

	unordered_map<string, I_Component *> components;
};

template<typename T>
inline T * Object::add_component()
{
	Component_Type type = I_Component::deduce_component_type<T>();

	auto new_component = new T(this->id, type);

	components.insert(make_pair(new_component->get_component_name(), new_component));

	return new_component;
}
