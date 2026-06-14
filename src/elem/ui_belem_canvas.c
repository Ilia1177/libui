#include "libui.h"

static void	ui_bhook_canvassize(ui_box_t* b, SDL_Event *e, void* data)
{
	ui_log("hook update canvas size");
	ui_win_t* win;
	ui_box_t* menu;
	(void)e;
	(void)data;

	win = b->parent_window;
	menu = win->boxes;
	int menuH = menu->area.h;
	int menuW = menu->area.w;
	int winH = win->area.h;
	int winW = win->area.w;
	if (menuW == winW && menuH == winH) {
		return;
	} else if (menuW == winW) {
		b->area = (SDL_Rect) {0, menuH, winW, winH - menuH};
	} else if (menuH == winH) {
		b->area = (SDL_Rect) {menuW, 0, winW - menuW, winH};
	}
}

ui_box_t *ui_belem_canvas(ui_win_t* win) 
{
	win->canvas = ui_box_create(win, ui_area(0,0,0,0), win->colors[3]);
	ui_bhook_append(&win->canvas->on_window_event, ui_bhook_canvassize);
	return win->canvas;
}
