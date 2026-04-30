#include "ui_win.h"
#include <unistd.h>

// create window type in drawable sixel (real pixels)
ui_win_t* ui_win_create(int w, int h, char* title)
{
	ui_win_t *window = (ui_win_t*)malloc(sizeof(ui_win_t));

	if (!window) {
		return NULL;
	}
	// window->w = w;
	// window->h = h;
	window->flags = 0; // Initialize all flags to 0
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

	window->flags |= WIN_DIRTY;
	ui_win_get_scale(window);
	window->update = ui_win_update;
	window->destroy = ui_win_destroy;
	window->render = ui_win_render;

	window->on_key_down = NULL;
	window->on_key_up = NULL;
	window->on_mouse_down = NULL;
	window->on_click_up = NULL;
	window->on_mouse_wheel = NULL;
	window->on_mouse_motion = ui_win_on_mouse_motion_handler;
	// window->on_mouse_motion = NULL;
	window->on_windows_event = NULL;

	window->background_color.r = 128;
	window->background_color.g = 128;
	window->background_color.b = 128;
	window->background_color.a = 128;

    // Initialize event handler pointers to NULL
    // window->on_key_down = NULL;
    // window->on_key_up = NULL;
    // window->on_mouse_button_down = NULL;
    // window->on_mouse_button_up = NULL;
    // window->on_mouse_motion = NULL;
    // window->on_mouse_wheel = NULL;
    // window->on_window_resized = NULL; // Initialize window resized handler

	return window;
}


