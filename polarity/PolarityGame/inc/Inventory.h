#ifndef _Inventory_h_
#define _Inventory_h_

#include "Common.h"

#include <set>

// Forward declarations.
class EntityBase;
class Item;

class Inventory
{
public:
	Inventory(EntityBase *owner);
	~Inventory();

protected:
	Inventory(const Inventory &i) {}
	Inventory &operator=(const Inventory &i) { return *this; }

public:
	std::set< Item* > getEquippedItems() { return m_equipped; }
	std::set< Item* > getInventoryItems() { return m_inventory; }

	void addItem(Item *item);
	void removeItem(Item *item);

	void unequip(Item *i);
	std::set< Item* > unequipAll();

	void equip(Item *i);
	void equip(const std::set< Item* > &items);

	bool isEquipped(Item *i);

	// TODO: this is for debugging.
	void printInventory();

private:
	std::set< Item* > m_equipped;
	std::set< Item* > m_inventory;

	EntityBase *m_owner;
};

#endif // _Inventory_h_