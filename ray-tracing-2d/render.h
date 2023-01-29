#pragma once
#include "base.h"
#include "ray.h"

class renderline {

};

void renderchunk(SDL_Renderer* renderer,ray t) {
	SDL_SetRenderDrawColor(renderer,t.projectedColor.r,t.projectedColor.g,t.projectedColor.b,t.projectedColor.a);
}