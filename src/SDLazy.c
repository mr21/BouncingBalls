#include	"CList.h"

size_t		CList_size (CList const* li)           { return li ? li->size  : 0;             }
int		CList_empty(CList const* li)           { return li ? !li->size : 1;             }
CLink*		CList_begin(CList const* li)           { return li ? li->begin : NULL;          }
CLink*		CList_end  (CList const* li)           { return li ? li->end   : NULL;          }
CLink*          CList_next (CList const* li, size_t n) { return CLink_next(CList_begin(li), n); }
CLink*          CList_prev (CList const* li, size_t n) { return CLink_prev(CList_end(li), n);   }
#include	"CList.h"

CList*		CList_move_back(CLink* lna, CLink* lnb, CList* li)
{
  CList		tmpli;

  CList_extract(lna, lnb, &tmpli);
  return CList_merge_back(li, &tmpli);
}

CList*		CList_move_front(CLink* lna, CLink* lnb, CList* li)
{
  CList		tmpli;

  CList_extract(lna, lnb, &tmpli);
  return CList_merge_front(li, &tmpli);
}

CList*		CList_move_after(CLink* lna, CLink* lnb, CLink* lnc)
{
  CList		tmpli;

  CList_extract(lna, lnb, &tmpli);
  return CList_merge_after(lnc, &tmpli);
}

CList*		CList_move_before(CLink* lna, CLink* lnb, CLink* lnc)
{
  CList		tmpli;

  CList_extract(lna, lnb, &tmpli);
  return CList_merge_before(lnc, &tmpli);
}
#include	<stdlib.h>
#include	"CList.h"

void		CList_init(CList* li)
{
  li->begin = li->end = NULL;
  li->size = 0;
}
#include	<stdlib.h>
#include	"CList.h"

CLink*		CList_pop_front(CList* li)
{
  CLink*	ln = li->begin;

  if (!ln)
    return NULL;
  --li->size;
  if (!(li->begin = ln->next))
    li->end = NULL;
  else
    li->begin->prev = NULL;
  if (ln->free)
    ln->free(ln->data);
  free(ln);
  return li->begin;
}

CLink*		CList_pop_back(CList* li)
{
  CLink*	ln = li->end;

  if (!ln)
    return NULL;
  --li->size;
  if (!(li->end = ln->prev))
    li->begin = NULL;
  else
    li->end->next = NULL;
  if (ln->free)
    ln->free(ln->data);
  free(ln);
  return li->end;
}

CLink*		CList_pop(CLink* ln)
{
  CList*	li = CLink_list(ln);
  CLink*	ln_ret;

  if (!li)
    return NULL;
  if (ln == li->begin)
    return CList_pop_front(li);
  if (ln == li->end)
    return CList_pop_back(li), NULL;
  ln->prev->next = ln_ret = ln->next;
  ln->next->prev = ln->prev;
  --li->size;
  if (ln->free)
    ln->free(ln->data);
  free(ln);
  return ln_ret;
}

CLink*		CList_popn(CLink* a, CLink* b)
{
  CList*	li = a->list;
  CLink*	ln = a, *tmp, *b_next = b->next;

  if (!a->prev)
    li->begin = b->next;
  else
    a->prev->next = b->next;
  if (!b->next)
    li->end = a->prev;
  else
    b->next->prev = a->prev;
  for (; ln != b_next; ln = tmp)
    {
      tmp = ln->next;
      if (ln->free)
        ln->free(ln->data);
      free(ln);
      --li->size;
    }
  return tmp;
}
#include	<stdlib.h>
#include	"CList.h"

void		CList_extract(CLink* lna, CLink* lnb, CList* newli)
{
  CList*	li = lna->list;

  newli->begin = lna;
  newli->end = lnb;
  newli->size = CList_count_n_li(lna, lnb, newli);
  li->size -= newli->size;
  if (lna->prev)
    lna->prev->next = lnb->next;
  else
    li->begin = lnb->next;
  if (lnb->next)
    lnb->next->prev = lna->prev;
  else
    li->end = lna->prev;
  lna->prev = lnb->next = NULL;
}

CList*		CList_cut(CLink* lna, CLink* lnb)
{
  CList*	la = malloc(sizeof *la);

  if (la)
    CList_extract(lna, lnb, la);
  return la;
}

CList*		CList_cut_back(CLink* ln)  { return CList_cut(ln, ln->list->end);   }
CList*		CList_cut_front(CLink* ln) { return CList_cut(ln->list->begin, ln); }
#include	<stdlib.h>
#include	"CList.h"

void		CList_clear(CList* li)
{
  CLink*	ln = li->begin;
  CLink*	tmp;

  for (; ln; ln = tmp)
    {
      tmp = ln->next;
      if (ln->free)
	ln->free(ln->data);
      free(ln);
    }
  li->begin = li->end = NULL;
  li->size = 0;
}
#include	"CList.h"

void		CList_foreach(CList* li, int (*f)())
{
  CLink*	ln = CList_begin(li);

  while (ln)
    switch (f(ln->data))
      {
      case CLIST_CONTINUE:	ln = ln->next;		break;
      case CLIST_ERASE:		ln = CList_pop(ln);	break;
      case CLIST_ERASE_STOP:	CList_pop(ln);
      default:			return;
      }
}
#include	"CList.h"

CLink*		CList_pfind_after(CLink const* ln, void const* data)
{
  for (; ln && ln->data != data; ln = ln->next);
  return (CLink*)ln;
}

CLink*		CList_pfind_before(CLink const* ln, void const* data)
{
  for (; ln && ln->data != data; ln = ln->prev);
  return (CLink*)ln;
}

CLink*		CList_pfind_front(CList const* li, void const* data)
{
  return CList_pfind_after(CList_begin(li), data);
}

CLink*		CList_pfind_back(CList const* li, void const* data)
{
  return CList_pfind_before(CList_end(li), data);
}

CLink*          CList_ffind_after(CLink const* ln, int (*f)())
{
  for (; ln && f(ln->data) == CLIST_CONTINUE; ln = ln->next);
  return (CLink*)ln;
}

CLink*          CList_ffind_before(CLink const* ln, int (*f)())
{
  for (; ln && f(ln->data) == CLIST_CONTINUE; ln = ln->prev);
  return (CLink*)ln;
}

CLink*		CList_ffind_front(CList const* li, int (*f)())
{
  return CList_ffind_after(CList_begin(li), f);
}

CLink*		CList_ffind_back(CList const* li, int (*f)())
{
  return CList_ffind_before(CList_end(li), f);
}
#include	<stdlib.h>
#include	"CList.h"

static void	add(CList* li, CList* la)
{
  CLink*	ln = li->begin;

  for (; ln; ln = ln->next)
    ln->list = li;
  li->size += la->size;
  CList_init(la);
}

CList*		CList_merge_back(CList* li, CList* la)
{
  if (li != la && la && la->size)
    {
      if (li->size)
	li->end->next = la->begin;
      else
	li->begin = la->begin;
      la->begin->prev = li->end;
      li->end = la->end;
      add(li, la);
    }
  return li;
}

