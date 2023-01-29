#define SDL_MAIN_HANDLED
#include "base.h"
#include "ray.h"
#include "render.h"

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;

vector<edge> scenelines;
edge cameraline;

SDL_Event e;

vector<int> usedh;

bool rayshowmode = true;

int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_CreateWindowAndRenderer(1920,1080,0,&window,&renderer);
	vector<ray> rays;

	for(double i = 270; i < 270+180; i += 0.01) {
		rays.push_back(ray({1920 / 2,2},i));
	}
	scenelines.push_back({{0,0},{1920,0},{255,0,0,255}});
	scenelines.push_back({{1920,1080},{0,1080},{0,255,0,255}});
	scenelines.push_back({{1920,0},{1920,1090},{0,0,255,255}});
	cameraline={{0,0},{0,1080},{0,0,0,0}};
	int finishedRays=0;

	SDL_SetRenderDrawColor(renderer,255,255,255,255);
	SDL_RenderClear(renderer);
	
	while(e.key.keysym.scancode != SDL_SCANCODE_Q) {
		SDL_PollEvent(&e);
		if(e.key.keysym.scancode == SDL_SCANCODE_E && e.type == SDL_KEYUP) {
			SDL_SetRenderDrawColor(renderer,255,255,255,255);
			SDL_RenderClear(renderer);
			rayshowmode = !rayshowmode;
			cout << "changed mode to " << rayshowmode << endl;
		}
		for(int i = 0; i < rays.size(); i++) {
			rays[i].update();
			SDL_Color c = (rays[i].projectedColor);
			if(rayshowmode) {
				SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
				SDL_RenderDrawPoint(renderer,rays[i].position.x,rays[i].position.y);
			}
			if(rays[i].finishedpath && !rays[i].checked) {
				rays[i].checked = true;
				finishedRays++;
				if(!rayshowmode) {
					renderchunk(rays[i]);
				}
				cout << "finished rays: " << finishedRays << ", remaining:" << rays.size()-finishedRays << "\r";
			}
		}
		SDL_RenderPresent(renderer);
	}
	cout << "ended rendering " << endl;
	while(1);
}