#ifndef _ItemManager_h_
#define _ItemManager_h_

#include "Factory.h"
#include "Singleton.h"

// Forward declarations.
class Item;

class ItemManager : public Factory< Item >, public Singleton< ItemManager >
{
public:
	void loadItems(const std::string &fileName);
};

#endif // _ItemManager_h_