CList*		CList_merge_front(CList* li, CList* la)
{
  if (li != la && la && la->size)
    {
      if (li->size)
	li->begin->prev = la->end;
      else
	li->end = la->end;
      la->end->next = li->begin;
      li->begin = la->begin;
      add(li, la);
    }
  return li;
}

CList*		CList_merge_after(CLink* ln, CList* la)
{
  CList*	li = CLink_list(ln);

  if (li && ln == li->end)
    return CList_merge_back(li, la);
  if (li != la && la && la->size && ln)
    {
      la->begin->prev = ln;
      la->end->next = ln->next;
      ln->next->prev = la->end;
      ln->next = la->begin;
      add(li, la);
    }
  return li;
}

CList*		CList_merge_before(CLink* ln, CList* la)
{
  CList*	li = CLink_list(ln);

  if (li && ln == li->begin)
    return CList_merge_front(li, la);
  if (li != la && la && la->size && ln)
    {
      la->begin->prev = ln->prev;
      la->end->next = ln;
      ln->prev->next = la->begin;
      ln->prev = la->end;
      add(li, la);
    }
  return li;
}
#include	"CList.h"

CList*		CLink_list(CLink const* ln)      { return ln ? ln->list : NULL; }
void*		CLink_data(CLink const* ln)      { return ln ? ln->data : NULL; }
void		(*CLink_free(CLink const* ln))() { return ln ? ln->free : NULL; }

CLink*		CLink_prev(CLink const* ln, size_t n)
{
  for (; ln && n; ln = ln->prev, --n);
  return (CLink*)ln;
}

CLink*		CLink_next(CLink const* ln, size_t n)
{
  for (; ln && n; ln = ln->next, --n);
  return (CLink*)ln;
}
#include	<stdlib.h>
#include	<string.h>
#include	"CList.h"

static CLink*	newLink(CList* li, void* data, size_t sz, void (*destr)())
{
  CLink*	new = malloc(sizeof(*new) + sz);

  if (new)
    {
      new->list = li;
      new->free = destr;
      new->prev = new->next = NULL;
      if (!sz)
	new->data = data;
      else
	{
	  new->data = &new->data + 1;
	  data ? memcpy(new->data, data, sz) :
	    memset(new->data, 0, sz);
	}
      if (++li->size == 1)
	li->begin = li->end = new;
    }
  return new;
}

CLink*		CList_push_front(CList* li, void* data, size_t sz, void (*destr)())
{
  CLink*	new = newLink(li, data, sz, destr);

  if (new && li->size > 1)
    {
      new->next = li->begin;
      li->begin->prev = new;
      li->begin = new;
    }
  return new;
}

CLink*		CList_push_back(CList* li, void* data, size_t sz, void (*destr)())
{
  CLink*	new = newLink(li, data, sz, destr);

  if (new && li->size > 1)
    {
      new->prev = li->end;
      li->end->next = new;
      li->end = new;
    }
  return new;
}

CLink*		CList_push_after(CLink* ln, void* data, size_t sz, void (*destr)())
{
  CList*	li = ln->list;
  CLink*	new;

  if (ln == li->end)
    return CList_push_back(li, data, sz, destr);
  if ((new = newLink(li, data, sz, destr)))
    {
      new->prev = ln;
      new->next = ln->next;
      ln->next->prev = new;
      ln->next = new;
    }
  return new;
}

CLink*		CList_push_before(CLink* ln, void* data, size_t sz, void (*destr)())
{
  CList*	li = ln->list;
  CLink*	new;

  if (ln == li->begin)
    return CList_push_front(li, data, sz, destr);
  if ((new = newLink(li, data, sz, destr)))
    {
      new->prev = ln->prev;
      new->next = ln;
      ln->prev->next = new;
      ln->prev = new;
    }
  return new;
}
#include	"CList.h"

size_t		CList_count(CLink const* a, CLink const* b)
{
  size_t	n = 1;

  for (; a && a != b; a = a->next)
    ++n;
  return n;
}

size_t		CList_count_n_li(CLink* a, CLink* b, CList const* li)
{
  size_t	s = 0;

  do
    ++s, a->list = (CList*)li;
  while (a != b && (a = a->next));
  return s;
}
#include		"Math/myMath.h"
#include		<stdlib.h>

int			fequal(double n, double x, double i)
{
  return n > x - i && n < x + i;
}

double			mycos(double rad)
{
  if (fequal(rad, 0, .001))
    return 1;
  if (fequal(fabs(rad), M_PI_2, .001))
    return 0;
  if (fequal(fabs(rad), M_PI, .001))
    return -1;
  return cos(rad);
}

double			mysin(double rad)
{
  if (fequal(rad, 0, .001) || fequal(fabs(rad), M_PI, .001))
    return 0;
  if (fequal(rad, M_PI_2, .001))
    return 1;
  if (fequal(fmod(rad, M_PI_2), -M_PI_2, .001))
    return -1;
  return sin(rad);
}

v2i*			v2i_(int x, int y)
{
  static v2i		v;

  v.x = x;
  v.y = y;
  return &v;
}

v2f*			v2f_(float x, float y)
{
  static v2f		v;

  v.x = x;
  v.y = y;
  return &v;
}

m4f*			m4f_(float a, float b, float c, float d)
{
  static m4f		m;

  m.a = a;
  m.b = b;
  m.c = c;
  m.d = d;
  return &m;
}

void			v2f_mul_f(v2f* v, float f)
{
  v->x *= f;
  v->y *= f;
}

void			v2f_mul_m4f(v2f* v, m4f const* m)
{
  double		tmpX = v->x;

  v->x = v->x * m->a + v->y * m->b;
  v->y = tmpX * m->c + v->y * m->d;
}
#include		"SDLazy/SDLazy.h"

SDLazy			g_sdlazy;

static void		_initEvents(void)
{
  if (g_sdlazy.flags | SDL_RESIZABLE)
    SDLazy_AddEvent(SDL_VIDEORESIZE,	SDLazy_ResizeEvent);
  SDLazy_AddEvent(SDL_MOUSEMOTION,	SDLazy_MouseMoveEvent);
  SDLazy_AddEvent(SDL_MOUSEBUTTONDOWN,	SDLazy_ClicDownEvent);
  SDLazy_AddEvent(SDL_MOUSEBUTTONUP,	SDLazy_ClicUpEvent);
}

static void		_initObjects(void)
{
  SDLazy_InitSurfaces(&g_sdlazy.surfaces);
  SDLazy_InitAnims(&g_sdlazy.anims);
  SDLazy_InitButtons(&g_sdlazy.buttons);
  SDLazy_InitCursors(&g_sdlazy.cursors);
}

void			SDLazy_Init(int w, int h, Uint32 flags, const char* title)
{
  memset(&g_sdlazy, 0, sizeof(g_sdlazy));
  if (SDL_Init(SDL_INIT_VIDEO) == -1)
    SDLazy_Quit("SDL_Init failed");
  if (!(g_sdlazy.scr = SDL_SetVideoMode(w, h, 32, flags)))
    SDLazy_Quit("SDL_SetVideoMode failed");
  g_sdlazy.flags = flags;
  g_sdlazy.title = title;
  SDLazy_SetFPS(500);
  SDLazy_SetFPSMax(90);
  SDL_WM_SetCaption(title, NULL);
  _initObjects();
  _initEvents();
}
#include		"SDLazy/SDLazy.h"
#include		<stdio.h>

