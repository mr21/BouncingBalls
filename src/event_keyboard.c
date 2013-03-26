#include	"data.h"

void		event_keydown(SDL_Event* e)
{
  Data*		d = SDLazy_GetData();

  switch (e->key.keysym.sym)
    {
    case SDLK_ESCAPE:	SDLazy_Quit(0);						break;
    case 'g':		d->gravity = 0.;					break;
    case ' ':		SDL_FillRect(SDLazy_GetScreen(), 0, d->bgColor);	break;
    default:									break;
    }
}

void		event_keyup(SDL_Event* e)
{
  Data*		d = SDLazy_GetData();

  switch (e->key.keysym.sym)
    {
    case 'g':		d->gravity = GRAVITY_DEF;				break;
    default:									break;
    }
}
