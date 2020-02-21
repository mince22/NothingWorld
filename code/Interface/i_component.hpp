#pragma once

#include "../common.hpp"

enum class Component_Type
{
	Transform,
	ModelComponent,
	Max,
};

string component_type_name[u32(Component_Type::Max)] = { "Transform", "Model_Component" };

class I_Component {
public:
	template<typename T>
	static const Component_Type deduce_component_type();

public:
	I_Component(u32 owner_id) : owner_id(owner_id) { };

	I_Component(u32 owner_id, Component_Type type) : owner_id(owner_id), component_type(type) {
		component_name = component_type_name[(u32)component_type] + "_" + to_string(++component_id);
	};

	virtual ~I_Component() {};

	virtual void init() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;

	const Component_Type& get_component_type() const { return component_type; }
	string get_component_name() const { return component_name; }

protected:
	Component_Type component_type;
	string component_name;
	u32 owner_id;
	static u32 component_id;
};

u32 I_Component::component_id = -1;

template <typename T>
inline const Component_Type I_Component::deduce_component_type()
{
	string type_name = typeid(T).name();
	Component_Type type = Component_Type::Max;

	if (type_name.find(component_type_name[u32(Component_Type::Transform)]) != string::npos) type = Component_Type::Transform;
	else if (type_name.find(component_type_name[u32(Component_Type::ModelComponent)]) != string::npos) type = Component_Type::ModelComponent;

	return type;
}