void			SDLazy_Quit(char* msg)
{
  SDLazy_FreeSurfaces(&g_sdlazy.surfaces);
  SDLazy_FreeCursors(&g_sdlazy.cursors);
  SDLazy_FreeButtons(&g_sdlazy.buttons);
  SDL_Quit();
  if (msg)
    fprintf(stderr, "SDLazy-Error: %s (%s)\n", msg, SDL_GetError());
  free(g_sdlazy.titleFPS);
  exit(0);
}
#include	"SDLazy/SDLazy.h"

void*		SDLazy_GetData(void)      { return g_sdlazy.data; }
void		SDLazy_SetData(void* d)   { g_sdlazy.data = d; }
SDL_Surface*	SDLazy_GetScreen(void)    { return g_sdlazy.scr; }
double		SDLazy_GetFrameTime(void) { return g_sdlazy.frameTime; }
double		SDLazy_GetTotalTime(void) { return g_sdlazy.totalTime; }

void		SDLazy_SetFPS(Uint16 delay)     { g_sdlazy.fpsDelay = delay; }
void		SDLazy_SetFPSMax(Uint16 fpsMax) { g_sdlazy.delay    = fpsMax ? 1.0 / fpsMax * 1000 : 0; }

v2i*		SDLazy_GetMouse(void)  { return &g_sdlazy.mouse; }
int		SDLazy_GetMouseX(void) { return g_sdlazy.mouse.x; }
int		SDLazy_GetMouseY(void) { return g_sdlazy.mouse.y; }
#include	"SDLazy/SDLazy.h"

void		SDLazy_Fullscreen(void)
{
  SDL_Surface*	tmp;

  g_sdlazy.flags = (g_sdlazy.flags | SDL_FULLSCREEN) ?
    g_sdlazy.flags ^ SDL_FULLSCREEN :
    g_sdlazy.flags | SDL_FULLSCREEN;
  tmp = SDL_SetVideoMode(SDLazy_GetWidth(0), SDLazy_GetHeight(0), 32, g_sdlazy.flags);
  if (!tmp)
    SDLazy_Quit("SDL_SetVideoMode failed");
  SDL_FreeSurface(g_sdlazy.scr);
  g_sdlazy.scr = tmp;
}
#include			"SDLazy/SDLazy.h"

static void			_calcFrametime(void)
{
  static Uint32			ticks;
  Uint32			tmp = ticks;

  ticks = SDL_GetTicks();
  g_sdlazy.frameTime = (ticks - tmp) / 1000.0;
  g_sdlazy.totalTime = ticks / 1000.0;
}

static void			_fps(void)
{
  static int			ticks;

  if (!g_sdlazy.fpsDelay || SDL_GetTicks() - ticks < g_sdlazy.fpsDelay)
    return;
  if (!g_sdlazy.titleFPS && !(g_sdlazy.titleFPS = malloc(strlen(g_sdlazy.title) + 16)))
    {
      g_sdlazy.fpsDelay = 0;
      return;
    }
  ticks = SDL_GetTicks();
  sprintf(g_sdlazy.titleFPS, "%s %4d fps", g_sdlazy.title, (int)(1 / g_sdlazy.frameTime));
  SDL_WM_SetCaption(g_sdlazy.titleFPS, NULL);
}

static void			_delay(void)
{
  static Uint32			ticksPrev;
  Uint32			ticks;

  if (g_sdlazy.delay)
    {
      ticks = SDL_GetTicks();
      if (ticks - ticksPrev > g_sdlazy.delay)
	ticksPrev = ticks;
      else
	SDL_Delay(g_sdlazy.delay - (ticks - ticksPrev));
    }
}

static void			_nothing(void) {}

void				SDLazy_Loop(void* data, void (*core)(void), void (*render)(void))
{
  if (!core)
    core = _nothing;
  if (!render)
    render = _nothing;
  g_sdlazy.data = data;
  for (;;)
    {
      _calcFrametime();
      _fps();
      SDLazy_EventsUpdate();
      (*core)();
      (*render)();
      SDLazy_AnimsUpdate(&g_sdlazy.anims);
      SDL_Flip(g_sdlazy.scr);
      _delay();
    }
}
#include			<SDL/SDL.h>

void				SDLazy_SetPix(SDL_Surface *srf, int x, int y, Uint32 px)
{
  int				bpp = srf->format->BytesPerPixel;
  Uint8*			p = (Uint8*)srf->pixels + y * srf->pitch + x * bpp;

  switch (bpp)
    {
    case 1: *p = px; break;
    case 2: *(Uint16 *)p = px; break;
    case 4: *(Uint32 *)p = px; break;
    case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
	{
	  p[0] = (px >> 16) & 0xff;
	  p[1] = (px >> 8) & 0xff;
	  p[2] = px & 0xff;
	}
      else
	{
	  p[0] = px & 0xff;
	  p[1] = (px >> 8) & 0xff;
	  p[2] = (px >> 16) & 0xff;
	}
    }
}

void				SDLazy_SetPix_Secure(SDL_Surface *srf, int x, int y, Uint32 px)
{
  if (x >= 0 && x < srf->w && y >= 0 && y < srf->h)
    SDLazy_SetPix(srf, x, y, px);
}
#include	"SDLazy/SDLazy.h"

Uint32		SDLazy_GetPix(SDL_Surface* srf, int x, int y)
{
  int		bpp = srf->format->BytesPerPixel;
  Uint8*	p = (Uint8*)srf->pixels + y * srf->pitch + x * bpp;

  switch (bpp)
    {
    case 1: return *p;
    case 2: return *(Uint16*)p;
    case 4: return *(Uint32*)p;
    }
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
    return p[0] << 16 | p[1] << 8 | p[2];
  return p[0] | p[1] << 8 | p[2] << 16;
}

Uint32		SDLazy_GetPix_secure(SDL_Surface* srf, int x, int y)
{
  return x < 0 || x >= srf->w || y < 0 || y >= srf->h ?
    0 : SDLazy_GetPix(srf, x, y);
}
#include		"SDLazy/SDLazy.h"
#include		"Math/myMath.h"

static double		_get_angle(int x, int y, double r)
{
  if (x > r / -2.0 && x < r / 2.0)
    {
      if (y < 0)
        return 2 * M_PI - acos(x / r);
      return acos(x / r);
    }
  if (x < 0)
    return M_PI - asin(y / r);
  if (y < 0)
    return 2 * M_PI + asin(y / r);
  return asin(y / r);
}

static void		_circle_1px(SDLazy_Circle* c, int x, int y, Uint32 col)
{
  double		a = _get_angle(x, y, c->r_tmp);

  if (c->full_circle ||
      (c->deg[0] < a && a < c->deg[1]) ||
      (c->deg[0] > c->deg[1] && (c->deg[0] < a || a < c->deg[1])))
    SDLazy_SetPix_Secure(c->srf, c->x + x, c->y + y, col);
}

