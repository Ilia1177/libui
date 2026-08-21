#include "libui.h"

SDL_Point ui_box_mousepos(ui_box_t *box) 
{
	SDL_Point win_mouse;

	win_mouse = ui_win_mousepos(box->win);
    return (SDL_Point){
        win_mouse.x - box->area.x,
        win_mouse.y - box->area.y
    };
}

