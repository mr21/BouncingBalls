#include	"data.h"
#include	<stdlib.h>
#include	<time.h>

static int	newBall(Data* d, int type)
{
  Ball*		b = malloc(sizeof *b);
  int		ret = b && CList_push_back(&d->balls, b, 0, free);
  static int	idMax;

  if (ret)
    {
      b->id = ++idMax;
      b->sprite = SDLazy_SpriteCreate(d->srfBall);
      b->anim = SDLazy_AnimCreate(d->srfBallAnim, 11, 0, ANIM_PLAYING);
      SDLazy_SetCenterAlign(b->anim, ALIGN_CENTER);
      b->r = SDLazy_GetWidth(b->sprite) / 2;
      SDLazy_SetCenterAlign(b->sprite, ALIGN_CENTER);
      SDLazy_SetPosX(b->sprite, 75 * idMax);
      SDLazy_SetPosY(b->sprite, 200 * (1 + idMax % 2));
      b->dir.x = 0.f;
      b->dir.y = 0.f;
      b->angularVel = 0.f;
      b->dissipation = d->ballsDissipation[type];
      b->mouseCoord.x = 0;
      b->mouseCoord.y = 0;
    }
  return ret;
}

int		data_init(Data* d)
{
  int		i, ret = 1;

  d->points = 0;
  d->nbEnemiesKilled = 0;

  d->gravity = GRAVITY_DEF;
  d->bgColor = SDL_MapRGB(SDLazy_GetScreen()->format, 20, 20, 20);

  d->srfEnemy    = SDLazy_SurfaceLoad("img/balls.png", SDLazy_Rect(105, 80, 32, 32));
  d->srfBall     = SDLazy_SurfaceLoad("img/balls.png", SDLazy_Rect( 10, 80, 71, 71));
  d->srfBallAnim = SDLazy_SurfaceLoad("img/balls.png", SDLazy_Rect( 10, 15, 35, 35));

  d->srfBalls[FOOTBALL]   = SDLazy_SurfaceLoad("img/football.png", NULL);
  d->srfBalls[BASKETBALL] = SDLazy_SurfaceLoad("img/disc.png", NULL);
  d->srfBalls[BASEBALL]   = SDLazy_SurfaceLoad("img/baseball.png", NULL);

  d->ballsDissipation[FOOTBALL] = .5;
  d->ballsDissipation[BASKETBALL] = .8;
  d->ballsDissipation[BASEBALL] = .3;
  d->ballClicked = NULL;
  CList_init(&d->balls);
  for (i = 0; ret && i < 3; ++i)
    ret = newBall(d, BASKETBALL);
  CList_init(&d->enemies);
  return ret;
}

int		main(int ac, char** av)
{
  Data		d;
  (void)ac;
  (void)av;

  srand(time(NULL));
  SDLazy_Init(1000, 600, SDL_HWSURFACE | SDL_DOUBLEBUF/* | SDL_FULLSCREEN*/, "BDBalls");
  if (data_init(&d))
    {
      SDLazy_AddEvent(SDL_MOUSEMOTION,		event_mousemove);
      SDLazy_AddEvent(SDL_MOUSEBUTTONDOWN,	event_mousedown);
      SDLazy_AddEvent(SDL_MOUSEBUTTONUP,	event_mouseup);
      SDLazy_AddEvent(SDL_KEYDOWN,		event_keydown);
      SDLazy_AddEvent(SDL_KEYUP,		event_keyup);
      SDLazy_AddEvent(SDL_QUIT,			event_quit);
      SDLazy_Loop(&d, core, render);
    }
  return 0;
}

int     WinMain(int ac, char** av)
{
    return main(ac, av);
}