static void		_circle_8px(SDLazy_Circle* c, int x, int y, Uint32 col)
{
  _circle_1px(c,  x,  y, col);
  _circle_1px(c,  y,  x, col);
  _circle_1px(c, -x,  y, col);
  _circle_1px(c, -y,  x, col);
  _circle_1px(c,  x, -y, col);
  _circle_1px(c,  y, -x, col);
  _circle_1px(c, -x, -y, col);
  _circle_1px(c, -y, -x, col);
}

static void		_andres(SDLazy_Circle* c, double col[])
{
  int			x = 0;
  int			y = c->r_tmp;
  int			a = y - 1;
  Uint32		_col = SDL_MapRGB(c->srf->format, col[0], col[1], col[2]);

  while (y >= x)
    {
      _circle_8px(c, x, y, _col);
      if (a >= 2 * x)
        {
          a -= 2 * x;
          ++x;
        }
      else if (a <= 2 * (c->r_tmp - y))
        {
          a += 2 * y;
          --y;
        }
      else
        {
          a += 2 * (y - x);
          --y;
          ++x;
        }
    }
}

void			SDLazy_DrawCircle(SDLazy_Circle* c)
{
  double		r_int;
  double		col_inc[3];
  double		col_tmp[3];

  if (c->border_size > 0)
    {
      r_int = c->r;
      c->r_tmp = c->r + c->border_size;
    }
  else
    {
      c->r_tmp = c->r;
      r_int = c->r + c->border_size;
    }

  c->deg[0] = fmod(c->deg[0], 2 * M_PI);
  c->deg[1] = fmod(c->deg[1], 2 * M_PI);

  col_inc[0] = (c->col_ext.r - c->col_int.r) / (r_int - c->r_tmp);
  col_inc[1] = (c->col_ext.g - c->col_int.g) / (r_int - c->r_tmp);
  col_inc[2] = (c->col_ext.b - c->col_int.b) / (r_int - c->r_tmp);
  col_tmp[0] = c->col_ext.r;
  col_tmp[1] = c->col_ext.g;
  col_tmp[2] = c->col_ext.b;

  while (c->r_tmp >= r_int)
    {
      col_tmp[0] += col_inc[0];
      col_tmp[1] += col_inc[1];
      col_tmp[2] += col_inc[2];
      _andres(c, col_tmp);
      --c->r_tmp;
    }
}
#include		<SDL/SDL.h>

SDL_Rect*		SDLazy_Rect(Sint32 x, Sint32 y, Uint32 w, Uint32 h)
{
  static SDL_Rect	r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  return &r;
}
#include	"SDLazy/SDLazy.h"
#include	"Math/myMath.h"

static double		cosA, sinA;

static SDL_Surface*	_createSrf(SDL_Rect* r, SDL_PixelFormat* px, double rad, v2f* scl)
{
  v2f			v1, v2;
  m4f			mat;

  cosA = mycos(rad);
  sinA = mysin(rad);
  mat.a =  cosA * scl->x;
  mat.b = -sinA * scl->y;
  mat.c =  sinA * scl->x;
  mat.d =  cosA * scl->y;
  v1.x = ceil(r->w / 2.);
  v1.y = ceil(r->h / 2.);
  v2.x = -v1.x;
  v2.y =  v1.y;
  v2f_mul_m4f(&v1, &mat);
  v2f_mul_m4f(&v2, &mat);
  return SDL_CreateRGBSurface(SDL_SWSURFACE,
			      ceil(2 * SDL_max(fabs(v1.x), fabs(v2.x))),
			      ceil(2 * SDL_max(fabs(v1.y), fabs(v2.y))),
			      px->BitsPerPixel, px->Rmask, px->Gmask, px->Bmask, px->Amask);
}

static void		_rotScale(SDL_Surface* src, SDL_Rect* src_r, v2f* scl, SDL_Surface* dst)
{
  m4f			mat;
  v2f			v, v2;
  int			x, y;

  mat.a =  cosA / scl->x;
  mat.b =  sinA / scl->x;
  mat.c = -sinA / scl->y;
  mat.d =  cosA / scl->y;
  for (y = 0, v.y = -dst->h / 2; y < dst->h; ++y, ++v.y)
    for (x = 0, v.x = -dst->w / 2; x < dst->w; ++x, ++v.x)
      {
	v2 = v;
	v2f_mul_m4f(&v2, &mat);
	v2.x += src_r->w / 2;
	v2.y += src_r->h / 2;
	SDLazy_SetPix(dst, x, y,
		      (v2.x < 0 || v2.x >= src_r->w ||
		       v2.y < 0 || v2.y >= src_r->h) ? 0 :
		      SDLazy_GetPix(src, src_r->x + v2.x, src_r->y + v2.y));
      }
}

SDL_Surface*		SDLazy_RotScale(SDL_Surface* src, SDL_Rect* src_r, double rad, v2f* scale)
{
  SDL_Surface*		dst = _createSrf(src_r, src->format, rad, scale);

  if (dst)
    _rotScale(src, src_r, scale, dst);
  else
    SDLazy_Quit("SDLazy_RotScale > SDL_CreateRGBSurface failed");
  return dst;
}
#include		"SDLazy/SDLazy.h"
#include		"SDLazy/SDLazy_ObjHeader.h"
#include		"SDLazy/SDLazy_Align.h"

static void		_align(v2f* v, v2f* pos, Uint8 align, SDL_Rect* rc)
{
  *v = *pos;
  if (!(align & ALIGN_LEFT))
    v->x += align & ALIGN_RIGHT  ? rc->w : rc->w / 2.;
  if (!(align & ALIGN_TOP))
    v->y += align & ALIGN_BOTTOM ? rc->h : rc->h / 2.;
}

static void		_center(SDLazy_ObjHeader* h, v2f* v)
{
  v2f			vCenter;
  double		cosA = cos(h->rad_rel);
  double		sinA = sin(h->rad_rel);
  m4f			mat;

  _align(&vCenter, &h->cnt, h->cnt_align, &h->tsp_curr->r);
  vCenter.x -= h->tsp_curr->r.w / 2.;
  vCenter.y -= h->tsp_curr->r.h / 2.;
  vCenter.x *= h->scl_rel.x;
  vCenter.y *= h->scl_rel.y;
  mat.a = cosA;
  mat.b = -sinA;
  mat.c = sinA;
  mat.d = cosA;
  v2f_mul_m4f(&vCenter, &mat);
  v->x = vCenter.x + h->tsp_curr->srf->w / 2.;
  v->y = vCenter.y + h->tsp_curr->srf->h / 2.;
}

static void		SDLazy_CalcPositionAbs(SDLazy_ObjHeader* h)
{
  v2f			v_cnt;
  v2f			v_pos;

  _center(h, &v_cnt);
  _align(&v_pos, &h->pos_rel, h->pos_align, SDLazy_Rect(0, 0, SDLazy_GetWidth(0), SDLazy_GetHeight(0)));
  h->pos_abs.x = v_pos.x - v_cnt.x;
  h->pos_abs.y = v_pos.y - v_cnt.y;
}

static void		SDLazy_RadScaleAbs(SDLazy_ObjHeader* h, double* rad_tmp, v2f* scl_tmp)
{
  /* pour le moment pas de parent */
  *rad_tmp = h->rad_rel;
  *scl_tmp = h->scl_rel;
}

