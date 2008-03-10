// (c) 2007-2008 David Roberts

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include "cube.h"

#include "softlines.h"

cube::cube() {
	m_x_orient = 2*M_PI * double(rand())/double(RAND_MAX);
	m_y_orient = 2*M_PI * double(rand())/double(RAND_MAX);
	
	m_mouse_left = false;
	m_quit = false;
	m_last_mouse_x = -1;
	m_last_mouse_y = -1;
	m_last_ticks = 0;
	
	// SDL init
	if(SDL_Init(SDL_INIT_VIDEO) == -1) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}
	
	m_videoflags = SDL_ANYFORMAT | SDL_SWSURFACE | SDL_RESIZABLE; // | SDL_FULLSCREEN;
	
	m_screen = SDL_SetVideoMode(640, 480, 0, m_videoflags);
	if(m_screen == NULL) {
		fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
		exit(2);
	}
	
	// set window caption
	SDL_WM_SetCaption("Cube","Cube");
	
	draw();
}

cube::~cube() {
	SDL_Quit();
}

void cube::loop() {
	do {
		while(SDL_PollEvent(&m_event)) {
			if(m_event.type == SDL_VIDEORESIZE) {
				m_screen = SDL_SetVideoMode(m_event.resize.w, m_event.resize.h, 0, m_videoflags);
				if(m_screen == NULL ) {
					fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
					exit(2);
				}
				draw();
			}
			
			if(m_event.type == SDL_MOUSEBUTTONDOWN && m_event.button.button == SDL_BUTTON_LEFT) m_mouse_left = true;
		
			if(m_event.type == SDL_MOUSEBUTTONUP) m_mouse_left = false;
		
			if(m_event.type == SDL_MOUSEMOTION) {
				m_mouse_x = m_event.motion.x;
				m_mouse_y = m_event.motion.y;
			}
			
			if(m_event.type == SDL_QUIT) m_quit = true;
		}
		
		if(m_mouse_left) {
			if(m_last_mouse_x != -1) {
				int mx = m_mouse_x - m_last_mouse_x;
				int my = m_mouse_y - m_last_mouse_y;
				if(mx || my) {
					m_x_orient += mx * 2*M_PI/500;
					m_y_orient -= my * 2*M_PI/500;
					draw();
				}
			}
			m_last_mouse_x = m_mouse_x;
			m_last_mouse_y = m_mouse_y;
		} else {
			m_last_mouse_x = -1;
			m_last_mouse_y = -1;
		}
		
		// do not let the event cycle consume the whole processor
		uint cur_ticks = SDL_GetTicks();
		int rem_ticks = 40 - (cur_ticks - m_last_ticks);
		if(rem_ticks >= 0) SDL_Delay(rem_ticks);
		m_last_ticks = cur_ticks;
	} while(!m_quit);
}

void cube::draw() {
	cls();
	
	SDL_LockSurface(m_screen);
	
	double h_side = sqrt(2)/2;
	
	double disp = h_side * cos(m_y_orient); //projected displacement between top and bottom of cube
	
	for(double i = m_x_orient; i < m_x_orient + 2*M_PI; i += M_PI/2) { //iterate through remaining 3 vertical edges
		double lx = cos(i + M_PI/2); double x = cos(i);
		double ly = sin(i + M_PI/2) * sin(m_y_orient); double y = sin(i) * sin(m_y_orient);
		draw_line(x/2, (y+disp)/2, lx/2, (ly+disp)/2); //top edge
		draw_line(x/2, (y-disp)/2, lx/2, (ly-disp)/2); //bottom edge
		draw_line(x/2, (y+disp)/2, x/2, (y-disp)/2); //vertical edge
	}
	
	SDL_UnlockSurface(m_screen);
	SDL_UpdateRect(m_screen, 0, 0, 0, 0);
}

void cube::draw_line(double x1, double y1, double x2, double y2) {
	int hw = m_screen->w / 2;
	int hh = m_screen->h / 2;
	
	line(m_screen, int(hw+hh*x1), m_screen->h - int(hh+hh*y1), int(hw+hh*x2), m_screen->h - int(hh+hh*y2), SDL_MapRGB(m_screen->format, 0, 0, 0));
}

void cube::cls() {
	SDL_FillRect(m_screen, NULL, SDL_MapRGB(m_screen->format, 255, 255, 255));
}

int main() {
	cube mcube = cube();
	mcube.loop();
	return 0;
}
