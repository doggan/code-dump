//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <vector>

using namespace std;

vector< vector< vector< SDL_Surface* > > > m_world;

//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;

//The surfaces
SDL_Surface *screen = NULL;

SDL_Surface *player = NULL;

struct coord
{
	unsigned int x, y, z;
};

coord g_currPos;
coord g_playerPos;

SDL_Surface *load_image( std::string filename ) 
{
	//The image that's loaded
	SDL_Surface* loadedImage = NULL;

	//The optimized image that will be used
	SDL_Surface* optimizedImage = NULL;

	//Load the image using SDL_image
	loadedImage = IMG_Load( filename.c_str() );

	//If the image loaded
	if( loadedImage != NULL )
	{
		//Create an optimized image
		optimizedImage = SDL_DisplayFormat( loadedImage );

		//Free the old image
		SDL_FreeSurface( loadedImage );
	}

	//Return the optimized image
	return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
	//Rectangle to hold the offsets
	SDL_Rect offset;

	//Get offsets
	offset.x = x;
	offset.y = y;

	//Blit the surface
	SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
	//Initialize all SDL subsystems
	if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		return false;    
	}

	//Set up the screen
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

	//If there was an error in setting up the screen
	if( screen == NULL )
	{
		return false;    
	}

	//Set the window caption
	SDL_WM_SetCaption( "PNG test", NULL );

	//If everything initialized fine
	return true;    
}

void clean_up()
{
	//Quit SDL
	SDL_Quit();    
}

void moveNorth()
{
	g_currPos.x -= 1;
}

void moveSouth()
{
	g_currPos.x += 1;
}

void moveEast()
{
	g_currPos.y += 1;
}

void moveWest()
{
	g_currPos.y -= 1;
}

void moveUp()
{
	g_currPos.z += 1;
}

void moveDown()
{
	g_currPos.z -=1;
}

int main( int argc, char* args[] )
{
	for (int i = 0; i < 3; ++i)
	{
		vector < vector < SDL_Surface*> > colVec;
		for (int j = 0; j < 3; ++j)
		{
			vector < SDL_Surface* > depthVec;
			for (int k = 0; k < 3; ++k)
			{
				depthVec.push_back(NULL);
			}
			colVec.push_back(depthVec);
		}
		m_world.push_back(colVec);
	}
	
	//Initialize
	if( init() == false )
	{
		return 1;    
	}

	g_currPos.x = 0;
	g_currPos.y = 0;
	g_currPos.z = 0;

	m_world[0][0][0] = load_image( "1.png" );
	m_world[1][0][0] = load_image( "2.png" );

	m_world[1][0][1] = load_image( "3.png" );

	m_world[1][0][2] = load_image( "4.png" );

	player = load_image("foo.png");
	Uint32 colorkey = SDL_MapRGB( player->format, 0, 0xFF, 0xFF );
	SDL_SetColorKey( player, SDL_SRCCOLORKEY, colorkey );

	bool quit = false;
	SDL_Event event;

	while (quit == false)
	{
		SDL_Flip( screen );

		apply_surface( 0, 0, m_world[g_currPos.x][g_currPos.y][g_currPos.z], screen );
		apply_surface( g_playerPos.x, g_playerPos.y, player, screen);

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym)
				{
				case SDLK_UP: moveNorth(); break;
				case SDLK_DOWN: moveSouth(); break;
				case SDLK_PAGEUP: moveUp(); break;
				case SDLK_PAGEDOWN: moveDown(); break;
					
				case SDLK_ESCAPE: quit = true; break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x = event.button.x;
					int y = event.button.y;

					g_playerPos.x = x;
					g_playerPos.y = y;

					printf("%i, %i\n", x, y);
				}
			}
		}
	}

	//Free the surface and quit SDL
	clean_up();

	return 0;    
}
