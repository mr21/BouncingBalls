#include	"data.h"

void		event_mousemove(SDL_Event* e)
{
  Data*		d = SDLazy_GetData();
  Ball*		b = d->ballClicked;
  v2f		pos_save;

  if (b)
    {
      d->mouseRel.x = e->motion.xrel;
      d->mouseRel.y = e->motion.yrel;
      b->angularVel = 0.f;
      b->dir.x = e->motion.xrel * 100.;
      b->dir.y = e->motion.yrel * 100.;
      pos_save = *SDLazy_GetPos(b->sprite);
      SDLazy_SetPosX(b->sprite, SDLazy_GetMouseX() - b->mouseCoord.x);
      SDLazy_SetPosY(b->sprite, SDLazy_GetMouseY() - b->mouseCoord.y);
      if (moveBall(b))
	{
	  SDLazy_SetPos(b->sprite, &pos_save);
	  d->ballClicked = NULL;
	}
    }
}

static int	_ballClicked(Ball* b)
{
  int		ret;
  v2i		v;

  v.x = SDLazy_GetMouseX() - SDLazy_GetPosX(b->sprite);
  v.y = SDLazy_GetMouseY() - SDLazy_GetPosY(b->sprite);
  ret = POW2(b->r) > POW2(v.x) + POW2(v.y);
  if (ret)
    {
      b->mouseCoord = v;
      b->dir.x = 0.f;
      b->dir.y = 0.f;
    }
  return ret;
}

void		event_mousedown(SDL_Event* e)
{
  Data*		d = SDLazy_GetData();
  (void)e;

  d->mouseRel.x = 0;
  d->mouseRel.y = 0;
  d->ballClicked = CLink_data(CList_ffind_front(&d->balls, _ballClicked));
  if (d->ballClicked)
    {
      SDLazy_AnimPause(d->ballClicked->anim);
    }
}

void		event_mouseup(SDL_Event* e)
{
  Data*		d = SDLazy_GetData();
  (void)e;

  if (d->ballClicked)
    {
      SDLazy_AnimPlay(d->ballClicked->anim);
      if (abs(d->mouseRel.x) > 2 && abs(d->mouseRel.y) > 2)
        {
          d->ballClicked->dir.x = d->mouseRel.x * 100;
          d->ballClicked->dir.y = d->mouseRel.y * 100;
        }
      d->ballClicked = NULL;
    }
}
