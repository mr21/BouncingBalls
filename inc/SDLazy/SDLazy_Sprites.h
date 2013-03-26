#ifndef			SDLAZY_SPRITES_H
# define		SDLAZY_SPRITES_H

# include		"SDLazy_ObjHeader.h"
# include		"SDLazy_TinySprite.h"
# include		"SDLazy_Surfaces.h"
# include		"CList.h"

typedef			struct
{
  SDLazy_ObjHeader	header;
  SDLazy_TinySprite	tinySp;
}			SDLazy_Sprite;

typedef			struct
{
  CList			list;
}			SDLazy_Sprites;

void			SDLazy_FreeSprites(SDLazy_Sprites*);

SDLazy_Sprite*		SDLazy_SpriteCreate(SDLazy_Surface*);

#endif
