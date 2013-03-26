#ifndef			DATA_H
# define		DATA_H

# include		"SDLazy/SDLazy.h"

# define		GRAVITY_DEF	2500.f

enum
{
  FOOTBALL,
  BASKETBALL,
  BASEBALL,

  NB_BALLS
};

typedef			struct
{
  SDLazy_Sprite*	sprite;
  SDLazy_Anim*		anim;
  int			r;
  unsigned		id;
  v2f			dir;
  float			angularVel;
  v2i			mouseCoord;
  float			dissipation;
}			Ball;

typedef			struct
{
  SDLazy_Sprite*	sprite;
  double		speed;
}			Enemy;

typedef			struct
{
  unsigned		points;
  unsigned		nbEnemiesKilled;
  float			gravity;
  Uint32		bgColor;
  v2i			mouseRel;
  SDLazy_Surface*	srfEnemy;
  SDLazy_Surface*	srfBall;
  SDLazy_Surface*	srfBallAnim;
  SDLazy_Surface*	srfBalls[NB_BALLS];
  float			ballsDissipation[NB_BALLS];
  CList			balls;
  Ball*			ballClicked;
  CList			enemies;
}			Data;

void			core(void);
int			moveBall(Ball*);
void			render(void);

void			event_mousedown(SDL_Event*);
void			event_mouseup(SDL_Event*);
void			event_mousemove(SDL_Event*);
void			event_keydown(SDL_Event*);
void			event_keyup(SDL_Event*);
void			event_quit(SDL_Event*);

#endif
