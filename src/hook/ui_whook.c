#include "ui_win.h"

int ui_whook_windirty(ui_win_t* win, SDL_Event *e, void *data) {
	(void)data;
	(void)e;
	if (!win)
		return 0;
	win->state |= WIN_DIRTY;
	return 1;
}

int ui_whook_quitkey(ui_win_t* win, SDL_Event *e, void *data)
{
	(void)data;

	if (!e || !win) {
		return 1;
	} else if (e->type == SDL_KEYDOWN) {
		switch (e->key.keysym.sym) {
			case SDLK_ESCAPE:
				win->state |= WIN_QUIT;
				return 0;

			default:
				break;
		}
	}
	return 1;
} 

// ONLY FOR TESTING
int ui_whook_loadimage(ui_win_t* win, SDL_Event* e, void *data) 
{
	(void)e;
	(void)data;
	ui_globalApp_t* app;
	static bool running = false;

	app = win->global;
	// printf("loading: %d, running: %d, input_nb: %d\n", app->state & APP_LOADING, running, app->input_nb);
	app = win->global;
	if (!(app->state & APP_LOADING) && !running) {
		ui_welem_input(app);
		app->state |= APP_LOADING;
		running = true;
		return 1;
	} else if (!(app->state & APP_LOADING) && running) {
		running = false;
		return 0;
	} else if(app->input_nb <= 0)
		return 1;
	SDL_Surface *surface = IMG_Load((char*)app->inputs[0]);
	reset_state_and_input(app, &running);
	if (!surface)
		return -1;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(win->renderer, surface);
	SDL_FreeSurface(surface);
	if (!tex)
		return -1;
	SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
	ui_layer_make(win->canvas, tex);
	win->state |= WIN_DIRTY;
	return 0;
}

