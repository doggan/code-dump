#ifndef _NPCManager_h_
#define _NPCManager_h_

#include "Factory.h"
#include "Singleton.h"

// Forward declarations.
class NPC;

class NPCManager : public Factory< NPC >, public Singleton< NPCManager >
{
public:
	void loadNPCs(const std::string &fileName);
	void saveNPCs(const std::string &fileName);	// TODO:

	NPC *getObject(uint id);

private:
	struct Loot
	{
		uint id;
		float chance;
	};

	/* This is used to reference the NPC's data in the 'master list'. When the NPC is
	* actually generated in the world, this data is queried, random numbers are generated,
	* and the NPC is created with 'unique' data.
	*/
	struct NPCData
	{
		std::vector<Loot> lootVec;
		uint goldMax;
	};

	std::map< uint, NPCData > m_npcLootMap;
};

#endif // _NPCManager_h_