void			SDLazy_Blit(void* o)
{
#define                 SCALE_MIN       0.01
#define                 RAD_MIN         0.001
  SDLazy_ObjHeader*	h = o;
  double		rad_tmp;
  v2f			scl_tmp;

  if (SDLazy_GetType(o) == ANIM && ((SDLazy_Anim*)o)->status == ANIM_STOP)
    return;
  SDLazy_RadScaleAbs(h, &rad_tmp, &scl_tmp);
  if (!h->tsp_curr->srf ||
      fabs(h->tsp_curr->rad_abs - rad_tmp) > RAD_MIN ||
      fabs(h->tsp_curr->scl_abs.x - scl_tmp.x) > SCALE_MIN ||
      fabs(h->tsp_curr->scl_abs.y - scl_tmp.y) > SCALE_MIN)
    {
      h->tsp_curr->rad_abs = rad_tmp;
      h->tsp_curr->scl_abs = scl_tmp;
      SDL_FreeSurface(h->tsp_curr->srf);
      h->tsp_curr->srf =
	SDLazy_RotScale(h->srf_src, &h->tsp_curr->r, h->tsp_curr->rad_abs, &h->tsp_curr->scl_abs);
    }
  SDLazy_CalcPositionAbs(h);
  if (SDL_BlitSurface(h->tsp_curr->srf, NULL, h->srf_dst,
                      h->srf_dst == SDLazy_GetScreen() ? &h->pos_abs : NULL) == -1)
    SDLazy_Quit("SDLazy_SpriteBlitOnScreen > SDL_BlitSurface failed");
}
#include		"SDLazy/SDLazy.h"

/*
void			SDLazy_BlitOnScreen(void* obj)
{
  SDLazy_GetSprite(obj)->dst = SDLazy_GetScreen();
}

void			SDLazy_BlitOnParent(void* obj)
{
  SDLazy_Sprite*	sp = SDLazy_GetSprite(obj);

  sp->dst = sp->parent ? sp->parent->srf->src : SDLazy_GetScreen();
}
*/
#include	"SDLazy/SDLazy_TinySprite.h"

void		SDLazy_TinySpriteInit(SDLazy_TinySprite* tSp, SDL_Rect* r)
{
  tSp->r = *r;
  tSp->srf = NULL;
  tSp->rad_abs = 0;
  tSp->scl_abs.x = 1.;
  tSp->scl_abs.y = 1.;
}
#include	"SDLazy/SDLazy.h"
#include	"SDLazy/SDLazy_ObjHeader.h"
#include	"SDLazy/SDLazy_Align.h"
#include	<string.h>

#define		OBJ(X)		((SDLazy_ObjHeader*)X)

void		SDLazy_InitObjHeader(SDLazy_ObjHeader* o, eObjType type, SDL_Surface* src)
{
  memset(o, 0, sizeof *o);
  o->obj_type = type;
  o->srf_src = src;
  o->srf_dst = SDLazy_GetScreen();
  o->cnt_align = ALIGN_TOP | ALIGN_LEFT;
  o->pos_align = ALIGN_TOP | ALIGN_LEFT;
  o->scl_rel.x = 1.0;
  o->scl_rel.y = 1.0;
}

int		SDLazy_Hover(void* o)
{
  return OBJ(o)->tsp_curr->srf &&
    g_sdlazy.mouse.x >= OBJ(o)->pos_abs.x &&
    g_sdlazy.mouse.x <  OBJ(o)->pos_abs.x + OBJ(o)->tsp_curr->srf->w &&
    g_sdlazy.mouse.y >= OBJ(o)->pos_abs.y &&
    g_sdlazy.mouse.y <  OBJ(o)->pos_abs.y + OBJ(o)->tsp_curr->srf->h;
}

eObjType	SDLazy_GetType(void* o) { return OBJ(o)->obj_type; }

/*
void		SDLazy_BlitOnScreen(void* o){OBJ(o)->srf_dst = SDLazy_GetScreen();}
void		SDLazy_BlitOnParent(void* o){OBJ(o)->srf_dst = OBJ(o)->parent;}
*/

double		SDLazy_GetWidth(void* o)
{
  return o ? OBJ(o)->tsp_curr->r.w * OBJ(o)->scl_rel.x : SDLazy_GetScreen()->w;
}

double		SDLazy_GetHeight(void* o)
{
  return o ? OBJ(o)->tsp_curr->r.h * OBJ(o)->scl_rel.y : SDLazy_GetScreen()->h;
}

/*   Setters ( = ) */
void		SDLazy_SetPosAlign	(void* o, Uint8  a) { OBJ(o)->pos_align =  a; }
void		SDLazy_SetPos		(void* o, v2f*   v) { OBJ(o)->pos_rel   = *v; }
void		SDLazy_SetPosX		(void* o, double x) { OBJ(o)->pos_rel.x =  x; }
void		SDLazy_SetPosY		(void* o, double y) { OBJ(o)->pos_rel.y =  y; }
void		SDLazy_SetScale		(void* o, v2f*   v) { OBJ(o)->scl_rel   = *v; }
void		SDLazy_SetScaleX	(void* o, double x) { OBJ(o)->scl_rel.x =  x; }
void		SDLazy_SetScaleY	(void* o, double y) { OBJ(o)->scl_rel.y =  y; }
void		SDLazy_SetRot		(void* o, double r) { OBJ(o)->rad_rel   =  r; }
void		SDLazy_SetCenterAlign	(void* o, Uint8  a) { OBJ(o)->cnt_align =  a; }
void		SDLazy_SetCenter	(void* o, v2f*   v) { OBJ(o)->cnt       = *v; }
void		SDLazy_SetCenterX	(void* o, double x) { OBJ(o)->cnt.x     =  x; }
void		SDLazy_SetCenterY	(void* o, double y) { OBJ(o)->cnt.y     =  y; }

/*   Increment ( += ) */
void		SDLazy_IncPos		(void* o, v2f*   v) { OBJ(o)->pos_rel.x += v->x; OBJ(o)->pos_rel.y += v->y; }
void		SDLazy_IncPosX		(void* o, double x) { OBJ(o)->pos_rel.x += x; }
void		SDLazy_IncPosY		(void* o, double y) { OBJ(o)->pos_rel.y += y; }
void		SDLazy_IncScale		(void* o, v2f*   v) { OBJ(o)->scl_rel.x += v->x; OBJ(o)->scl_rel.y += v->y; }
void		SDLazy_IncScaleX	(void* o, double x) { OBJ(o)->scl_rel.x += x; }
void		SDLazy_IncScaleY	(void* o, double y) { OBJ(o)->scl_rel.y += y; }
void		SDLazy_IncRot		(void* o, double r) { OBJ(o)->rad_rel += r; }
void		SDLazy_IncCenter	(void* o, v2f*   v) { OBJ(o)->cnt.x += v->x; OBJ(o)->cnt.y += v->y; }
void		SDLazy_IncCenterX	(void* o, double x) { OBJ(o)->cnt.x += x; }
void		SDLazy_IncCenterY	(void* o, double y) { OBJ(o)->cnt.y += y; }

