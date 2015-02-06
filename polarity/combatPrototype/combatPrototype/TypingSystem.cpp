#include "TypingSystem.h"

#include <cassert>

const std::string TypingSystem::rank1[] =
{
	"arm", "air", "bed", "red", "head", "dead"
};

const std::string TypingSystem::rank2[] =
{
	"hello", "crime", "always", "capital", "adult", "backpack"
};

const std::string TypingSystem::rank3[] =
{
	"aircraft carrier", "cappuccino", "butterfly", "chocolate", "electricity", "programming"
};

const std::string TypingSystem::rank4[] =
{
	"he arrived late to the meeting",
	"she is not very nice to her friends",
	"i want my money back now",
	"and do not forget my black tshirt",
	"i cant cry and i cant see",
	"these sentences are fun to come up with"
};

const std::string TypingSystem::rank5[] =
{
	"another approach might be to use machine learning techniques",
	"even if the collision system does not change these techniques do not work well",
	"with this approach we can define a relationship between the two",
	"this shows how path finding space might be constructed with this method",
	"we can use a similar approach for tile based path finding",
	"notice that this modulo operation does not generate a truly uniform distribution"
};

TypingSystem::TypingSystem()
{
	m_inProgress = false;

	m_source = NULL;
	m_target = NULL;
	m_currentSpell = NULL;

	m_activePhrase = "";

	m_mistakes = 0;
}

void TypingSystem::invoke(EntityBase *source, EntityBase *target, Spell *s)
{
	assert(source);
	assert(target);
	assert(s);

	m_inProgress = true;

	m_source = source;
	m_target = target;
	m_currentSpell = s;

	// TODO: eventually, let's display the entire phrase on the screen, and then
	//		the 'progress' through the phrase, like Typing of the Dead. We'll need
	//		2 strings: originalPhrase, and currentPhrase.. or something.
	m_activePhrase = selectPhrase(s->m_difficulty);
	m_activePhraseLength = m_activePhrase.size();

	std::string str1 = "Casting: " + s->m_name + " on Target: " + m_target->m_name;
	std::string str2 = "Word: " + m_activePhrase;

	printf("%s\n%s\n", str1.c_str(), str2.c_str());

	m_mistakes = 0;
}

void TypingSystem::update(Uint16 unicode)
{
	if (m_inProgress == false)
		return;

	// TODO: need to allow #'s, capitals, punctuation, etc.
	if ((unicode >= (Uint16)'a' ) && 
		(unicode <= (Uint16)'z' ) ||
		(unicode == (Uint16)' ' ))
	{
		bool finished = judge((char)unicode);

		// Finished with the phrase. Apply all final results.
		if (finished)
		{
			m_inProgress = false;

			float accuracy = (float)m_activePhraseLength / (m_mistakes + m_activePhraseLength);

			printf("Casting!\n");
			printf("Mistakes: %i, Accuracy: %f\n", m_mistakes, accuracy);

			// Apply the effects of the spell.
			SpellSystem::getSingleton().apply(m_currentSpell, m_source, m_target, accuracy);
		}
		else
		{
			std::string str1 = "Word: " + m_activePhrase;
			printf("%s\n", str1.c_str());
		}
	}
}

bool TypingSystem::judge(char letter)
{
	if (letter == m_activePhrase[0])
		m_activePhrase = m_activePhrase.substr(1, m_activePhrase.size() - 1);
	else	// Mistake!
		++m_mistakes;

	// Finished w/ the phrase.
	if (m_activePhrase == "")
		return true;

	return false;
}

std::string TypingSystem::selectPhrase(Uint16 difficulty)
{
	int randVal = rand() % 6;
	std::string word;

	switch (difficulty)
	{
	case 0:
		word = rank1[randVal];
		break;
	case 1:
		word = rank2[randVal];
		break;

	case 2:
		word = rank3[randVal];
		break;

	case 3:
		word = rank4[randVal];
		break;

	case 4:
		word = rank5[randVal];
		break;
	}

	return word;
}