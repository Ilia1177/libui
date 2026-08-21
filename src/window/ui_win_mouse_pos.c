#include "libui.h"

SDL_Point ui_win_mousepos(ui_win_t* win) 
{
	(void)win;
    int gx, gy;
    SDL_GetGlobalMouseState(&gx, &gy);

	ui_scale_t scale = ui_win_get_scale(win);

   // global mouse → window-relative logical coords
	int lx = gx - win->area.x;
	int ly = gy - win->area.y;
	// logical → physical pixels
	int px = (int)(lx * scale.x);
	int py = (int)(ly * scale.y);

	SDL_Point p = {px, py};
	return p;
}
