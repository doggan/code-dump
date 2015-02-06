#include "Item.h"

Item::Item(uint id, const std::string &name)
: m_id(id),
m_name(name)
{
	m_slot = GENERAL;
}

Item::~Item()
{

}

bool Item::use()
{
	return true;
}

bool Item::isEquippable()
{
	if (m_slot != GENERAL)
		return true;

	return false;
}