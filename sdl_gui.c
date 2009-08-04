/*
 * (C) David Olofson, 2003, 2006
 *   modified by the pineapple team, 2009
 */
#include "sdl_gui.h"
#include "gui.h"
#include "pineapple.h"

static int die = 0;
SDL_Surface *screen;
Uint32 fontcolor;

int main(int argc, char *args[]){
	int last_tick;

	if(SDL_Init(SDL_INIT_VIDEO)!=0)
		return 1;

	atexit(SDL_Quit);
	signal(SIGTERM, breakhandler);
	signal(SIGINT, breakhandler);

	screen = SDL_SetVideoMode(640,480,32,SDL_SWSURFACE);
	if(screen==NULL){
		fprintf(stderr,"video init failed");
		SDL_Quit();
		return 1;
	}else{
		fprintf(stdout,"initialized the surface\n");
	}

	fontcolor = SDL_MapRGB(screen->format, 255, 0, 255);
	SDL_WM_SetCaption("pineapple-tracker","pineapple.ico");
	SDL_EnableKeyRepeat(250,25);

	draw_main();
	SDL_Flip(screen);

	last_tick=SDL_GetTicks();
	while(!die){
		SDL_Event ev;
		int tick = SDL_GetTicks();
		int dt = tick - last_tick;
		last_tick = tick;

		/* Handle GUI events */
		while(SDL_PollEvent(&ev)){
			switch(ev.type){
			case SDL_KEYDOWN:
				switch(currmode){
				case PM_NORMAL:
					//handle_key(&ev);
					break;
				case PM_VISUAL:
					break;
				case PM_VISUALLINE:
					break;
				case PM_CMDLINE:
					break;
				case PM_INSERT:
					break;
				case PM_JAMMER:
					break;
				}
				break;
			case SDL_QUIT:
				die = 1;
				break;
			default:
				break;
			}
		}

		//update_main(screen, dt);

		gui_refresh();
		SDL_Delay(10);
	}

	SDL_Quit();
	return 0;
}

void gui_refresh(void){
	SDL_Flip(screen);
}

static void draw_songed(void){
	gui_box(0,20,210,460,fontcolor,screen);
}

static void draw_tracked(void){
	gui_box(215,20,210,460,fontcolor,screen);
}

static void draw_instred(void){
	gui_box(430,20,210,460,fontcolor,screen);
}

static void draw_main(void){
	draw_songed();
	draw_tracked();
	draw_instred();
}

static void update_main(SDL_Surface *screen, int dt){
}

SDL_Surface *gui_load_image(const char *fn){
	SDL_Surface *cvt;
	SDL_Surface *img = SDL_LoadBMP(fn);
	if(!img)
		return NULL;
	cvt = SDL_DisplayFormat(img);
	SDL_FreeSurface(img);
	return cvt;
}

void gui_box(int x, int y, int w, int h, Uint32 c, SDL_Surface *dst){
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = 1;
	SDL_FillRect(dst, &r, c);

	r.x = x;
	r.y = y + h - 1;
	r.w = w;
	r.h = 1;
	SDL_FillRect(dst, &r, c);

	r.x = x;
	r.y = y + 1;
	r.w = 1;
	r.h = h - 2;
	SDL_FillRect(dst, &r, c);

	r.x = x + w - 1;
	r.y = y + 1;
	r.w = 1;
	r.h = h - 2;
	SDL_FillRect(dst, &r, c);

	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	//gui_dirty(&r);
}

void gui_bar(int x, int y, int w, int h, Uint32 c, SDL_Surface *dst){
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	SDL_FillRect(dst, &r, SDL_MapRGB(dst->format, 0, 0, 0));
	gui_box(x, y, w, h, c, dst);
}

static void breakhandler(int a){
	die = 1;
}