/*   Getters */
Uint8		SDLazy_GetPosAlign	(void* o) { return  OBJ(o)->pos_align; }
v2f*		SDLazy_GetPos		(void* o) { return &OBJ(o)->pos_rel;   }
double		SDLazy_GetPosX		(void* o) { return  OBJ(o)->pos_rel.x; }
double		SDLazy_GetPosY		(void* o) { return  OBJ(o)->pos_rel.y; }
v2f*		SDLazy_GetScale		(void* o) { return &OBJ(o)->scl_rel;   }
double		SDLazy_GetScaleX	(void* o) { return  OBJ(o)->scl_rel.x; }
double		SDLazy_GetScaleY	(void* o) { return  OBJ(o)->scl_rel.y; }
double		SDLazy_GetRot		(void* o) { return  OBJ(o)->rad_rel;   }
Uint8		SDLazy_GetCenterAlign	(void* o) { return  OBJ(o)->cnt_align; }
v2f*		SDLazy_GetCenter	(void* o) { return &OBJ(o)->cnt;       }
double		SDLazy_GetCenterX	(void* o) { return  OBJ(o)->cnt.x;     }
double		SDLazy_GetCenterY	(void* o) { return  OBJ(o)->cnt.y;     }

#undef		OBJ
#include		"SDLazy/SDLazy.h"
#include		"SDLazy/SDLazy_CursorsAscii.h"

static SDL_Cursor*	_create(const char* img[], int x, int y)
{
  int			i, row, col;
  Uint8			data[4 * 32];
  Uint8			mask[4 * 32];

  i = -1;
  for (row = 0; row < 32; ++row)
    for (col = 0; col < 32; ++col)
      {
	if (col % 8)
	  {
	    data[i] <<= 1;
	    mask[i] <<= 1;
	  }
	else
	  {
	    ++i;
	    data[i] = mask[i] = 0;
	  }
	switch (img[row][col])
	  {
	  case '.':
	    data[i] |= 0x01;
	    mask[i] |= 0x01;
	    break;
	  case 'X':
	    mask[i] |= 0x01;
	    break;
	  case ' ':
	    break;
	  }
      }
  return SDL_CreateCursor(data, mask, 32, 32, x, y);
}

void			SDLazy_InitCursors(SDLazy_Cursors* curs)
{
  curs->tab[CURSOR_ARROW] = _create(cur_arrow, 0, 0);
  curs->tab[CURSOR_POINTER] = _create(cur_pointer, 5, 0);
  curs->tab[CURSOR_OPEN_HAND] = _create(cur_open_hand, 7, 5);
  curs->tab[CURSOR_CLOSE_HAND] = _create(cur_close_hand, 7, 5);
  curs->tab[CURSOR_CROSS] = _create(cur_cross, 9, 9);
  g_sdlazy.cursors.cursor_curr = CURSOR_ARROW;
  SDLazy_SetCursor(CURSOR_ARROW);
}
#include	"SDLazy/SDLazy.h"

void		SDLazy_FreeCursors(SDLazy_Cursors* curs)
{
  unsigned	u;

  for (u = 0; u < CURSOR_TOTAL; ++u)
    SDL_FreeCursor(curs->tab[u]);
}
#include	"SDLazy/SDLazy.h"

void		SDLazy_SetCursor(eCursor cur)
{
  g_sdlazy.cursors.cursor_before = g_sdlazy.cursors.cursor_curr;
  g_sdlazy.cursors.cursor_curr = cur;
  SDL_SetCursor(g_sdlazy.cursors.tab[cur]);
}
#include		"SDLazy/SDLazy.h"

void			SDLazy_SwapCursor(void)
{
  eCursor		tmp = g_sdlazy.cursors.cursor_before;

  g_sdlazy.cursors.cursor_before = g_sdlazy.cursors.cursor_curr;
  g_sdlazy.cursors.cursor_curr = tmp;
  SDL_SetCursor(g_sdlazy.cursors.tab[tmp]);
}
#include		"SDLazy/SDLazy.h"
#include		<stdlib.h>

static int		_need_realloc(Uint8 type)
{
  void*			tmp;

  if (!(tmp = realloc(g_sdlazy.e[type].f, (g_sdlazy.e[type].nb_f + 1) * sizeof(*g_sdlazy.e[type].f))))
    return -1;
  g_sdlazy.e[type].f = tmp;
  ++g_sdlazy.e[type].sz_f;
  return 0;
}

void				SDLazy_AddEvent(Uint8 type, void (*f)(SDL_Event*))
{
  if (g_sdlazy.e[type].nb_f >= g_sdlazy.e[type].sz_f &&
      _need_realloc(type))
    SDLazy_Quit("SDLazy_AddEvent: realloc failed");
  g_sdlazy.e[type].f[g_sdlazy.e[type].nb_f] = f;
  ++g_sdlazy.e[type].nb_f;
}
#include	"SDLazy/SDLazy.h"

void		SDLazy_DelEvent(Uint8 type, void (*f)(SDL_Event*))
{
  int		i, j;

  for (i = 0; i < g_sdlazy.e[type].nb_f; ++i)
    if (g_sdlazy.e[type].f[i] == f)
      {
	for (j = i + 1; j < g_sdlazy.e[type].nb_f; ++j)
	  g_sdlazy.e[type].f[j - 1] = g_sdlazy.e[type].f[j];
	--g_sdlazy.e[type].nb_f;
      }
}
#include	"SDLazy/SDLazy.h"

void		SDLazy_EventsUpdate(void)
{
  SDL_Event	e;
  int		i;

  while (SDL_PollEvent(&e))
    for (i = 0; i < g_sdlazy.e[e.type].nb_f; ++i)
      g_sdlazy.e[e.type].f[i](&e);
}
#include	"SDLazy/SDLazy.h"

void		SDLazy_ResizeEvent(SDL_Event* e)
{
  SDL_FreeSurface(g_sdlazy.scr);
  if (!(g_sdlazy.scr = SDL_SetVideoMode(e->resize.w, e->resize.h, 32, g_sdlazy.flags)))
    SDLazy_Quit("SDL_SetVideoMode failed");
}

void		SDLazy_MouseMoveEvent(SDL_Event* e)
{
  g_sdlazy.mouse.x = e->motion.x;
  g_sdlazy.mouse.y = e->motion.y;
  SDLazy_ButtonMouseMove();
}

void		SDLazy_ClicDownEvent(SDL_Event* e)
{
  if (e->button.button == SDL_BUTTON_LEFT)
    SDLazy_ButtonClicDown();
}

void		SDLazy_ClicUpEvent(SDL_Event* e)
{
  if (e->button.button == SDL_BUTTON_LEFT)
    SDLazy_ButtonClicUp();
}
#include	"SDLazy/SDLazy_Surfaces.h"

void		SDLazy_InitSurfaces(SDLazy_Surfaces* s)
{
  CList_init(&s->list);
}
#include	"SDLazy/SDLazy_Surfaces.h"
#include	<stdlib.h>

void		SDLazy_FreeSurface(SDLazy_Surface* srf)
{
  if (srf->to_free)
    SDL_FreeSurface(srf->header.srf_src);
  free(srf);
}

