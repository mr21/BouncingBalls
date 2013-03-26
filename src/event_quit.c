#include	"data.h"

void		event_quit(SDL_Event* e)
{
  (void)e;
  SDLazy_Quit(0);
}
