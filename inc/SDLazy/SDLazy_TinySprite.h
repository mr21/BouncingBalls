#ifndef			SDLAZY_TINYSPRITE
# define		SDLAZY_TINYSPRITE

# include		"Math/myMath.h"
# include		<SDL/SDL.h>

typedef			struct
{
  SDL_Rect		r;
  SDL_Surface*		srf;
  double		rad_abs;
  v2f			scl_abs;
}			SDLazy_TinySprite;

void			SDLazy_TinySpriteInit(SDLazy_TinySprite*, SDL_Rect*);

#endif
