#include "ui_win.h"
#include <unistd.h>

int ui_win_init_handlers(ui_win_t* win)
{
	ui_whook_add(&win->update, ui_whook_update_default);
	ui_whook_add(&win->destroy ,ui_whook_destroy_default);
	ui_whook_add(&win->render ,ui_whook_render_default);
	ui_whook_add(&win->on_click_down, ui_whook_clickdown_default);
	ui_whook_add(&win->on_click_up, ui_whook_clickup_default);
	ui_whook_add(&win->on_mouse_motion, ui_whook_mousemotion_default);
	ui_whook_add(&win->on_window_event, ui_whook_windowevent_default);

	return 0;
}

int ui_win_init(ui_globalApp_t*app, ui_win_t* win)
{
	win->renderer = SDL_CreateRenderer(
        win->ptr, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!win->renderer) {
        return -1;
    }
	SDL_GetWindowPosition(win->ptr, &win->area.x, &win->area.y);
	SDL_GL_GetDrawableSize(win->ptr, &win->area.w, &win->area.h);
	ui_win_get_scale(win);
    win->id = SDL_GetWindowID(win->ptr);
	win->font = TTF_OpenFont("Abell Extended.ttf", 38);
	TTF_SetFontStyle(win->font, TTF_STYLE_BOLD);
	if (!win->font)
		fprintf(stderr, "TTF_OpenFont error: %s\n", TTF_GetError());
	ui_win_init_handlers(win);
	win->background_color = (SDL_Color){128, 128, 128, 255};
	win->global = app;
	win->colors = calloc(sizeof(SDL_Color), 5);
	win->colors[1] = PURPLE;
	win->colors[2] = ORANGE;
	win->colors[3] = TEAL;
	win->colors[4] = COLOR_BG;
    // Initialize event handler pointers to NULL
	return 0;
 
}
// create window type in drawable sixel (real pixels)
// area.x <= 0 makes the window centered on the x axe
// area.y <= 0 makes the window centered on the y axe
// area.<w|h> <= 0 makes the window resizable with w=400 h=200
ui_win_t* ui_win_create(ui_globalApp_t* app, SDL_Rect area, char* title)
{
	ui_win_t *win;
	Uint32 flags;

	win = (ui_win_t*)calloc(sizeof(ui_win_t), 1);

	flags = SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP;
	if (!win)
		return NULL;
	if (area.x <= 0 || area.w <= 0 || area.h <= 0)
		area.x = SDL_WINDOWPOS_CENTERED;
	if (area.y <= 0 || area.w <= 0 || area.h <= 0)
		area.y = SDL_WINDOWPOS_CENTERED;
	if (area.w <= 0 || area.h <= 0) {
		flags |= SDL_WINDOW_RESIZABLE;
		area.w = 400;
		area.h = 200;
	}
	win->ptr = SDL_CreateWindow(title, area.x, area.y, area.w, area.h, flags);
	if (!win->ptr) {
        return NULL;
	}

	if (ui_win_init(app, win) != 0) {
		return NULL;
	}
	return win;
}