void		SDLazy_FreeSurfaces(SDLazy_Surfaces* srf)
{
  CList_clear(&srf->list);
}
#include		"SDLazy/SDLazy.h"
#include		<stdlib.h>
#include		<string.h>

static SDL_Surface*	_already_loaded(CList* li, char const* path, SDL_Surface* src)
{
  SDLazy_Surface*	srf;
  CLink*		ln;

  if (path)
    {
      for (ln = li->begin; ln && (srf = ln->data); ln = ln->next)
	if (srf->path && !strcmp(path, srf->path))
	  return srf->header.srf_src;
    }
  else
    for (ln = li->begin; ln && (srf = ln->data); ln = ln->next)
      if (src == srf->header.srf_src)
	return srf->header.srf_src;
  return NULL;
}

static int		_init(SDLazy_Surface* srf, char const* path, SDL_Surface* src, SDL_Rect* src_r)
{
  srf->path = path;
  srf->to_free = 0;
  SDLazy_InitObjHeader(&srf->header, SURFACE, _already_loaded(&g_sdlazy.surfaces.list, path, src));
  srf->header.tsp_curr = &srf->tinySp;
  if (!srf->header.srf_src)
    {
      if (src)
        srf->header.srf_src = src;
      else if (!(srf->header.srf_src = IMG_Load(path)))
	{
	  SDLazy_Quit("SDLazy_SurfaceLoad > IMG_Load failed");
	  return -1;
	}
      srf->to_free = 1;
    }
  SDLazy_TinySpriteInit(&srf->tinySp, src_r ? src_r : SDLazy_Rect(0, 0, srf->header.srf_src->w, srf->header.srf_src->h));
  return 0;
}

static SDLazy_Surface*	_load(char const* path, SDL_Surface* src, SDL_Rect* src_r)
{
  SDLazy_Surface*	srf = malloc(sizeof *srf);

  if (!srf)
    SDLazy_Quit("SDLazy_SurfaceLoadPack: malloc failed");
  _init(srf, path, src, src_r);
  if (!CList_push_back(&g_sdlazy.surfaces.list, srf, 0, SDLazy_FreeSurface))
    SDLazy_Quit("SDLazy_SurfaceLoadPack: CList_push_back failed");
  return srf;
}

SDLazy_Surface*		SDLazy_SurfacePack(SDL_Surface* src, SDL_Rect* src_r)
{
  return _load(NULL, src, src_r);
}

SDLazy_Surface*		SDLazy_SurfaceLoad(char const* path, SDL_Rect* src_r)
{
  return _load(path, NULL, src_r);
}
#include		"SDLazy/SDLazy.h"

SDLazy_Sprite*		SDLazy_SpriteCreate(SDLazy_Surface* srf)
{
  SDLazy_Sprite*	sp = malloc(sizeof *sp);

  if (!sp)
    SDLazy_Quit("SDLazy_SpriteCreate > malloc failed");
  else if (!CList_push_back(&g_sdlazy.sprites.list, sp, 0, NULL))
    SDLazy_Quit("SDLazy_SpriteCreate > CList_push_back failed");
  sp->header = srf->header;
  sp->header.obj_type = SPRITE;
  SDLazy_TinySpriteInit(&sp->tinySp, &srf->header.tsp_curr->r);
  sp->header.tsp_curr = &sp->tinySp;
  return sp;
}
#include	"SDLazy/SDLazy_Sprites.h"

void		SDLazy_FreeSprites(SDLazy_Sprites* s)
{
  CList_clear(&s->list);
}
#include	"SDLazy/SDLazy.h"
#include	<stdlib.h>

void		SDLazy_AnimDestroy(SDLazy_Anim* anim)
{
  CList_pop(CList_pfind_back(&g_sdlazy.anims.list, anim));
}

void		SDLazy_InitAnims(SDLazy_Anims* anims)
{
  SDLazy_AnimsTimer(TIMER_DEFAULT);
  CList_init(&anims->list);
}

void		SDLazy_FreeAnims(SDLazy_Anims* anims)
{
  CList_clear(&anims->list);
}
#include		"SDLazy/SDLazy.h"
#include		<stdlib.h>

static int		_createTinySp(SDLazy_Anim* an, SDL_Rect* src_r)
{
  SDL_Rect		r = *src_r;
  Uint16		u;

  if (!(an->tinySp_tab = malloc(an->nb_frames * sizeof *an->tinySp_tab)))
    return -1;
  for (u = 0; u < an->nb_frames; ++u, r.x += r.w)
    SDLazy_TinySpriteInit(an->tinySp_tab + u, &r);
  return 0;
}

static void		SDLazy_AnimDestructor(SDLazy_Anim* anim)
{
  free(anim->tinySp_tab);
  free(anim);
}

SDLazy_Anim*		SDLazy_AnimCreate(SDLazy_Surface* srf, Uint16 nb_frames, Uint16 frame_loop, eAnimStatus status)
{
  SDLazy_Anim*		an = calloc(1, sizeof *an);

  if (!an)
    SDLazy_Quit("SDLazy_AnimCreate malloc failed");
  if (!CList_push_back(&g_sdlazy.anims.list, an, 0, SDLazy_AnimDestructor))
    SDLazy_Quit("SDLazy_AnimCreate CList_push_back failed");
  SDLazy_InitObjHeader(&an->header, ANIM, srf->header.srf_src);
  an->nb_frames = nb_frames;
  an->status = status;
  an->pause_or_stop = status == ANIM_STOP ? ANIM_STOP : ANIM_PAUSE;
  an->frame_loop = SDL_min(frame_loop, nb_frames);
  an->ms_interval = g_sdlazy.anims.timer_default;
  if (_createTinySp(an, &srf->tinySp.r))
    SDLazy_Quit("SDLazy_AnimCreate malloc failed");
  an->header.tsp_curr = an->tinySp_tab;
  return an;
}
#include	"SDLazy/SDLazy.h"

void		SDLazy_AnimReplay(SDLazy_Anim* anim)
{
  anim->status = ANIM_PLAYING;
  anim->frame_curr = 0;
  anim->header.tsp_curr = anim->tinySp_tab;
}

int		SDLazy_AnimPlay(SDLazy_Anim* anim)
{
  anim->status = ANIM_PLAYING;
  return CLIST_CONTINUE;
}

int		SDLazy_AnimPause(SDLazy_Anim* anim)
{
  if (anim->status == ANIM_PLAYING)
    anim->status = ANIM_PAUSE;
  return CLIST_CONTINUE;
}

int		SDLazy_AnimStop(SDLazy_Anim* anim)
{
  anim->status = anim->pause_or_stop;
  anim->frame_curr = 0;
  anim->header.tsp_curr = anim->tinySp_tab;
  return CLIST_CONTINUE;
}

void		SDLazy_AnimsPlay(void)  { CList_foreach(&g_sdlazy.anims.list, SDLazy_AnimPlay);  }
void		SDLazy_AnimsPause(void) { CList_foreach(&g_sdlazy.anims.list, SDLazy_AnimPause); }
void		SDLazy_AnimsStop(void)  { CList_foreach(&g_sdlazy.anims.list, SDLazy_AnimStop);  }
#include	"SDLazy/SDLazy.h"

