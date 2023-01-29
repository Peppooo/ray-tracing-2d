#pragma once

#include "base.h"

class ray {
public:
	vec2 position;
	vec2 reflectionpoint;
	double angle;
	//double reflecting=100;
	double speed = 1;
	SDL_Color light_color = {255,255,255,255};
	vector<SDL_Color> colorsreflections;
	bool finishedpath = false;
	double pixelh;
	SDL_Color projectedColor = {light_color};
	bool checked = false;
	ray(vec2 center,double _angle) {
		position = center;
		reflectionpoint = center;
		angle = _angle;
	}
	void update() {
		for(int i = 0; i < scenelines.size(); i++)
		{
			if(scenelines[i].distancefrom(position) < 1)
			{
				if(!finishedpath) {
					light_color = scenelines[i].color;
					colorsreflections.push_back(scenelines[i].color);
					projectedColor = coloravg(colorsreflections);
					double a = position.y - reflectionpoint.y;
					double b = position.x - reflectionpoint.x;
					double c = sqrt(pow(a,2) + pow(position.x - reflectionpoint.x,2));
					angle += degrees(acos((pow(a,2) + pow(b,2) - pow(c,2)) / (2 * a * b)));
					reflectionpoint = position;
					break;
				}
			}
			if(cameraline.distancefrom(position) < 1) { // touch camera
				//cout << "touched camera" << endl;
				finishedpath = true;
				pixelh = position.y;
				projectedColor = light_color;
				break;
			}
		}
		if(!finishedpath) {
			position.x += sin(rads(angle)) * speed;
			position.y += cos(rads(angle)) * speed;
		}
	}
};