#include "ui_win.h"
// #include "libft.h"
#include <unistd.h>

int ui_win_init_handlers(ui_win_t* win)
{
	// ui_whook_add(&win->update,ui_whook_update_default);
	ui_whook_prepend(&win->destroy ,ui_whook_destroy_default);
	ui_whook_prepend(&win->render ,ui_whook_render_default);
	ui_whook_prepend(&win->on_click_down, ui_whook_clickdown_default);
	ui_whook_prepend(&win->on_key_down, ui_whook_keydown_default);
	ui_whook_prepend(&win->on_mouse_wheel, ui_whook_mousewheel_default);
	ui_whook_prepend(&win->on_click_up, ui_whook_clickup_default);
	ui_whook_prepend(&win->on_mouse_motion, ui_whook_mousemotion_default);
	ui_whook_prepend(&win->on_window_event, ui_whook_windowevent_default);

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
	SDL_RenderGetScale(win->renderer, &app->scale_x, &app->scale_y);
	ui_win_get_scale(win);
    win->id = SDL_GetWindowID(win->ptr);
	win->font = TTF_OpenFont("AgentExtLgtDB Normal.ttf", 24);
	if (!win->font) {
		printf("Font not provided\n");
		return 2;
	}
	TTF_SetFontStyle(win->font, TTF_STYLE_BOLD);
	ui_win_init_handlers(win);
	win->background_color = COLOR_BG;
	win->global = app;
	win->colors = calloc(sizeof(SDL_Color), 5);
	win->colors[0] = (SDL_Color) {0, 0, 0, 127};
	win->colors[1] = (SDL_Color) {0, 0, 0, 127};
	win->colors[2] = (SDL_Color) {0, 0, 0, 127};
	win->colors[3] = (SDL_Color) {0, 0, 0, 127};
	win->zoom = 1.0f;
    // Initialize event handler pointers to NULL
	win->cache = SDL_CreateTexture(
		win->renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		win->area.w,
		win->area.h
	);
	return 0;
}

// create window type in drawable sixel (real pixels)
// area.x <= 0 makes the window centered on the x axe
// area.y <= 0 makes the window centered on the y axe
// area.<w|h> <= 0 makes the window resizable with w=400 h=200
ui_win_t* ui_win_create(ui_globalApp_t* app, SDL_Rect area, const char* title, uint32_t flags)
{
	ui_win_t *win;

	win = (ui_win_t*)calloc(sizeof(ui_win_t), 1);
	if (!win || !title)
		return NULL;
	if (area.x <= 0)
		area.x = SDL_WINDOWPOS_CENTERED;
	if (area.y <= 0)
		area.y = SDL_WINDOWPOS_CENTERED;
	if (area.w <= 0 || area.h <= 0) {
		area.w = 400;
		area.h = 200;
	}
	flags |= SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_ALWAYS_ON_TOP;
	win->ptr = SDL_CreateWindow(title, area.x, area.y, area.w, area.h, flags);
	if (!win->ptr) {
        return NULL;
	}
	int status = ui_win_init(app, win);
	if (status) {
		return NULL;
	}
	win->label = title;
	ui_win_add(&app->windows, win);
	return win;
}

ui_win_t* ui_get_win_by_name(ui_globalApp_t* app, const char* name)
{
	ui_win_t* curr;

	curr = app->windows;
	while(curr) {
		if (!strncmp(name, curr->label, strlen(curr->label))) {
			return curr;
		}
		curr = curr->next;
	}
	return NULL;
}