/* Getters */
eAnimStatus	SDLazy_AnimStatus(SDLazy_Anim* a) { return a->status; }
Uint16		SDLazy_AnimNbFrames(SDLazy_Anim* a) { return a->nb_frames; }

/* Setters */
void		SDLazy_AnimTimer(SDLazy_Anim* a, Uint16 ms) { a->ms_interval = ms; }
void		SDLazy_AnimFrameLoop(SDLazy_Anim* a, Uint16 frm) { a->frame_loop = SDL_min(a->nb_frames, frm); }

static int	_change_timer(SDLazy_Anim* anim)
{
  anim->ms_interval = g_sdlazy.anims.timer_default;
  return CLIST_CONTINUE;
}

void		SDLazy_AnimsTimer(Uint16 ms)
{
  g_sdlazy.anims.timer_default = ms;
  CList_foreach(&g_sdlazy.anims.list, _change_timer);
}
#include	"SDLazy/SDLazy_Anims.h"

static int	update(SDLazy_Anim* anim)
{
  if (anim->status == ANIM_PLAYING &&
      SDL_GetTicks() - anim->ms_curr >= anim->ms_interval)
    {
      anim->ms_curr = SDL_GetTicks();
      if (++anim->frame_curr < anim->nb_frames)
	anim->header.tsp_curr = anim->tinySp_tab + anim->frame_curr;
      else if (anim->frame_loop == anim->nb_frames)
	SDLazy_AnimStop(anim);
      else
	{
	  anim->frame_curr = anim->frame_loop;
	  anim->header.tsp_curr = anim->tinySp_tab + anim->frame_curr;
	}
    }
  return CLIST_CONTINUE;
}

void		SDLazy_AnimsUpdate(SDLazy_Anims* anims)
{
  CList_foreach(&anims->list, update);
}
#include		"SDLazy/SDLazy.h"
#include		"SDLazy/SDLazy_ObjHeader.h"
#include		<stdlib.h>

void			SDLazy_InitButtons(SDLazy_Buttons* buttons)
{
  CList_init(&buttons->list);
}

void			SDLazy_FreeButtons(SDLazy_Buttons* buttons)
{
  CList_clear(&buttons->list);
}

void			SDLazy_ButtonDestroy(SDLazy_Button* b)
{
  CList_pop(CList_pfind_back(&g_sdlazy.buttons.list, b));
}

static int		_createTinySprites(SDLazy_Button* b, SDLazy_Surface* srf)
{
  SDLazy_TinySprite**	tinySp_tab = malloc(b->nb_states * sizeof *tinySp_tab);
  SDLazy_TinySprite*	tinySp_buf;
  SDL_Rect		r = srf->tinySp.r;
  Uint16		u, v, ry = r.y;

  if (!tinySp_tab || !(tinySp_buf = malloc(b->nb_states * 3 * sizeof *tinySp_buf)))
    return -1;
  for (u = 0; u < b->nb_states; ++u)
    {
      tinySp_tab[u] = tinySp_buf + u * 3;
      for (v = 0, r.y = ry; v < 3; ++v, r.y += r.h)
	SDLazy_TinySpriteInit(tinySp_tab[u] + v, &r);
      r.x += r.w;
    }
  b->tinySp_tab = tinySp_tab;
  b->header.tsp_curr = tinySp_tab[0] + 0;
  return 0;
}

static void		SDLazy_ButtonDestructor(SDLazy_Button* b)
{
  if (b->tinySp_tab)
    {
      free(b->tinySp_tab[0]);
      free(b->tinySp_tab);
    }
  free(b);
}

SDLazy_Button*		SDLazy_ButtonCreate(SDLazy_Surface* srf, Uint16 nb_states)
{
  SDLazy_Button*	b = calloc(1, sizeof *b);

  if (!b)
    SDLazy_Quit("SDLazy_ButtonCreate > calloc failed");
  if (!CList_push_back(&g_sdlazy.buttons.list, b, 0, SDLazy_ButtonDestructor))
    SDLazy_Quit("SDLazy_ButtonCreate > CList_push_back failed");
  SDLazy_InitObjHeader(&b->header, BUTTON, srf->header.srf_src);
  b->nb_states = nb_states;
  if (_createTinySprites(b, srf))
    SDLazy_Quit("SDLazy_ButtonCreate > calloc failed");
  b->enable = 1;
  return b;
}
#include	"SDLazy/SDLazy.h"

static void	_setTinySp_curr(SDLazy_Button* b)
{
  b->header.tsp_curr = b->tinySp_tab[b->curr_state] + b->hover + b->pressed;
}

static int	_clicDown(SDLazy_Button* b)
{
  if (b->enable && b->hover)
    {
      b->pressed = 1;
      if (b->f[BUTTON_CLICDOWN])
	b->f[BUTTON_CLICDOWN](b);
      _setTinySp_curr(b);
    }
  return CLIST_CONTINUE;
}

static int	_clicUp(SDLazy_Button* b)
{
  if (b->enable && b->hover)
    {
      b->pressed = 0;
      if (b->f[BUTTON_CLICUP])
	b->f[BUTTON_CLICUP](b);
      if (++b->curr_state >= b->nb_states)
	b->curr_state = 0;
      _setTinySp_curr(b);
    }
  return CLIST_CONTINUE;
}

void		SDLazy_ButtonClicDown(void)
{
  CList_foreach(&g_sdlazy.buttons.list, _clicDown);
}

void		SDLazy_ButtonClicUp(void)
{
  CList_foreach(&g_sdlazy.buttons.list, _clicUp);
}

void		SDLazy_ButtonMouseMove(void)
{
  CLink*		ln = CList_begin(&g_sdlazy.buttons.list);
  SDLazy_Button*	b;

  for (; ln; ln = ln->next)
    {
      b = ln->data;
      if (!b->enable)
	continue;
      if (SDLazy_Hover(b))
	{
	  if (!b->hover)
	    {
	      b->hover = 1;
	      SDLazy_SetCursor(CURSOR_POINTER);
	      if (b->f[BUTTON_MOUSEOVER])
		b->f[BUTTON_MOUSEOVER](b);
		_setTinySp_curr(b);
	    }
	}
      else if (b->hover)
	{
	  b->hover = 0;
	  SDLazy_SwapCursor();
	  if (b->f[BUTTON_MOUSEOUT])
	    b->f[BUTTON_MOUSEOUT](b);
	  b->pressed = 0;
	  _setTinySp_curr(b);
	}
    }
}
#include	"SDLazy/SDLazy.h"

void		SDLazy_ButtonEnable(SDLazy_Button* b) { b->enable = 1; }
void		SDLazy_ButtonDisable(SDLazy_Button* b) { b->enable = 0; }

void		SDLazy_ButtonSetFun(SDLazy_Button* b, eButtonEvent e, void (*f)()) { b->f[e] = f; }

Uint16		SDLazy_ButtonGetState(SDLazy_Button* b) { return b->curr_state; }
void		SDLazy_ButtonSetState(SDLazy_Button* b, Uint16 st)
{
  if (st < b->nb_states)
    {
      b->curr_state = st;
      b->header.tsp_curr = b->tinySp_tab[st] + b->hover + b->pressed;
    }
}
