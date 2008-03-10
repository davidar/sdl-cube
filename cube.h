// (c) 2007-2008 David Roberts

#ifndef CUBE_H
#define CUBE_H

#include <SDL/SDL.h>

class cube {
	public:
		explicit cube();
		virtual ~cube();
		void loop();
	
	protected:
		void cls();
		void draw_line(double, double, double, double);
		void draw();
		
		double m_x_orient;
		double m_y_orient;
		SDL_Surface* m_screen;
		bool m_mouse_left;
		int m_mouse_x;
		int m_mouse_y;
		bool m_quit;
		SDL_Event m_event;
		uint m_videoflags;
		int m_last_mouse_x;
		int m_last_mouse_y;
		uint m_last_ticks;
};

#endif
