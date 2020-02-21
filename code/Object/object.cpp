#pragma once
#include "object.hpp"
#include "../Interface/i_execute.hpp"

Object::Object(wstring name)
{
	create();
	Execute::add_object_pool(this);
}

void Object::create()
{
	
}

void Object::destroy()
{
	for (auto component : components)
		component.second->destroy();
}

void Object::add_object_pool()
{
}


const Object_ID Object::get_id() const
{
	return id;
}

void Object::add_child(const Object & child) const
{
	Object_ID child_id = child.get_id();

}

void Object::delete_child(Object & child)
{
}
