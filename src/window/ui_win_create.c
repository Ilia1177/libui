#include "ui_win.h"
#include <unistd.h>


// create window type in drawable sixel (real pixels)
ui_win_t* ui_win_create(int w, int h, ui_globalApp_t* app, char* title)
{
	ui_win_t *window = (ui_win_t*)malloc(sizeof(ui_win_t));

	if (!window) {
		return NULL;
	}
	window->flags = WIN_DIRTY; // Initialize all flags to dirty rendering first
	window->win = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        w, h,
        SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
    );
    if (!window->win) {
        return NULL;
    }
	SDL_GetWindowPosition(window->win, &window->area.x, &window->area.y);
	SDL_GL_GetDrawableSize(window->win, &window->area.w, &window->area.h);  // 1600 x 1200
    window->id = SDL_GetWindowID(window->win); // Initialize SDL_WindowID
	window->renderer = SDL_CreateRenderer(
        window->win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!window->renderer) {
        return NULL;
    }

	window->font = TTF_OpenFont("CharcoalFirst.ttf", 22);
	TTF_SetFontStyle(window->font, TTF_STYLE_BOLD);

	if (!window->font)
		fprintf(stderr, "TTF_OpenFont error: %s\n", TTF_GetError());

	ui_win_get_scale(window);
	ui_whook_add(&window->update, ui_whook_update_default);
	ui_whook_add(&window->destroy ,ui_whook_destroy_default);
	ui_whook_add(&window->render ,ui_whook_render_default);


	window->on_mouse_motion = NULL;
	ui_whook_add(&window->on_click_down, ui_whook_clickdown_default);
	ui_whook_add(&window->on_click_up, ui_whook_clickup_default);
	ui_whook_add(&window->on_mouse_motion, ui_whook_mousemotion_default);

	window->on_mouse_wheel = NULL;
	window->on_windows_event = NULL;
	window->on_key_down = NULL;
	window->on_key_up = NULL;

	window->background_color.r = 128;
	window->background_color.g = 128;
	window->background_color.b = 128;
	window->background_color.a = 255;

	window->canvas = ui_box_create((SDL_Rect){0, 0, window->area.w, window->area.h}, (SDL_Color){255, 0, 0,255}, window);
	ui_bhook_add(&window->canvas->update, ui_bhook_maxsize);
	ui_bhook_add(&window->canvas->update, ui_bhook_movelayer);
	window->global = app;
    // Initialize event handler pointers to NULL
    window->on_key_down = NULL;
    window->on_key_up = NULL;
    window->on_mouse_wheel = NULL;
    // window->on_window_event = NULL;
	return window;
}


