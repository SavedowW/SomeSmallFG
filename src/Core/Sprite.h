#ifndef SPRITE_H_
#define SPRITE_H_
#include <SDL.h>
#include <memory>
#include <cassert>
#include "Vector2.h"

class Sprite
{
public:
	virtual SDL_Texture* getSprite() = 0;
	virtual void update();
	virtual Vector2<float> getSize() = 0;

	virtual ~Sprite();
};

#endif