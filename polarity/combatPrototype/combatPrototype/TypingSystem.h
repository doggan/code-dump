#ifndef _TypingSystem_h_
#define _TypingSystem_h_

#include "Singleton.h"
#include "EntityBase.h"
#include "SpellSystem.h"

#include "SDL/SDL.h"

#include <string>

class TypingSystem : public Singleton<TypingSystem>
{
public:
	TypingSystem();
	virtual ~TypingSystem() {}

	void invoke(EntityBase *source, EntityBase *target, Spell *s);
	void update(Uint16 unicode);

private:
	std::string selectPhrase(Uint16 difficulty);

	// Returns true if finished w/ the phrase, false if we need to keep going.
	bool judge(char letter);

	bool m_inProgress;
	EntityBase *m_source;
	EntityBase *m_target;
	Spell *m_currentSpell;

	std::string m_activePhrase;
	Uint16 m_activePhraseLength;

	Uint16 m_mistakes;

	static const std::string rank1[];
	static const std::string rank2[];
	static const std::string rank3[];
	static const std::string rank4[];
	static const std::string rank5[];
};

#endif // _TypingSystem_h_