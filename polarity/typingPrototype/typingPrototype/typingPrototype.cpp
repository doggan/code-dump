// typingPrototype.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <Windows.h>

#include "SDL/SDL.h"

#include <iostream>
#include <string>
#include <cassert>
#include <time.h>

#include "wordList.h"

using namespace std;

#define SCREEN_WIDTH 500
#define SCREEN_HEIGHT 500
#define SCREEN_BPP 32

SDL_Surface *screen = NULL;

bool init() { 
	//Initialize all SDL subsystems 
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) { 
		return false; 
	} 
	
	//Set up the screen 
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE ); 
	
	//If there was an error in setting up the screen 
	if( screen == NULL ) { return false; } 
	
	//Set the window caption 
	SDL_WM_SetCaption( "Event test", NULL ); 
	
	//If everything initialized fine 
	return true;
}

#define NUM_SPELLS 1
#define NUM_RANKS 5

string spellList[NUM_SPELLS] =
{
	"Fire Ball",
//	"Heal"
};

int g_fireBallMaxDmg[] = {
	10, 100, 200, 400, 1000
};

int g_selectedSpell;
int g_selectedRank;

string g_activeWord;
int g_activeWordLength;
int g_numMistakes;

enum CURR_MODE
{
	SELECT_SPELL = 1,
	SELECT_RANK
};

CURR_MODE g_currMode;

string selectWord(int rank)
{
	int randVal = rand() % 6;
	string word;

	switch (rank)
	{
	case 0:
		word = rank0[randVal];
		break;

	case 1:
		word = rank1[randVal];
		break;

	case 2:
		word = rank2[randVal];
		break;

	case 3:
		word = rank3[randVal];
		break;

	case 4:
		word = rank4[randVal];
		break;
	}

	return word;
}

void judge(char letter)
{
	if (letter == g_activeWord[0])
		g_activeWord = g_activeWord.substr(1, g_activeWord.size() - 1);
	else	// mistake!
		++g_numMistakes;

}

int _tmain(int argc, _TCHAR* argv[])
{
	srand(time(NULL));

	// TODO: v1
	// X Select spell level (rank 1-5)
	// X Present the player w/ a sentence/word of that level difficulty
	// X Read in user input, parse for accuracy
	// X Based on accuracy (no backspace key), calculate damage dealt to NPC

	init();

	SDL_EnableUNICODE( SDL_ENABLE );

	bool quit = false;
	bool activeState = false;
	bool wordFinished = false;
	SDL_Event event;

	while (quit == false)
	{
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				if ((event.key.keysym.unicode >= (Uint16)'a' ) && 
					(event.key.keysym.unicode <= (Uint16)'z' ) ||
					(event.key.keysym.unicode == (Uint16)' ' ))
				{
					judge((char)event.key.keysym.unicode);
					cout << "Type: " << g_activeWord << endl;
					if (g_activeWord == "")
						wordFinished = true;
				}
				else if (( event.key.keysym.unicode >= (Uint16)'0' ) &&
						 ( event.key.keysym.unicode <= (Uint16)'9' ) )
				{
					if (activeState)
					{
						if (g_currMode == SELECT_SPELL)
						{
							char myChar = (char)event.key.keysym.unicode;
							g_selectedSpell = atoi(&myChar);
							g_currMode = SELECT_RANK;
							cout << "Select Rank (0-4):" << endl;
						}
						else if (g_currMode == SELECT_RANK)
						{
							char myChar = (char)event.key.keysym.unicode;
							g_selectedRank = atoi(&myChar);

							cout << "Casting " << spellList[g_selectedSpell] << " Rank " << g_selectedRank << "..." << endl;

							g_activeWord = selectWord(g_selectedRank);
							g_activeWordLength = (int)g_activeWord.size();

							cout << "Type: " << g_activeWord << endl;
						}
					}
				}
				else if (event.key.keysym.sym == SDLK_F1)
				{
					// reset
					wordFinished = false;
					activeState = true;
					g_numMistakes = 0;

					g_currMode = SELECT_SPELL;
					cout << "Select Spell:" << endl;
					for (int i = 0; i < NUM_SPELLS; ++i)
						cout << i << ": " << spellList[i] << endl;
				}
				else if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = true;
			}
			
			if (event.type == SDL_QUIT)
				quit = true;
		}

		// Casting complete.
		if (g_activeWord == "" && activeState && wordFinished)
		{
			float acc = (float)g_activeWordLength / (g_numMistakes + g_activeWordLength);
			int maxDmg = g_fireBallMaxDmg[g_selectedRank];

			cout << "Mistakes: " << g_numMistakes << endl;
			cout << "Accuracy: " << acc << endl;
			cout << "Max Dmg: " << maxDmg << endl;
			cout << "Actual Dmg: " << acc * maxDmg << endl;
			cout << "-------------------" << endl;
			activeState = false;
		}
	}

	// TODO: v2
	// Add in timer to time how long it takes to cast (from start of typing).
	// Display WPS speed
	// Add in an NPC that takes damage, does damage to player
	// Add a player heal spell

	SDL_EnableUNICODE( SDL_DISABLE ); 
	SDL_Quit();

	return 0;
}