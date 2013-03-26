#include	"data.h"

static int	_renderBall(Ball* b)
{
  SDLazy_Blit(b->sprite);
  SDLazy_Blit(b->anim);
  return CLIST_CONTINUE;
}

static int	_renderEnemies(Enemy* enemy)
{
  SDLazy_Blit(enemy->sprite);
  return CLIST_CONTINUE;
}

void		render(void)
{
  Data*		d = SDLazy_GetData();

  SDL_FillRect(SDLazy_GetScreen(), 0, d->bgColor);
  CList_foreach(&d->balls, _renderBall);
  CList_foreach(&d->enemies, _renderEnemies);
}
