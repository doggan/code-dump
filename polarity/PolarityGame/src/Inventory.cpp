#include "Inventory.h"

#include "GUI.h"
#include "Item.h"
#include "ItemManager.h"
#include "Player.h"

Inventory::Inventory(EntityBase *owner)
{
	m_owner = owner;
}

Inventory::~Inventory()
{

}

void Inventory::addItem(Item *item)
{
	m_inventory.insert(item);
}

void Inventory::removeItem(Item *item)
{
	unequip(item);

	std::set< Item* >::iterator iter = m_inventory.find(item);
	if (iter != m_inventory.end())
	{
		delete *iter;
		m_inventory.erase(iter);
		return;
	}

	assert(!"Inventory::removeItem(id), id not found!");
}

void Inventory::equip(Item *i)
{
	// If i is equippable, and there is no item currently in the same slot, equip it.
	if (i->isEquippable())
		// TODO: && need to check if an item is alrdy equipped in the given slots.
	{
		m_inventory.erase(i);
		m_equipped.insert(i);
		m_owner->applyStats(i->getStats());

		return;
	}

	if (dynamic_cast<Player*>(m_owner))
		GUI::getSingleton().addToChat("Unable to equip: " + i->getName());
}

void Inventory::equip(const std::set< Item* > &items)
{
	std::set< Item* >::const_iterator iter = items.begin();
	for (iter; iter != items.end(); ++iter)
	{
		equip(*iter);
	}
}

void Inventory::unequip(Item *i)
{
	// If i is equipped, unequip it.
	if (isEquipped(i))
	{
		m_equipped.erase(i);
		m_inventory.insert(i);
		m_owner->deapplyStats(i->getStats());
	}
}

std::set< Item* > Inventory::unequipAll()
{
	std::set< Item* > res = m_equipped;
	
	std::set< Item* >::iterator iter = m_equipped.begin();
	for (iter; iter != m_equipped.end(); ++iter)
	{
		m_inventory.insert(*iter);
		m_owner->deapplyStats((*iter)->getStats());
	}
	m_equipped.clear();
	
	return res;
}

bool Inventory::isEquipped(Item *i)
{
	std::set< Item* >::const_iterator iter = m_equipped.find(i);
	return iter != m_equipped.end();
}

void Inventory::printInventory()
{
	printf("Owner: %s\n", m_owner->getName().c_str());
	printf("Equipped Items:\n");

	std::set< Item* >::iterator iter;
	for (iter = m_equipped.begin(); iter != m_equipped.end(); ++iter)
	{
		printf("  %s\n", (*iter)->getName().c_str());
	}

	printf("Non-equipped Items:\n");
	for (iter = m_inventory.begin(); iter != m_inventory.end(); ++iter)
	{
		printf("  %s\n", (*iter)->getName().c_str());
	}

	printf("Gold: %u\n", m_owner->getGold());

	printf("------------------\n\n");
}