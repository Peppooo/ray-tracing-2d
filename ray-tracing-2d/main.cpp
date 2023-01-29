#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;

double rads(double degrees) {
	return degrees * (M_PI/180);
}

double degrees(double radiants) {
	return radiants / (M_PI/180);
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
	int reflecting = 70;
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

vector<edge> scenelines;
edge cameraline;

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

class ray {
public:
	vec2 position;
	vec2 reflectionpoint;
	double angle;
	double reflecting=100;
	double speed = 1;
	SDL_Color light_color = {0,0,0,255};
	vector<SDL_Color> colorsreflections;
	bool finishedpath = false;
	double pixelh;
	SDL_Color projectedColor;
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
					double a = position.y-reflectionpoint.y;
					double b = position.x-reflectionpoint.x;
					double c = sqrt(pow(a,2) + pow(position.x-reflectionpoint.x,2));
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

SDL_Event e;

void renderchunk(ray t) {
	SDL_SetRenderDrawColor(renderer,t.projectedColor.r,t.projectedColor.g,t.projectedColor.b,t.projectedColor.a);
	SDL_RenderDrawLine(renderer,0,t.pixelh,1920,t.pixelh);
}

vector<int> usedh;

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(1920,1080,0,&window,&renderer);
	vector<ray> rays;
	for(double i = 0; i < 360; i += 0.01) {
		rays.push_back(ray({1920 / 2,1080 / 2},i));
	}
	scenelines.push_back({{1,1},{1919,1},{255,0,0,255}});
	scenelines.push_back({{1919,1079},{1,1079},{255,0,0,255}});
	scenelines.push_back({{1919,1},{1919,1079},{255,0,0,255}});
	cameraline={{1,1},{1,1079},{255,0,255,255}};
	int finishedRays=0;
	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderClear(renderer);
	//SDL_RenderClear(renderer);
	//cameraline.draw(renderer);
	while(e.key.keysym.scancode != SDL_SCANCODE_Q) {
		SDL_PollEvent(&e);
		for(int i = 0; i < rays.size(); i++) {
			rays[i].update();
			SDL_Color c = coloravg(rays[i].colorsreflections);
			//SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
			//SDL_RenderDrawPoint(renderer,rays[i].position.x,rays[i].position.y);
			if(rays[i].finishedpath && !rays[i].checked) {
				rays[i].checked = true;
				finishedRays++;
				renderchunk(rays[i]);
				cout << "finished rays: " << finishedRays << ", remaining:" << rays.size()-finishedRays << "\r";
			}
		}
		SDL_RenderPresent(renderer);
		//SDL_RenderPresent(renderer);
	}
	cout << "ended rendering " << endl;
	while(1);
}