#ifndef _Item_h_
#define _Item_h_

#include "Common.h"

class Item
{
public:
	enum SLOT
	{
		GENERAL,
		WEAPON,
		CHEST,
		HEAD
	};

	Item(uint id, const std::string &name);
	~Item();

	Item *create() const { return new Item(*this); }

	uint getID() { return m_id; }
	const std::string &getName() { return m_name; }
	const Stats &getStats() { return m_stats; }

	void setSlot(SLOT slot) { m_slot = slot; }
	void setStats(const Stats &stats) { m_stats = stats; }

	// TODO: returning 'bool' may be unnecessary!
	bool use(); // attempts to cast the spell/effect tied to this item

	bool isEquippable();
	
protected:
	uint m_id;
	std::string m_name;

	Stats m_stats;
	SLOT m_slot;

//	Spell *m_effect;
//	uint m_charges;

//	TODO: graphic (image for this item).
};

#endif // _Item_h_