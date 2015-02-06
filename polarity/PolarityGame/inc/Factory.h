#ifndef _Factory_h_
#define _Factory_h_

#include "Utilities.h"

// Must included the header files for objects that will use this class.
// This prevents memory leak, with 'warning C4150'.
#include "Item.h"
#include "NPC.h"

#include <map>

template <class T>
class Factory
{
private:
	typedef std::map<uint, T*> MapIntT;
	MapIntT m_objects;

public:
	Factory() {}
	virtual ~Factory() {
		// Free up allocated events.
		typename MapIntT::iterator iter;
		for (iter = m_objects.begin(); iter != m_objects.end(); ++iter)
			delete iter->second;
	}

	// NOTE: A new heap-object is created and stored! The originally passed object can be safetly deleted.
	void addObject(T *e)
	{
		T *e2 = e->create();
		m_objects.insert(std::make_pair(e2->getID(), e2));
	}

	void removeObject(T *e)
	{
		m_objects.erase(e->getID());
	}

	// NOTE: A new heap-object is created and returned!
	virtual T *getObject(uint id)
	{
		typename MapIntT::const_iterator iter = m_objects.find(id);

		if (iter == m_objects.end())
			EXCEPTION(Exception::ERR_INVALID_PARAM, "Factory::getObject()", "Object id (" + toStr(id) + ") does not exist;");

		T *e = iter->second;
		return e->create();
	}

	// Unique id generation.
	uint newID()
	{
		uint walk = 0;

		while (true)
		{
			// Found a new key.
			if (m_objects.find(walk) == m_objects.end())
			{
				return walk;
			}

			++walk;
		}
	}
};

#endif // _Factory_h_