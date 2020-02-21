#pragma once
#include "../common.hpp"
#include "object_id.hpp"
#include "../Interface/i_component.hpp"

class Object {
public:
	Object(wstring name = L"");

	void create();
	void destroy();
	
	void add_object_pool();

	template<typename T> T* add_component();
	
	const Object_ID get_id() const;

	void add_child(const Object& child) const;
	void delete_child(Object& child);

private:
	Object_ID id;

	bool has_parent;
	bool active;

	Transform transform;
	unordered_map<string, I_Component *> components;
};

template<typename T>
inline T * Object::add_component()
{
	Component_Type type = I_Component::deduce_component_type<T>();

	auto new_component = new T(this->id.get_id(), type);

	components.insert(unordered_map<string, I_Component *>::value_type(new_component->get_component_name(), new_component));

	return new_component;
}
