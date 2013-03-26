#include	"data.h"

static int	collide_walls(Data* d, Ball* b)
{
  v2f*		pos = SDLazy_GetPos(b->sprite);
  int		collided = 0;

  if (pos->x < b->r || pos->x > SDLazy_GetWidth(NULL) - b->r)
    {
      b->dir.x *= -1;
      collided = 1;
    }
  if (pos->y < b->r || pos->y > SDLazy_GetHeight(NULL) - b->r)
    {
      b->dir.y *= -1;
      collided = 1;
    }
  if (collided)
    {
      v2f_mul_f(&b->dir, b->dissipation);
      if ((b->dir.y > -50. * b->dissipation && b->dir.y < 0.) && d->gravity > 100.)
	{
	  /*b->dir.y = 0.f;
	    b->angularVel = 0.f;*/
	}
    }
  return collided;
}

static int	collide_balls(Data* d, Ball* b0)
{
  Ball*		bn;
  v2f*		b0_pos = SDLazy_GetPos(b0->sprite);
  v2f*		bn_pos;
  CLink*	ln = CList_begin(&d->balls);
  v2f		n, g, v1, v2;

  for (; ln; ln = ln->next)
    if ((bn = ln->data) != b0)
      {
	bn_pos = SDLazy_GetPos(bn->sprite);
	if (POW2(b0->r + bn->r) >= POW2(bn_pos->x - b0_pos->x) + POW2(bn_pos->y - b0_pos->y))
	  {
	    /* calcul de la base */
	    n.x = (bn_pos->x - b0_pos->x) / (b0->r + bn->r);
	    n.y = (bn_pos->y - b0_pos->y) / (b0->r + bn->r);
	    g.x = -n.y;
	    g.y = +n.x;
	    /* calcul des "dir" dans cette base */
	    v1.x = n.x * b0->dir.x + n.y * b0->dir.y;
	    v1.y = g.x * b0->dir.x + g.y * b0->dir.y;
	    v2.x = n.x * bn->dir.x + n.y * bn->dir.y;
	    v2.y = g.x * bn->dir.x + g.y * bn->dir.y;
	    /* permute les coordonnees */
	    b0->dir.x = n.x * v2.x +  g.x * v1.y;
	    b0->dir.y = n.y * v2.x +  g.y * v1.y;
	    bn->dir.x = n.x * v1.x +  g.x * v2.y;
	    bn->dir.y = n.y * v1.x +  g.y * v2.y;
	    /*v2f_mul_f(&b0->dir, b0->dissipation);
	      v2f_mul_f(&bn->dir, bn->dissipation);*/
	    return 1;
	  }
      }
  return 0;
}

static void	gravity(Data* d, Ball* b)
{
  float		g = d->gravity * SDLazy_GetFrameTime();

  if (SDLazy_GetPosY(b->sprite) + (b->dir.y + g) * SDLazy_GetFrameTime()
      < SDLazy_GetHeight(NULL) - b->r)
    b->dir.y += g;
}

/* Ranger cette fonction dans la lib de vecteur */
static double	v2f_getAngle(v2f const* u, v2f const* v)
{
  double	s, c, nn =
    sqrt(POW2(u->x) + POW2(u->y)) *
    sqrt(POW2(v->x) + POW2(v->y));

  /*printf("nn:%+8.2f\n", nn);*/

  /*printf("u{%8.2f; %8.2f} v{%8.2f; %8.2f} nn:%8.2f", u->x, u->y, v->x, v->y, nn);*/
  if (fabs(nn) > 0.001)
    {
      c = (u->x * v->x + u->y * v->y) / nn;
      if (c >= 1.)
	c = 0.999;
      /*printf("  c:%5.2f  ", c);*/
      s = (u->x * v->y - u->y * v->x);
      return acos(c) * (s < 0 ? -1 : 1);
    }
  return 0.f;
}

static void	calc_angular_vel(Ball* b, v2f* dirBefore)
{
  double	ang;

  v2f_mul_f(dirBefore, -1.f);
  ang = v2f_getAngle(dirBefore, &b->dir);
  b->angularVel = ang * 0.01 * sqrt(POW2(b->dir.x) + POW2(b->dir.y));
}

int		moveBall(Ball* b)  
{
  Data*		d = SDLazy_GetData();
  v2f		dirBefore;
  int		ret;

  /*
  printf("id:%u | r:%2d | pos:{%+7.2f;%+7.2f} | posInt:{%+7.2f;%+7.2f} | dir:{%+7.2f;%+7.2f} | angularVel:%+5.2f\n",
	 b->id,
	 b->r,
	 SDLazy_GetPosX(b->sprite), SDLazy_GetPosY(b->sprite),
	 b->sprite->header.pos_rel.x,
	 b->sprite->header.pos_rel.y,
	 b->dir.x, b->dir.y,
	 b->angularVel,
	 b->mouseCoord.x, b->mouseCoord.y,
	 b->dissipation
	 );
  */

  if (b != d->ballClicked)
    {
      SDLazy_IncPosX(b->sprite, b->dir.x * SDLazy_GetFrameTime());
      SDLazy_IncPosY(b->sprite, b->dir.y * SDLazy_GetFrameTime());
      SDLazy_IncRot(b->sprite, b->angularVel * SDLazy_GetFrameTime());
      gravity(d, b);
    }
  dirBefore = b->dir;
  ret = collide_walls(d, b) || collide_balls(d, b);
  if (ret)
    calc_angular_vel(b, &dirBefore);
  return ret;
}
