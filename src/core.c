#include	"data.h"
#include	<stdlib.h>

int		dotInSquare(v2f* dot, v2f sqCoord, int sqW, int sqH)
{
  return
    dot->x >= sqCoord.x && dot->x < sqCoord.x + sqW &&
    dot->y >= sqCoord.y && dot->y < sqCoord.y + sqH;
}

static Ball*	ballCurr;
static int	_ballCollideEnemies(Enemy* enemy)
{
  Data*		d = SDLazy_GetData();
  v2f		bPos = *SDLazy_GetPos(ballCurr->sprite);
  v2f*		ePos = SDLazy_GetPos(enemy->sprite);
  int		bW = SDLazy_GetWidth(ballCurr->sprite);
  int		bH = SDLazy_GetHeight(ballCurr->sprite);
  int		eW = SDLazy_GetWidth(enemy->sprite);
  int		eH = SDLazy_GetHeight(enemy->sprite);
  int		ret;

  bPos.x -= bW / 2.0;
  bPos.y -= bH / 2.0;
  ret =
    ballCurr != d->ballClicked &&
    (dotInSquare(ePos                            , bPos, bW, bH) ||
     dotInSquare(v2f_(ePos->x + eW, ePos->y     ), bPos, bW, bH) ||
     dotInSquare(v2f_(ePos->x     , ePos->y + eH), bPos, bW, bH) ||
     dotInSquare(v2f_(ePos->x + eW, ePos->y + eH), bPos, bW, bH));
  if (ret)
    {
      d->points += enemy->speed;
      ++d->nbEnemiesKilled;
      printf("pts: %6u\n", d->points);
    }
  return ret ? CLIST_ERASE : CLIST_CONTINUE;
}

static int	_coreBall(Ball* b)
{
  Data*		d = SDLazy_GetData();
  v2f		pos_save;

  pos_save = *SDLazy_GetPos(b->sprite);
  if (moveBall(b))
    SDLazy_SetPos(b->sprite, &pos_save);
  SDLazy_SetPos(b->anim, SDLazy_GetPos(b->sprite));
  SDLazy_SetRot(b->anim, SDLazy_GetRot(b->sprite));
  ballCurr = b;
  CList_foreach(&d->enemies, _ballCollideEnemies);
  return CLIST_CONTINUE;
}

Enemy*		createEnemy(SDLazy_Surface* srf)
{
  Enemy*	enemy = malloc(sizeof *enemy);

  if (enemy)
    {
      enemy->sprite = SDLazy_SpriteCreate(srf);
      enemy->speed = 10.0 + rand() % 100;
      SDLazy_SetPosX(enemy->sprite, rand() % (int)(SDLazy_GetWidth(NULL) - SDLazy_GetWidth(srf)));
      SDLazy_SetPosY(enemy->sprite, -SDLazy_GetHeight(srf));
    }
  return enemy;
}

void		destroyEnemy(Enemy* enemy)
{
  free(enemy->sprite);
}

static void	_newEnemy(Data* d)
{
  static double	timeBefore = 0.0;
  static double	freq = 1.0;
  double	time = SDLazy_GetTotalTime();
  Enemy*	enemy;

  if (time - timeBefore > freq &&
      (enemy = createEnemy(d->srfEnemy)))
    {
      if (freq > 0.08)
	freq /= 1.02;
      timeBefore = time;
      CList_push_front(&d->enemies, enemy, 0, destroyEnemy);
    }
}

static int	_coreEnemies(Enemy* enemy)
{
  Data*		d = SDLazy_GetData();

  SDLazy_IncPosY(enemy->sprite, enemy->speed * SDLazy_GetFrameTime());
  if (SDLazy_GetPosY(enemy->sprite) > SDLazy_GetHeight(NULL))
    {
      printf("Enemies killed : %u\n", d->nbEnemiesKilled);
      puts("GameOver");
      SDLazy_Quit(NULL);
    }
  return CLIST_CONTINUE;
}

void		core(void)
{
  Data*		d = SDLazy_GetData();

  _newEnemy(d);
  CList_foreach(&d->balls, _coreBall);
  CList_foreach(&d->enemies, _coreEnemies);
}
