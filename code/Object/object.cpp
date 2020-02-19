#pragma once
#include "object.hpp"
#include "../Interface/i_execute.hpp"

Object::Object()
{
	create();
}

void Object::create()
{
	id = Execute::object_pool.size();
	Execute::object_pool.push_back(this);
	add_component<Transform>();
}

const u32 Object::get_id() const
{
	return id;
}

void Object::add_child(const Object & child) const
{
	u32 child_id = child.get_id();

}

void Object::delete_child(Object & child)
{
}
