#pragma once
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

double rads(double degrees) {
	return degrees * (M_PI / 180);
}

double degrees(double radiants) {
	return radiants / (M_PI / 180);
}

class vec2 {
public:
	double x,y;
};

double distancep(vec2 p1,vec2 p2) {
	return sqrt(pow(p2.x - p1.x,2) + pow(p2.y - p1.y,2));
}

bool betweenp(vec2 point,vec2 point1,vec2 point2) {
	if(point1.x > point2.x) {
		std::swap(point1.x,point2.x);
	}
	if(point1.y > point2.y) {
		std::swap(point1.y,point2.y);
	}
	return point1.x <= point.x && point.x <= point2.x && point1.y <= point.y && point.y <= point2.y;
}

class edge {
public:
	vec2 p1;
	vec2 p2;
	SDL_Color color;
	int light_absorb = 100;
	bool isbetween(vec2 point) {
		return betweenp(point,p1,p2);
	}
	double distancefrom(vec2 point) {
		double numerator = std::abs((p2.y - p1.y) * point.x - (p2.x - p1.x) * point.y + p2.x * p1.y - p2.y * p1.x);
		double denominator = distancep(p1,p2);
		return numerator / denominator;
	}
	void draw(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
		SDL_RenderDrawLine(renderer,p1.x,p1.y,p2.x,p2.y);
	}
};

SDL_Color coloravg(vector<SDL_Color> c) {
	if(c.size() != 0) {
		unsigned long long int r,g,b;
		r = g = b = 0;
		for(int i = 0; i < c.size(); i++) {
			r += c[i].r;
			g += c[i].g;
			b += c[i].b;
		}
		return {(Uint8)(r / c.size()),(Uint8)(g / c.size() + 1),(Uint8)(b / c.size()),255};
	}
	else {
		return {255,255,255,255};
	}
}