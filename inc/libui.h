#ifndef LIBUI_H
# define LIBUI_H

# define PURPLE (SDL_Color){118, 91, 167, 255}
# define ORANGE (SDL_Color){255, 108, 47, 255}
# define TEAL (SDL_Color){0, 131, 138, 255}
# define COLOR_BG (SDL_Color){20, 20, 20, 255}
# define COLOR_WHITE (SDL_Color){255, 255, 255, 255}
# define COLOR_TRANSPARENT (SDL_Color){0, 0, 0, 0}

# include <stdbool.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL.h>
# include <SDL2/SDL_ttf.h>



typedef struct ui_globalApp_s ui_globalApp_t;
typedef struct ui_box_s ui_box_t;
typedef struct ui_win_s ui_win_t;
typedef void (*ui_bhook_fn_t)(ui_box_t*, SDL_Event*, void*);


typedef struct ui_layer_s {
	short			state;
	SDL_Texture*	texture;
	ui_box_t*		parent_box;
	SDL_Rect		area;
	struct ui_layer_s *next;
} ui_layer_t;

typedef struct ui_scale_s {
    float x;
    float y;
} ui_scale_t;

SDL_Texture *create_white_texture(SDL_Renderer *renderer, int w, int h);
SDL_Point ui_box_mousepos(ui_box_t *box);
SDL_Point ui_win_mousepos(ui_win_t *win);
// bool ui_is_mouse_in(ui_box_t*);
int ui_init();
int ui_quit();
char *ui_get_time(void);
void *ui_realloc(void *ptr, size_t old_size, size_t new_size);
SDL_Rect ui_area_center(SDL_Rect from, SDL_Rect to_center);
SDL_Rect ui_area(int, int, int, int);
void	reset_state_and_input(ui_globalApp_t* app, bool* running);
// LAYERS

void transfert_all_input(ui_globalApp_t* app, ui_box_t* box);
ui_layer_t*		ui_layer_selected(ui_layer_t* layers, SDL_Point* p);
ui_layer_t*		ui_layer_make(ui_box_t* cnv, SDL_Texture* texture);
void			ui_layer_clean(ui_layer_t** list);
int 			ui_layer_count(ui_layer_t* layers);
void			ui_layer_add(ui_layer_t** layers, ui_layer_t* new);

# include "ui_win.h"
# include "ui_box.h"
# include "ui_global.h"
# include "ui_elem.h"
#endif
