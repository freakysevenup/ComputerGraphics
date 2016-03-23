#include <SDL/SDL_net.h>

const int BUFFER_SIZE = sizeof(SDL_Event);

union Package
{
	SDL_Event e;					// The event
	char data[BUFFER_SIZE];	// The char * array filled with the same data as the SDL_Event
};