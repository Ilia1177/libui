#include "ui_win.h"

int ui_whook_loadimage(ui_win_t* win, SDL_Event* e, void *data) 
{
	(void)e;
	(void)data;
	ui_globalApp_t* global = win->global;
	ui_win_t *main_window = global->windows;
	ui_box_t* cnv = main_window->canvas;
	SDL_Renderer *renderer = main_window->renderer;
	static bool running = false;

	if (!global->loading && !running) {
		ui_welem_input(win);
		global->loading = true;
		printf("image load running\n");
		running = true;
	}
	if (!global->loading && running) {
		running = false;
		printf("image load cancel\n");
		return 0;
	}
	if(!global->input)
		return 1;
	SDL_Surface *surface = IMG_Load(global->input);
	if (global->input)
		free(global->input);
	global->input = NULL;
	running = false;
	global->loading = false;
	if (!surface) {
		printf("Error loading image\n");
		ui_welem_message(main_window, "cannot load file");
		printf("popup created\n");
		return -1;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	if (!texture)
		return -1;
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	ui_layer_add(&cnv->layers, ui_layer_create(cnv, texture));
	return 0;
}
