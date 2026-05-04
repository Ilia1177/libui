#ifndef LIBUI_H
# define LIBUI_H

# include <stdbool.h> // New: Include stdbool.h for 'bool' type
# include <SDL2/SDL_image.h>
# include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>





typedef struct ui_box_s ui_box_t;
typedef struct ui_win_s ui_win_t;

typedef struct ui_layer_s {
	SDL_Texture* texture;
	ui_box_t*	 box;
	SDL_Rect	 dimension;
	struct ui_layer_s *next;
} ui_layer_t;

typedef struct ui_scale_s {
    float x;
    float y;
} ui_scale_t;

SDL_Point ui_box_mousepos(ui_box_t *box);
SDL_Point ui_win_mousepos(ui_win_t *win);
// bool ui_is_mouse_in(ui_box_t*);
int ui_init();
int ui_quit();
char *ui_get_time(void);

ui_box_t*	ui_elem_button(ui_win_t* win);
ui_win_t*	ui_win_popup(ui_win_t* win);
// LAYERS

void	ui_layer_destroy(ui_layer_t** list);
int ui_layer_count(ui_layer_t* layers);
void	ui_layer_loadimage(ui_box_t* cnv, char *path);
void ui_layer_add(ui_layer_t** layers, ui_layer_t* new);
ui_layer_t *ui_layer_create(ui_box_t* cnv, SDL_Texture* texture);

SDL_Rect ui_tool_rectcenter(SDL_Rect, SDL_Rect);
# include "ui_win.h"
# include "ui_global.h"
#endif
